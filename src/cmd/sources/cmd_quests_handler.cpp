#include <cmd_quests_handler.h>
#include <QJsonArray>

CmdQuestsHandler::CmdQuestsHandler(){
    m_vInputs.push_back(CmdInputDef("subject").string_().optional().description("Filter by subject"));
}

std::string CmdQuestsHandler::cmd(){
    return "quests";
}

bool CmdQuestsHandler::accessUnauthorized(){
	return true;
}

bool CmdQuestsHandler::accessUser(){
	return true;
}

bool CmdQuestsHandler::accessTester(){
	return true;
}

bool CmdQuestsHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdQuestsHandler::inputs(){
	return m_vInputs;
};

QString CmdQuestsHandler::description(){
    return "Method will be returned quest list";
}

QStringList CmdQuestsHandler::errors(){
	QStringList	list;
	return list;
}

void CmdQuestsHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    int nUserID = 0;
    IUserToken *pUserToken = pRequest->userToken();
    if(pUserToken != NULL){
        nUserID = pUserToken->userid();
    }

    QString sSubject = jsonRequest["subject"].toString();

    QJsonArray quests;
    QSqlDatabase db = *(pRequest->server()->database());
	QSqlQuery query(db);

    query.prepare(""
                  "SELECT "
                  "     quest.idquest, "
                  "     quest.name, "
                  "     quest.score, "
                  "     quest.subject, "
                  "     quest.gameid, "
                  "     quest.count_user_solved, "
                  "     users_quests.dt_passed "
                  "FROM "
                  "   quest "
                  "LEFT JOIN"
                  "    users_quests ON users_quests.questid = quest.idquest AND users_quests.userid = :userid "
                  "WHERE"
                  "    quest.state = :state "
                  "    AND quest.subject = :subject "
                  "ORDER BY "
                  "    quest.subject, quest.score ASC, quest.score; ");

    query.bindValue(":userid", nUserID);
    query.bindValue(":state", "open");
    query.bindValue(":subject", sSubject);

    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
	while (query.next()) {
		QSqlRecord record = query.record();
        QJsonObject quest;
        quest["questid"] = record.value("idquest").toInt();
        quest["score"] = record.value("score").toInt();
        quest["name"] = record.value("name").toString();
        quest["gameid"] = record.value("gameid").toInt();
        quest["subject"] = record.value("subject").toString();
        quest["dt_passed"] = record.value("dt_passed").toString();
        quest["solved"] = record.value("count_user_solved").toInt();

        QString status = record.value("dt_passed").toString();
        quest["status"] = (status == "" ? "open" : "completed");

        quests.push_back(quest);
	}

    jsonResponse["data"] = quests;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
