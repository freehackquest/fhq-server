#include "update_max_score_game_task.h"
#include <fallen.h>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <employ_database.h>
#include <QSqlQuery> // TODO redesign
#include <QSqlRecord> // TODO redesign

UpdateMaxScoreGameTask::UpdateMaxScoreGameTask(int gameid) {
    TAG = "UpdateMaxScoreGameTask";
    m_nGameID = gameid;
}

UpdateMaxScoreGameTask::~UpdateMaxScoreGameTask() {
    
}

void UpdateMaxScoreGameTask::run() {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    WsjcppStorageConnection *pConn = pDatabase->getStorageConnection();
    WsjcppLog::info(TAG, "gameid " + std::to_string(m_nGameID));

    /*
    int nSummary = 0;
    {
        StorageSelect query = pConn->prepareSelect("quest");
        query.column("score").agregateSummary().as("sm1");
        query.where("state", "open");
        query.where("gameid", m_nGameID);
        if (!query.exec()) {
            WsjcppLog::err(TAG, select.getError());
            return;
        }
        StorageRow r = select.getRow();
        nSummary = r.getIntValue("sm1");
    }

    {
        StorageUpdate query2 = pConn->prepareUpdate("games");
        query2.column("maxscore", nSummary); // TODO throw
        query2.where("id", m_nGameID); // TODO throw 
        if (!query2.exec()) { // Will be not need here...
            WsjcppLog::err(TAG, query2.getError());
        }
    }
    
    */

    // pConn->selectSingleIntValue(SELECT SUM(score) FROM quest WHERE state = :state AND gameid = :gameid);
    // UPDATE games SET maxscore = 

    WsjcppLog::info(TAG, "gameid " + std::to_string(m_nGameID));
    
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("UPDATE games SET maxscore = (SELECT SUM(score) FROM quest WHERE state = :state AND gameid = :gameid) WHERE id = :id");
    query.bindValue(":state", "open");
    query.bindValue(":gameid", m_nGameID);
    query.bindValue(":id", m_nGameID);
    if (!query.exec()) {
        WsjcppLog::err(TAG, query.lastError().text().toStdString());
    }
};
