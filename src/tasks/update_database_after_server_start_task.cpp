/**********************************************************************************
 *                      Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * MIT License
 *
 * Copyright (c) 2011-2026 FreeHackQuest <freehackquest@gmail.com>
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
 * Original repository: https://github.com/freehackquest/fhq-server
 *
 ***********************************************************************************/

#include "update_database_after_server_start_task.h"
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSqlError>  // TODO redesign
#include <QSqlQuery>  // TODO redesign
#include <QSqlRecord> // TODO redesign
#include <QString>
#include <QThread>

#include <fhq/employees/employ_database.h>
#include <fhq/employees/employ_notify.h>
#include <fhq/employees/employ_uuids.h>

UpdateDatabaseAfterServerStartTask::UpdateDatabaseAfterServerStartTask() {
  TAG = "UpdateDatabaseAfterServerStartTask";
};

UpdateDatabaseAfterServerStartTask::~UpdateDatabaseAfterServerStartTask() {}

void UpdateDatabaseAfterServerStartTask::run() {
  WsjcppLog::info(TAG, "Begin");
  UpdateMissingUuidsInPublicEvents();
};

void UpdateDatabaseAfterServerStartTask::UpdateMissingUuidsInPublicEvents() {
  auto *pDatabase = findWsjcppEmploy<EmployDatabase>();
  auto *pUuids = findWsjcppEmploy<EmployUuids>();
  auto *pNotify = findWsjcppEmploy<EmployNotify>();

  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);
  query.prepare("SELECT COUNT(*) as cnt FROM public_events WHERE uuid = ''");
  if (!query.exec()) {
    WsjcppLog::err(TAG, query.lastError().text().toStdString());
    return;
  }
  int nCount = 0;
  if (query.next()) {
    QSqlRecord record = query.record();
    nCount = record.value("cnt").toInt();
  }
  WsjcppLog::info(TAG, "Found records for update: " + std::to_string(nCount));
  if (nCount == 0) {
    return;
  }
  query.prepare("SELECT * FROM public_events WHERE uuid = ''");
  if (!query.exec()) {
    std::string sError = query.lastError().text().toStdString();
    WsjcppLog::err(TAG, sError);
    pNotify->notifyDanger(EmployNotify::SERVER, sError);
    return;
  }
  int nProcessed = 0;
  int nPersents = 0;
  WsjcppLog::info(TAG, std::to_string(nPersents) + "% processed");
  while (query.next()) {
    QSqlRecord record = query.record();
    int nId = record.value("id").toInt();
    std::string sUuid = record.value("uuid").toString().toStdString();
    // WsjcppLog::info(TAG, std::to_string(nId) + " => '" + sUuid + "'");
    if (sUuid == "") {
      sUuid = pUuids->generateNewUuid("public_event");
      // WsjcppLog::info(TAG, std::to_string(nId) + " => '" + sUuid + "'");
      QSqlQuery queryUpdate(db);
      queryUpdate.prepare("UPDATE public_events SET uuid = :uuid WHERE id = :id");
      queryUpdate.bindValue(":uuid", QString::fromStdString(sUuid));
      queryUpdate.bindValue(":id", nId);
      if (!queryUpdate.exec()) {
        std::string sError = queryUpdate.lastError().text().toStdString();
        WsjcppLog::err(TAG, sError);
        pNotify->notifyDanger(EmployNotify::SERVER, sError);
        return;
      }
      nProcessed++;
    }
    int nNewPersentd = (nProcessed * 100) / nCount;
    if (nNewPersentd != nPersents) {
      nPersents = nNewPersentd;
      std::string sMessage = "Public events: " + std::to_string(nPersents) + "% processed (" +
                             std::to_string(nProcessed) + "/" + std::to_string(nCount) + ")";
      pNotify->notifyInfo(EmployNotify::SERVER, sMessage);
      WsjcppLog::info(TAG, sMessage);
    }
  }
}