#include <cmd_game_info_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameInfoHandler::CmdGameInfoHandler(){
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
}

QString CmdGameInfoHandler::cmd(){
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

const QVector<CmdInputDef> &CmdGameInfoHandler::inputs(){
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
    pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotImplementedYet());
    return;

    // TODO
    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["result"] = QJsonValue("DONE");
    jsonData["m"] = QJsonValue(m);
    pWebSocketServer->sendMessage(pClient, jsonData);
}
