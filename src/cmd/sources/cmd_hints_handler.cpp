#include <cmd_hints_handler.h>
#include <QJsonArray>

CmdHintsHandler::CmdHintsHandler(){
	m_vInputs.push_back(CmdInputDef("questid").required().integer_().description("Quest id"));
}

QString CmdHintsHandler::cmd(){
	return "hints";
}

bool CmdHintsHandler::accessUnauthorized(){
	return true;
}

bool CmdHintsHandler::accessUser(){
	return true;
}

bool CmdHintsHandler::accessTester(){
	return true;
}

bool CmdHintsHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdHintsHandler::inputs(){
	return m_vInputs;
};

QString CmdHintsHandler::description(){
	return "Output: list of hints, Input: questid";
}

QStringList CmdHintsHandler::errors(){
	QStringList	list;
	return list;
}

void CmdHintsHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	
	if(pUserToken == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotAuthorizedRequest());
		return;
	}

	int questid = obj["questid"].toInt();
	if(questid == 0){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::QuestIDMustBeNotZero());
		return;
	}

	QJsonArray hints;
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM quests_hints WHERE questid = :questid");
	query.bindValue(":questid", questid);
	query.exec();
	while (query.next()) {
		QSqlRecord record = query.record();
		int hintid = record.value("id").toInt();
		int questid2 = record.value("questid").toInt();
		QString text = record.value("text").toString();
		QString dt = record.value("dt").toString();
		QJsonObject hint;
        hint["id"] = hintid;
		hint["questid"] = questid2;
		hint["text"] = text;
		hint["dt"] = dt;
		hints.push_back(hint);
	}

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	jsonData["data"] = hints;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
