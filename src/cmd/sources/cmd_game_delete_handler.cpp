#include <cmd_game_delete_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameDeleteHandler::CmdGameDeleteHandler(){
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

QString CmdGameDeleteHandler::cmd(){
    return "game_delete";
}

bool CmdGameDeleteHandler::accessUnauthorized(){
	return false;
}

bool CmdGameDeleteHandler::accessUser(){
	return false;
}

bool CmdGameDeleteHandler::accessTester(){
	return false;
}

bool CmdGameDeleteHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdGameDeleteHandler::inputs(){
	return m_vInputs;
};

QString CmdGameDeleteHandler::description(){
    return "Delete the game";
}

QStringList CmdGameDeleteHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGameDeleteHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
    // TODO

    pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotImplementedYet());
    return;

    QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
    pWebSocketServer->sendMessage(pClient, jsonData);
}
