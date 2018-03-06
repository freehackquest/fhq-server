#include <cmd_quest_statistics_handler.h>
#include <runtasks.h>
#include <log.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdQuestStatisticsHandler::CmdQuestStatisticsHandler(){
	m_vInputs.push_back(CmdInputDef("questid").integer_().required().description("Quest ID"));
    TAG = "CmdQuestStatisticsHandler";
}

std::string CmdQuestStatisticsHandler::cmd(){
    return "quest_statistics";
}

bool CmdQuestStatisticsHandler::accessUnauthorized(){
    return true;
}

bool CmdQuestStatisticsHandler::accessUser(){
	return true;
}

bool CmdQuestStatisticsHandler::accessTester(){
	return true;
}

bool CmdQuestStatisticsHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdQuestStatisticsHandler::inputs(){
	return m_vInputs;
};

QString CmdQuestStatisticsHandler::description(){
	return "Update the quest info";
}

QStringList CmdQuestStatisticsHandler::errors(){
	QStringList	list;
	return list;
}

void CmdQuestStatisticsHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;


    QSqlDatabase db = *(pRequest->server()->database());

    int nQuestID = jsonRequest["questid"].toInt();

    // check quest exists
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", nQuestID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }

        if (!query.next()) {
            pRequest->sendMessageError(cmd(), Error(404, "Quest not found"));
            return;
        }
    }


    QString sQuery = "SELECT COUNT(t0.id) AS cnt FROM users_quests_answers t0 "
    "INNER JOIN users t1 ON t1.id = t0.userid "
    "WHERE t0.questid = :questid AND t0.passed = :passed AND t1.role = :role ";

    // tries
    {
        QSqlQuery query(db);
        query.prepare(sQuery);
        query.bindValue(":questid", nQuestID);
        query.bindValue(":passed", "No");
        query.bindValue(":role", "user");
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }

        if (query.next()) {
            QSqlRecord record = query.record();
            jsonResponse["tries"] = record.value("cnt").toInt();
        }else{
            pRequest->sendMessageError(cmd(), Error(404, "Quest not found"));
            return;
        }
    }

    // solved
    {
        QSqlQuery query(db);
        query.prepare(sQuery);
        query.bindValue(":questid", nQuestID);
        query.bindValue(":passed", "Yes");
        query.bindValue(":role", "user");
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }

        if (query.next()) {
            QSqlRecord record = query.record();
            jsonResponse["solved"] = record.value("cnt").toInt();
        }else{
            pRequest->sendMessageError(cmd(), Error(404, "Quest not found"));
            return;
        }
    }

    // how solved this quest
    {
        QSqlQuery query(db);
        query.prepare("SELECT t0.id, t0.logo, t0.nick, t0.university FROM users t0 "
                      "INNER JOIN users_quests t1 ON t0.id = t1.userid "
                      "WHERE t0.role = :role AND t1.questid = :questid ");
        query.bindValue(":role", "user");
        query.bindValue(":questid", nQuestID);

        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        QJsonArray users;
        while(query.next()) {
            QSqlRecord record = query.record();
            QJsonObject user;
            user["userid"] = record.value("id").toInt();
            user["logo"] = record.value("logo").toString().toHtmlEscaped();
            user["nick"] = record.value("nick").toString().toHtmlEscaped();
            user["university"] = record.value("university").toString().toHtmlEscaped();
            users.append(user);
        }
        jsonResponse["users"] = users;
    }

    // TODO check this
    // jsonResponse["data"] = jsonData;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
