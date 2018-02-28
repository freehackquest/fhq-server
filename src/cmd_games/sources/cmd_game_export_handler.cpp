#include <cmd_game_export_handler.h>
#include <runtasks.h>

#include <QTemporaryDir>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QDataStream>
#include <quazip.h>
#include <quazipfile.h>
#include <quazipfileinfo.h>
#include <log.h>
#include <iostream>
#include <employ_settings.h>

CmdGameExportHandler::CmdGameExportHandler(){
    TAG = "CmdGameExportHandler";
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
}

std::string CmdGameExportHandler::cmd(){
    return "game_export";
}

bool CmdGameExportHandler::accessUnauthorized(){
	return false;
}

bool CmdGameExportHandler::accessUser(){
	return false;
}

bool CmdGameExportHandler::accessTester(){
	return false;
}

bool CmdGameExportHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdGameExportHandler::inputs(){
	return m_vInputs;
};

QString CmdGameExportHandler::description(){
    return "Export the game";
}

QStringList CmdGameExportHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGameExportHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(QString(cmd().c_str()));
    jsonResponse["m"] = QJsonValue(m);

    QString sUuid = obj["uuid"].toString();
    QSqlDatabase db = *(pWebSocketServer->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM games WHERE uuid = :gameuuid");
    query.bindValue(":gameuuid", sUuid);

    if(!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }

    QJsonObject jsonGame;
    int nGameID = 0;
    if (query.next()) {
        QSqlRecord record = query.record();
        nGameID  = record.value("id").toInt();
        jsonGame["uuid"] = record.value("uuid").toString();
        jsonGame["title"] = record.value("title").toString();
        jsonGame["type_game"] = record.value("type_game").toString();
        jsonGame["date_start"] = record.value("date_start").toString();
        jsonGame["date_stop"] = record.value("date_stop").toString();
        jsonGame["date_restart"] = record.value("date_restart").toString();
        jsonGame["description"] = record.value("description").toString();
        jsonGame["state"] = record.value("state").toString();
        jsonGame["form"] = record.value("form").toString();
        jsonGame["organizators"] = record.value("organizators").toString();
        jsonGame["maxscore"] = record.value("maxscore").toInt();
    } else {
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Game not found"));
        return;
    }

    // find logo for game
    QString sGameLogoFilename = "";
    {
        EmploySettings *pSettings = findEmploy<EmploySettings>();
        QString sBasePath = pSettings->getSettString("server_folder_games");
        sGameLogoFilename = sBasePath + QString::number(nGameID) + ".png";
    }

    QString tmpDir = QDir::tempPath();
    quint64 ts = QDateTime::currentMSecsSinceEpoch();
    QString tmpZipFile = tmpDir + "/freehackquest-backend-game_" + QString::number(ts) + ".zip";

    // prepare zip archive
    QuaZip zip(tmpZipFile);
    zip.open(QuaZip::mdCreate);
    QuaZipFile export_zipfile(&zip);

    // pack logo
    {
        QFile fileLogo(sGameLogoFilename);
        if(fileLogo.exists() && fileLogo.open(QIODevice::ReadOnly)){
            export_zipfile.open(QIODevice::WriteOnly, QuaZipNewInfo(sUuid.toLower() + ".png"));
            // After .toString(), you should specify a text codec to use to encode the
            // string data into the (binary) file. Here, I use UTF-8:
            QByteArray baLogo = fileLogo.readAll();
            export_zipfile.write(baLogo);
            export_zipfile.close();
        }else{
            Log::warn(TAG, "Logo not found " + sGameLogoFilename);
        }
    }

    // pack json file
    {
        export_zipfile.open(QIODevice::WriteOnly, QuaZipNewInfo(sUuid.toLower() + ".json"));
        QJsonDocument doc(jsonGame);
        QString message = doc.toJson(QJsonDocument::Compact);
        export_zipfile.write(message.toUtf8());
        export_zipfile.close();
    }
    zip.close();

    // preapre zip base64
    {
        QFile fileZip(tmpZipFile);
        if (!fileZip.open(QIODevice::ReadOnly)){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, "Could not open zip file"));
            return;
        }
        QByteArray baZip = fileZip.readAll();
        QJsonObject jsonData;
        jsonData["zipfile_base64"] = QString(baZip.toBase64());
        jsonData["zipfile_name"] = "game_" + sUuid.toLower() + ".zip";
        jsonResponse["data"] = jsonData;
        fileZip.close();
        fileZip.remove();
    }
    jsonResponse["result"] = QJsonValue("DONE");
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}
