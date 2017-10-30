#include <update_user_rating_task.h>
#include <log.h>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

UpdateUserRatingTask::UpdateUserRatingTask(IWebSocketServer *pWebSocketServer, int nUserID){
	m_pWebSocketServer = pWebSocketServer;
    m_nUserID = nUserID;
    TAG = "UpdateUserRatingTask";
};

UpdateUserRatingTask::~UpdateUserRatingTask(){
	
}

void UpdateUserRatingTask::run(){
    Log::info(TAG, "userid " + QString::number(m_nUserID));
	QSqlDatabase db = *(m_pWebSocketServer->database());
    QSqlQuery query(db);
    query.prepare(""
                "SELECT "
                "    ifnull(SUM(quest.score),0) as sum_score  "
                "FROM "
                "    users_quests "
                "INNER JOIN "
                "    quest ON quest.idquest = users_quests.questid "
                "WHERE"
                "    (users_quests.userid = :userid) "
    );
    query.bindValue(":userid", m_nUserID);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
        return;
    }

    int nNewRating = 0;
    if (query.next()) {
        QSqlRecord record = query.record();
        nNewRating = record.value("sum_score").toInt();
    }

    QSqlQuery query_update(db);
    query_update.prepare("UPDATE users SET rating = :rating WHERE id = :userid");
    query_update.bindValue(":rating", nNewRating);
    query_update.bindValue(":userid", m_nUserID);

    if(!query_update.exec()){
        Log::err(TAG, query_update.lastError().text());
        return;
    }
}
