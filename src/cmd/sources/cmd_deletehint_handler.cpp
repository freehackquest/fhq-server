#include <cmd_deletehint_handler.h>

CmdDeleteHintHandler::CmdDeleteHintHandler(){
	m_vInputs.push_back(CmdInputDef("hintid").required().integer_().description("hint id"));
}

std::string CmdDeleteHintHandler::cmd(){
	return "deletehint";
}

bool CmdDeleteHintHandler::accessUnauthorized(){
	return false;
}

bool CmdDeleteHintHandler::accessUser(){
	return false;
}

bool CmdDeleteHintHandler::accessTester(){
	return false;
}

bool CmdDeleteHintHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdDeleteHintHandler::inputs(){
	return m_vInputs;
};

QString CmdDeleteHintHandler::description(){
	return "Method for delete hint from quest";
}

QStringList CmdDeleteHintHandler::errors(){
	QStringList	list;
	return list;
}

void CmdDeleteHintHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	
	if(pUserToken == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotAuthorizedRequest());
		return;
	}

	int hintid = obj["hintid"].toInt();
	if(hintid == 0){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::HintIDMustBeNotZero());
		return;
	}

	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("DELETE FROM quests_hints WHERE id = :hintid");
	query.bindValue(":hintid", hintid);
	query.exec();

	QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	pWebSocketServer->sendMessage(pClient, jsonData);
}
