#include "update_user_location_task.h"
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>


UpdateUserLocationTask::UpdateUserLocationTask(IWebSocketServer *pWebSocketServer, int userid, QString lastip){
	m_nUserID = userid;
	m_pWebSocketServer = pWebSocketServer;
	mLastIP = lastip;
};

UpdateUserLocationTask::~UpdateUserLocationTask(){
	
}

void UpdateUserLocationTask::run(){
	qDebug().nospace().noquote() << "UpdateUserLocationTask (uid " << m_nUserID << ")";
		
	QSqlDatabase db = *(m_pWebSocketServer->database());
	QSqlQuery query(db);
	query.prepare("SELECT * FROM users WHERE id = :id");
	query.bindValue(":id", m_nUserID);
	query.exec();
	if (query.next()) {
		QSqlRecord record = query.record();
		QString lastip = record.value("last_ip").toString();

		// mLastIP = "176.49.180.161";
		if(lastip != mLastIP){
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
			double lat = jsonObject["lat"].toDouble(0);
			double lon = jsonObject["lon"].toDouble(0);

			qDebug().nospace().noquote() << "UpdateUserLocationTask (uid " << m_nUserID << ") update last_ip, city, region, country";

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
			query_update.exec();
		}
	}else{
		qDebug().nospace().noquote() << "UpdateUserLocationTask failed for " << m_nUserID;

		// write to server errors
		QString errorInfo = "UpdateUserLocationTask failed (not found userid in database)\n";
		errorInfo += "UserID: " + QString::number(m_nUserID) + "\n";
		errorInfo += "LastIP: " + mLastIP + "\n";
		Errors::WriteServerError(errorInfo);
	}
};
