#include <update_quest_solved_task.h>
#include <utils_logger.h>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <employ_database.h>

UpdateQuestSolvedTask::UpdateQuestSolvedTask(int nQuestID){
    m_nQuestID = nQuestID;
    TAG = "UpdateQuestSolvedTask";
}

UpdateQuestSolvedTask::~UpdateQuestSolvedTask(){
    
}

void UpdateQuestSolvedTask::run(){
    Log::info(TAG, "questid " + QString::number(m_nQuestID));
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    query.prepare("UPDATE quest q "
                  "SET "
                  " count_user_solved = ( "
                  "     SELECT COUNT(*) FROM users_quests"
                  "     INNER JOIN users ON users.id = users_quests.userid"
                  "     WHERE questid = :questid AND users.role = 'user'"
                  " ) WHERE q.idquest = :questid2;");

    query.bindValue(":questid", m_nQuestID);
    query.bindValue(":questid2", m_nQuestID);

    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
    }
}
