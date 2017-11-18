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
#include <memory_cache_serversettings.h>

CmdGameExportHandler::CmdGameExportHandler(){
    TAG = "CmdGameExportHandler";
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
}

QString CmdGameExportHandler::cmd(){
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

const QVector<CmdInputDef> &CmdGameExportHandler::inputs(){
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
    jsonResponse["cmd"] = QJsonValue(cmd());
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
    QString sGameLogoFilename;
    {
        IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serversettings");
        if(pMemoryCache == NULL){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
            return;
        }
        MemoryCacheServerSettings *pMemoryCacheServerSettings = dynamic_cast<MemoryCacheServerSettings*>(pMemoryCache);
        QString sBasePath = pMemoryCacheServerSettings->getSettString("server_folder_games");
        sGameLogoFilename = sBasePath + QString::number(nGameID) + ".png";
    }

    QString tmpDir = QDir::tempPath();
    quint64 ts = QDateTime::currentMSecsSinceEpoch();
    QString tmpZipFile = tmpDir + "/freehackquest-backend-game_" + QString::number(ts) + ".zip";

    Log::info(TAG, "fileName: " + tmpZipFile);

    // prepare zip archive
    QuaZip zip(tmpZipFile);
    zip.open(QuaZip::mdCreate);
    QuaZipFile export_zipfile(&zip);

    // try pack logo
    {
        QFile fileLogo(sGameLogoFilename);
        if(fileLogo.exists()){
            Log::info(TAG, "Try pack logo");
            export_zipfile.open(QIODevice::WriteOnly, QuaZipNewInfo(sUuid.toLower() + ".png"));
            // After .toString(), you should specify a text codec to use to encode the
            // string data into the (binary) file. Here, I use UTF-8:
            QDataStream ts(&fileLogo);
            char *buff = new char[2048];
            memset(buff, 0, 2048);
            while(int readed = ts.readRawData(buff, 2048) > 0){
                export_zipfile.write(buff, readed);
                memset(buff, 0, 2048);
            }
            export_zipfile.close();
        }else{
            Log::warn(TAG, "Logo not found " + sGameLogoFilename);
        }
    }

    // try pack json file
    {
        Log::info(TAG, "Try pack json file");
        export_zipfile.open(QIODevice::WriteOnly, QuaZipNewInfo(sUuid.toLower() + ".json"));
        QJsonDocument doc(jsonGame);
        QString message = doc.toJson(QJsonDocument::Compact);
        export_zipfile.write(message.toUtf8());
        export_zipfile.close();
    }
    zip.close();

    // Qua
    jsonResponse["result"] = QJsonValue("DONE");
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}
