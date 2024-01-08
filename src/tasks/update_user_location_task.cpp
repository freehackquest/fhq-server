/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2024 FreeHackQuest <freehackquest@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

#include "update_user_location_task.h"
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSqlQuery>  // TODO redesign
#include <QSqlRecord> // TODO redesign
#include <QThread>
#include <employ_database.h>
#include <wsjcpp_geoip.h>

UpdateUserLocationTask::UpdateUserLocationTask(int userid, const std::string &sLastIP) {
  m_nUserID = userid;
  m_sLastIP = sLastIP;
  TAG = "UpdateUserLocationTask";
}

UpdateUserLocationTask::~UpdateUserLocationTask() {}

void UpdateUserLocationTask::run() {
  if (m_sLastIP == "::1") {
    WsjcppLog::info(TAG, "Skip " + m_sLastIP);
    return;
  }
  WsjcppLog::info(TAG, "userid = " + std::to_string(m_nUserID) + " start");
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);
  query.prepare("SELECT * FROM users WHERE id = :id");
  query.bindValue(":id", m_nUserID);
  if (!query.exec()) {
    WsjcppLog::err(TAG, query.lastError().text().toStdString());
  }
  if (query.next()) {
    QSqlRecord record = query.record();
    std::string sLastIP = record.value("last_ip").toString().toStdString();

    if (sLastIP != m_sLastIP) {
      // TODO redesign to curl request
      // TODO redesign check in database same ip first
      WsjcppLog::info(TAG, "Update user # " + std::to_string(m_nUserID) + " location by ip " + m_sLastIP);
      QNetworkAccessManager manager;

      WsjcppGeoIPResult res = WsjcppGeoIP::requestToIpApiCom(m_sLastIP);
      if (res.hasError()) {
        WsjcppLog::info(TAG, m_sLastIP + " -> " + res.getErrorDescription());
      } else {
        WsjcppLog::info(TAG, "userid = " + std::to_string(m_nUserID) + ", update last_ip, city, region, country");

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
        query_update.bindValue(":country", QString::fromStdString(res.getCountry()));
        query_update.bindValue(":region", QString::fromStdString(res.getRegionName()));
        query_update.bindValue(":city", QString::fromStdString(res.getCity()));
        query_update.bindValue(":latitude", res.getLatitude());
        query_update.bindValue(":longitude", res.getLongitude());
        query_update.bindValue(":id", m_nUserID);
        if (!query_update.exec()) {
          WsjcppLog::err(TAG, query_update.lastError().text().toStdString());
        }
      }
    } else {
      WsjcppLog::info(TAG, "IP address same for userid = " + std::to_string(m_nUserID));
    }
  } else {
    WsjcppLog::err(TAG, "failed for userid = " + std::to_string(m_nUserID) + "(not found userid in database)");
  }
};
