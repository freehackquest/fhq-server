#include <cmd_writeups_handler.h>
#include <QJsonArray>

CmdWriteUpsHandler::CmdWriteUpsHandler(){
	m_vInputs.push_back(CmdInputDef("questid").required().integer_().description("Quest ID"));
}

std::string CmdWriteUpsHandler::cmd(){
	return "writeups";
}

bool CmdWriteUpsHandler::accessUnauthorized(){
	return true;
}

bool CmdWriteUpsHandler::accessUser(){
	return true;
}

bool CmdWriteUpsHandler::accessTester(){
	return true;
}

bool CmdWriteUpsHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdWriteUpsHandler::inputs(){
	return m_vInputs;
};

QString CmdWriteUpsHandler::description(){
	return "This method will be returned list of writeups by questid";
}

QStringList CmdWriteUpsHandler::errors(){
	QStringList	list;
	return list;
}

void CmdWriteUpsHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	// bool bConvert = false;

	int questid = obj["questid"].toInt();
	if(questid == 0){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::QuestIDMustBeNotZero());
		return;
	}

	QJsonArray writeups;
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM quests_writeups WHERE questid = :questid");
	query.bindValue(":questid", questid);
	query.exec();
	while (query.next()) {
		QSqlRecord record = query.record();
		int hintid = record.value("id").toInt();
		int questid2 = record.value("questid").toInt();
		QString type = record.value("type").toString();
		QString link = record.value("link").toString();
		QString text = record.value("text").toString();
		QString dt = record.value("dt").toString();
		QJsonObject writeup;
		writeup["writeupid"] = hintid;
		writeup["questid"] = questid2;
		writeup["type"] = type;
		writeup["link"] = link;
		writeup["text"] = text;
		writeup["dt"] = dt;
		writeups.push_back(writeup);
	}

	QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	jsonData["data"] = writeups;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
