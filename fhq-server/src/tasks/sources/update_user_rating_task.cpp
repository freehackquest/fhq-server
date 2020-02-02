#include <update_user_rating_task.h>
#include <fallen.h>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <employ_database.h>
#include <QSqlQuery> // TODO redesign
#include <QSqlRecord> // TODO redesign

UpdateUserRatingTask::UpdateUserRatingTask(int nUserID) {
    m_nUserID = nUserID;
    TAG = "UpdateUserRatingTask";
}

UpdateUserRatingTask::~UpdateUserRatingTask() {

}

void UpdateUserRatingTask::run() {
    WSJCppLog::info(TAG, "userid " + std::to_string(m_nUserID));
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
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
    if (!query.exec()) {
        WSJCppLog::err(TAG, query.lastError().text().toStdString());
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

    if (!query_update.exec()) {
        WSJCppLog::err(TAG, query_update.lastError().text().toStdString());
        return;
    }
}
