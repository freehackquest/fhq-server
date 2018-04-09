#include "update_user_location_task.h"
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <log.h>
#include <employ_database.h>

UpdateUserLocationTask::UpdateUserLocationTask(int userid, QString lastip){
	m_nUserID = userid;
	mLastIP = lastip;
	TAG = "UpdateUserLocationTask";
}

UpdateUserLocationTask::~UpdateUserLocationTask(){
	
}

void UpdateUserLocationTask::run(){
	Log::info(TAG, "userid = " + QString::number(m_nUserID) + " start");
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QSqlDatabase db = *(pDatabase->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM users WHERE id = :id");
	query.bindValue(":id", m_nUserID);
	if(!query.exec()){
		Log::err(TAG, query.lastError().text());
	}
	if (query.next()) {
		QSqlRecord record = query.record();
		QString lastip = record.value("last_ip").toString();

		if(lastip != mLastIP){
            Log::info(TAG, "Update user # " + QString::number(m_nUserID) + " location by ip " + mLastIP);
			QNetworkAccessManager manager;
			QUrl url("http://ip-api.com/json/" + mLastIP);
			QNetworkRequest request(url);
			QNetworkReply *pReply = manager.get(request);
			QEventLoop eventLoop;
			QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
			eventLoop.exec();
			QByteArray data = pReply->readAll();

			QJsonDocument jsonResponse = QJsonDocument::fromJson(data);
			QJsonObject jsonObject = jsonResponse.object();
			
			QString country = jsonObject["country"].toString();
			QString region = jsonObject["regionName"].toString();
			QString city = jsonObject["city"].toString();
			double lat = jsonObject["lat"].toDouble();
			double lon = jsonObject["lon"].toDouble();

			Log::info(TAG, "userid = " + QString::number(m_nUserID) + ", update last_ip, city, region, country");

			QSqlQuery query_update(db);
			query_update.prepare("UPDATE users SET "
				"last_ip = :lastip,"
				"country = :country,"
				"region = :region,"
				"city = :city,"
				"latitude = :latitude,"
				"longitude = :longitude "
				" WHERE id = :id");
			query_update.bindValue(":lastip", mLastIP);
			query_update.bindValue(":country", country);
			query_update.bindValue(":region", region);
			query_update.bindValue(":city", city);
			query_update.bindValue(":latitude", lat);
			query_update.bindValue(":longitude", lon);
			query_update.bindValue(":id", m_nUserID);
			if(!query_update.exec()){
				Log::err(TAG, query_update.lastError().text());
			}
        }else{

        }
	}else{
		Log::err(TAG, "failed for userid = " + QString::number(m_nUserID) + "(not found userid in database)");
	}
};
