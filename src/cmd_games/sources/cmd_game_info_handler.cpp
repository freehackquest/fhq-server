#include <cmd_game_info_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameInfoHandler::CmdGameInfoHandler(){
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
}

std::string CmdGameInfoHandler::cmd(){
    return "game_info";
}

bool CmdGameInfoHandler::accessUnauthorized(){
	return false;
}

bool CmdGameInfoHandler::accessUser(){
	return false;
}

bool CmdGameInfoHandler::accessTester(){
	return false;
}

bool CmdGameInfoHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdGameInfoHandler::inputs(){
	return m_vInputs;
};

QString CmdGameInfoHandler::description(){
    return "Return game info";
}

QStringList CmdGameInfoHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGameInfoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
    QJsonObject jsonData;

    QString sUuid = obj["uuid"].toString().trimmed();

    QSqlDatabase db = *(pWebSocketServer->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM games WHERE uuid = :gameuuid");
    query.bindValue(":gameuuid", sUuid);

    if(!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }

    if (query.next()) {
        QSqlRecord record = query.record();
        jsonData["uuid"] = record.value("uuid").toString();
        jsonData["title"] = record.value("title").toString();
        jsonData["type_game"] = record.value("type_game").toString();
        jsonData["date_start"] = record.value("date_start").toString();
        jsonData["date_stop"] = record.value("date_stop").toString();
        jsonData["date_restart"] = record.value("date_restart").toString();
        jsonData["description"] = record.value("description").toString();
        jsonData["state"] = record.value("state").toString();
        jsonData["form"] = record.value("form").toString();
        jsonData["organizators"] = record.value("organizators").toString();
        jsonData["maxscore"] = record.value("maxscore").toInt();
    } else {
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Game not found"));
        return;
    }

    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(QString(cmd().c_str()));
    jsonResponse["result"] = QJsonValue("DONE");
    jsonResponse["m"] = QJsonValue(m);
    jsonResponse["data"] = jsonData;

    pWebSocketServer->sendMessage(pClient, jsonResponse);
}
