#include <cmd_game_export_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameExportHandler::CmdGameExportHandler(){
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
    pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotImplementedYet());
    return;

    // TODO
    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["result"] = QJsonValue("DONE");
    jsonData["m"] = QJsonValue(m);
    pWebSocketServer->sendMessage(pClient, jsonData);
}
