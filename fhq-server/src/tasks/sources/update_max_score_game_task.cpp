#include "update_max_score_game_task.h"
#include <log.h>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <employ_database.h>

UpdateMaxScoreGameTask::UpdateMaxScoreGameTask(int gameid){
	TAG = "UpdateMaxScoreGameTask";
	m_nGameID = gameid;
}

UpdateMaxScoreGameTask::~UpdateMaxScoreGameTask(){
	
}

void UpdateMaxScoreGameTask::run(){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

	Log::info(TAG, "gameid " + QString::number(m_nGameID));
		
    QSqlDatabase db = *(pDatabase->database());
	QSqlQuery query(db);
	query.prepare("UPDATE games SET maxscore = (SELECT SUM(score) FROM quest WHERE state = :state AND gameid = :gameid) WHERE id = :id");
	query.bindValue(":state", "open");
	query.bindValue(":gameid", m_nGameID);
	query.bindValue(":id", m_nGameID);
	if(!query.exec()){
		Log::err(TAG, query.lastError().text());
	}
};
