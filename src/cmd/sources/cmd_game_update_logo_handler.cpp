#include <cmd_game_update_logo_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameUpdateLogoHandler::CmdGameUpdateLogoHandler(){
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
}

QString CmdGameUpdateLogoHandler::cmd(){
    return "game_update_logo";
}

bool CmdGameUpdateLogoHandler::accessUnauthorized(){
	return false;
}

bool CmdGameUpdateLogoHandler::accessUser(){
	return false;
}

bool CmdGameUpdateLogoHandler::accessTester(){
	return false;
}

bool CmdGameUpdateLogoHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdGameUpdateLogoHandler::inputs(){
	return m_vInputs;
};

QString CmdGameUpdateLogoHandler::description(){
    return "Update game logo";
}

QStringList CmdGameUpdateLogoHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGameUpdateLogoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
    pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotImplementedYet());
    return;

    // TODO
    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["result"] = QJsonValue("DONE");
    jsonData["m"] = QJsonValue(m);
    pWebSocketServer->sendMessage(pClient, jsonData);
}
