#include "update_user_location_task.h"
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <utils_logger.h>
#include <employ_database.h>
#include <QSqlQuery> // TODO redesign
#include <QSqlRecord> // TODO redesign
#include <QJsonDocument> // TODO redesign
#include <QJsonObject> // TODO redesign

UpdateUserLocationTask::UpdateUserLocationTask(int userid, const std::string &sLastIP) {
    m_nUserID = userid;
    m_sLastIP = sLastIP;
    TAG = "UpdateUserLocationTask";
}

UpdateUserLocationTask::~UpdateUserLocationTask() {
    
}

void UpdateUserLocationTask::run() {
    Log::info(TAG, "userid = " + std::to_string(m_nUserID) + " start");
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id", m_nUserID);
    if (!query.exec()) {
        Log::err(TAG, query.lastError().text().toStdString());
    }
    if (query.next()) {
        QSqlRecord record = query.record();
        std::string sLastIP = record.value("last_ip").toString().toStdString();

        if (sLastIP != m_sLastIP) {
            // TODO redesign to curl request
            // TODO redesign check in database same ip first
            Log::info(TAG, "Update user # " + std::to_string(m_nUserID) + " location by ip " + m_sLastIP);
            QNetworkAccessManager manager;
            QUrl url("http://ip-api.com/json/" + QString::fromStdString(m_sLastIP));
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

            Log::info(TAG, "userid = " + std::to_string(m_nUserID) + ", update last_ip, city, region, country");

            QSqlQuery query_update(db);
            query_update.prepare("UPDATE users SET "
                "last_ip = :lastip,"
                "country = :country,"
                "region = :region,"
                "city = :city,"
                "latitude = :latitude,"
                "longitude = :longitude "
                " WHERE id = :id");
            query_update.bindValue(":lastip", QString::fromStdString(m_sLastIP));
            query_update.bindValue(":country", country);
            query_update.bindValue(":region", region);
            query_update.bindValue(":city", city);
            query_update.bindValue(":latitude", lat);
            query_update.bindValue(":longitude", lon);
            query_update.bindValue(":id", m_nUserID);
            if (!query_update.exec()) {
                Log::err(TAG, query_update.lastError().text().toStdString());
            }
        } else {
            // nothing
        }
    } else {
        Log::err(TAG, "failed for userid = " + std::to_string(m_nUserID) + "(not found userid in database)");
    }
};
