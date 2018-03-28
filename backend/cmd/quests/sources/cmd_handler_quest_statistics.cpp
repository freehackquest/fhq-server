#include <cmd_handler_quest_statistics.h>
#include <runtasks.h>
#include <log.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdHandlerQuestStatistics::CmdHandlerQuestStatistics(){
    TAG = "CmdHandlerQuestStatistics";

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
	m_vInputs.push_back(CmdInputDef("questid").integer_().required().description("Quest ID"));
}

// ---------------------------------------------------------------------

std::string CmdHandlerQuestStatistics::cmd(){
    return "quest_statistics";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerQuestStatistics::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerQuestStatistics::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdHandlerQuestStatistics::description(){
	return "Update the quest info";
}

// ---------------------------------------------------------------------

void CmdHandlerQuestStatistics::handle(ModelRequest *pRequest){
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
