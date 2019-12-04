#include "update_user_location_task.h"
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <fallen.h>
#include <employ_database.h>
#include <QSqlQuery> // TODO redesign
#include <QSqlRecord> // TODO redesign
#include <parser_ip_api_com.h>

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

            // TODO parse the {"message":"reserved range","query":"127.0.0.1","status":"fail"}

            QNetworkRequest request(url);
            QNetworkReply *pReply = manager.get(request);
            QEventLoop eventLoop;
            QObject::connect(pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
            eventLoop.exec();
            QByteArray data = pReply->readAll();
            std::string sJson = data.toStdString();
            ParserIpApiCom p;
            if (!p.parse(m_sLastIP, sJson)) {
                return;
            }
            std::string sCountry = p.getCountry();
            std::string sRegion = p.getRegionName();
            std::string sCity = p.getCity();
            double nLat = p.getLat();
            double nLon = p.getLon();

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
            query_update.bindValue(":country", QString::fromStdString(sCountry));
            query_update.bindValue(":region", QString::fromStdString(sRegion));
            query_update.bindValue(":city", QString::fromStdString(sCity));
            query_update.bindValue(":latitude", nLat);
            query_update.bindValue(":longitude", nLon);
            query_update.bindValue(":id", m_nUserID);
            if (!query_update.exec()) {
                Log::err(TAG, query_update.lastError().text().toStdString());
            }
        } else {
            Log::info(TAG, "IP address same for userid = " + std::to_string(m_nUserID));    
        }
    } else {
        Log::err(TAG, "failed for userid = " + std::to_string(m_nUserID) + "(not found userid in database)");
    }
};
