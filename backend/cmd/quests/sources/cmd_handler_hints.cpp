#include <cmd_handler_hints.h>
#include <QJsonArray>

CmdHandlerHints::CmdHandlerHints(){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
	m_vInputs.push_back(CmdInputDef("questid").required().integer_().description("Quest id"));
}

// ---------------------------------------------------------------------

std::string CmdHandlerHints::cmd(){
	return "hints";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerHints::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerHints::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdHandlerHints::description(){
	return "Output: list of hints, Input: questid";
}

// ---------------------------------------------------------------------

void CmdHandlerHints::handle(ModelRequest *pRequest){
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
