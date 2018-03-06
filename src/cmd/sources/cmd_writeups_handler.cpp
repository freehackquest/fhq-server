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

const std::vector<CmdInputDef> &CmdWriteUpsHandler::inputs(){
	return m_vInputs;
};

QString CmdWriteUpsHandler::description(){
	return "This method will be returned list of writeups by questid";
}

QStringList CmdWriteUpsHandler::errors(){
	QStringList	list;
	return list;
}

void CmdWriteUpsHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

	// bool bConvert = false;

    int questid = jsonRequest["questid"].toInt();
	if(questid == 0){
        pRequest->sendMessageError(cmd(), Errors::QuestIDMustBeNotZero());
		return;
	}

	QJsonArray writeups;
    QSqlDatabase db = *(pRequest->server()->database());
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

    jsonResponse["data"] = writeups;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
