#include <cmd_quests_subjects_handler.h>
#include <QJsonArray>

CmdQuestsSubjectsHandler::CmdQuestsSubjectsHandler(){
	// m_vInputs.push_back(CmdInputDef("filter_text").string_().optional().description("Filter by user email or nick"));
	// m_vInputs.push_back(CmdInputDef("filter_role").string_().optional().description("Filter by user role"));
}

std::string CmdQuestsSubjectsHandler::cmd(){
    return "quests_subjects";
}

bool CmdQuestsSubjectsHandler::accessUnauthorized(){
	return true;
}

bool CmdQuestsSubjectsHandler::accessUser(){
	return true;
}

bool CmdQuestsSubjectsHandler::accessTester(){
	return true;
}

bool CmdQuestsSubjectsHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdQuestsSubjectsHandler::inputs(){
	return m_vInputs;
};

QString CmdQuestsSubjectsHandler::description(){
	return "Method returned list of games";
}

QStringList CmdQuestsSubjectsHandler::errors(){
	QStringList	list;
	return list;
}

void CmdQuestsSubjectsHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject /*obj*/){
    QJsonArray subjects;
	QSqlDatabase db = *(pWebSocketServer->database());
	QSqlQuery query(db);
    query.prepare("SELECT subject, COUNT(*) as cnt FROM `quest` WHERE quest.state = :state GROUP BY subject");
    query.bindValue(":state", "open");

    if(!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }
	while (query.next()) {
		QSqlRecord record = query.record();
        QJsonObject subject;
        subject["subject"] = record.value("subject").toString();
        subject["count"] = record.value("cnt").toInt();
        subjects.push_back(subject);
	}

	QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
    jsonData["data"] = subjects;
		
	pWebSocketServer->sendMessage(pClient, jsonData);
}
