#include <cmd_hints_handler.h>
#include <QJsonArray>

CmdHintsHandler::CmdHintsHandler(){
	m_vInputs.push_back(CmdInputDef("questid").required().integer_().description("Quest id"));
}

std::string CmdHintsHandler::cmd(){
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

const std::vector<CmdInputDef> &CmdHintsHandler::inputs(){
	return m_vInputs;
};

QString CmdHintsHandler::description(){
	return "Output: list of hints, Input: questid";
}

QStringList CmdHintsHandler::errors(){
	QStringList	list;
	return list;
}

void CmdHintsHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    int questid = jsonRequest["questid"].toInt();
	if(questid == 0){
        pRequest->sendMessageError(cmd(), Errors::QuestIDMustBeNotZero());
		return;
	}

	QJsonArray hints;
    QSqlDatabase db = *(pRequest->server()->database());

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

    jsonResponse["data"] = hints;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
