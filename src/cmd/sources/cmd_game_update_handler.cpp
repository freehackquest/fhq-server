#include <cmd_game_update_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameUpdateHandler::CmdGameUpdateHandler(){
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
    m_vInputs.push_back(CmdInputDef("logo").string_().required().description("Logo"));
    m_vInputs.push_back(CmdInputDef("name").string_().required().description("Name of the Game"));
    m_vInputs.push_back(CmdInputDef("description").string_().required().description("Description of the Game"));
    m_vInputs.push_back(CmdInputDef("state").string_().required().description("State of the game"));
    m_vInputs.push_back(CmdInputDef("form").string_().required().description("Form of the game"));
    m_vInputs.push_back(CmdInputDef("type").string_().required().description("Type of the game"));
    m_vInputs.push_back(CmdInputDef("date_start").string_().required().description("Date start"));
    m_vInputs.push_back(CmdInputDef("date_stop").string_().required().description("Date stop"));
    m_vInputs.push_back(CmdInputDef("date_restart").string_().required().description("Date restart"));
    m_vInputs.push_back(CmdInputDef("organizators").string_().required().description("Organizators"));
}

QString CmdGameUpdateHandler::cmd(){
    return "game_update";
}

bool CmdGameUpdateHandler::accessUnauthorized(){
	return false;
}

bool CmdGameUpdateHandler::accessUser(){
	return false;
}

bool CmdGameUpdateHandler::accessTester(){
	return false;
}

bool CmdGameUpdateHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdGameUpdateHandler::inputs(){
	return m_vInputs;
};

QString CmdGameUpdateHandler::description(){
	return "Create the quest";
}

QStringList CmdGameUpdateHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGameUpdateHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
    pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotImplementedYet());
    return;

    // TODO
    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["result"] = QJsonValue("DONE");
    jsonData["m"] = QJsonValue(m);
    pWebSocketServer->sendMessage(pClient, jsonData);
}
