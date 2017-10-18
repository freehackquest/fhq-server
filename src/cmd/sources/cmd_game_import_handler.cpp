#include <cmd_game_import_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameImportHandler::CmdGameImportHandler(){
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
}

QString CmdGameImportHandler::cmd(){
    return "game_import";
}

bool CmdGameImportHandler::accessUnauthorized(){
	return false;
}

bool CmdGameImportHandler::accessUser(){
	return false;
}

bool CmdGameImportHandler::accessTester(){
	return false;
}

bool CmdGameImportHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdGameImportHandler::inputs(){
	return m_vInputs;
};

QString CmdGameImportHandler::description(){
    return "Import game";
}

QStringList CmdGameImportHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGameImportHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
    pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotImplementedYet());
    return;

    // TODO
    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["result"] = QJsonValue("DONE");
    jsonData["m"] = QJsonValue(m);
    pWebSocketServer->sendMessage(pClient, jsonData);
}
