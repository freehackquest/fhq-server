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

#include "employ_public_events.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <fhq/employees/employ_database.h>
#include <fhq/employees/employ_notify.h>
#include <fhq/employees/employ_public_events.h>
#include <fhq/employees/employ_uuids.h>
#include <wsjcpp_employees.h>

REGISTRY_WJSCPP_EMPLOY(EmployPublicEvents)

EmployPublicEvents::EmployPublicEvents()
  : WsjcppEmployBase(
      EmployPublicEvents::name(),
      {EmployDatabase::name(), EmployUuids::name(), EmployGlobalSettings::name(), EmployNotify::name()}
    ) {
  TAG = EmployPublicEvents::name();
}

bool EmployPublicEvents::init() {
  findWsjcppEmploy<EmployUuids>()->addAllowedTypesOfUuid("public_event");
  return true;
}

bool EmployPublicEvents::deinit() {
  // TODO
  return true;
}

bool EmployPublicEvents::findPublicEvent(int nEventId, ModelPublicEvent &eventInfo, std::string &sErrorMessage) {

  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);
  query.prepare("SELECT * FROM public_events e WHERE id = :eventid");
  query.bindValue(":eventid", nEventId);
  if (!query.exec()) {
    sErrorMessage = query.lastError().text().toStdString();
    WsjcppLog::err(TAG, sErrorMessage);
    return false;
  }

  if (!query.next()) {
    sErrorMessage = "NOT_FOUND";
    return false;
  }

  QSqlRecord record = query.record();
  eventInfo.setDateTime(
    QDateTime::fromString(record.value("dt").toString(), "yyyy-MM-ddThh:mm:ss.zzz").toMSecsSinceEpoch()
  );
  eventInfo.setLocalId(record.value("id").toInt());
  eventInfo.setType(record.value("type").toString().toHtmlEscaped().toStdString());       // TODO htmlspecialchars
  eventInfo.setMessage(record.value("message").toString().toHtmlEscaped().toStdString()); // TODO htmlspecialchars
  eventInfo.setMeta(record.value("meta").toString().toStdString());                       // TODO
  return true;
}

/**
 * @brief Removes a public event by its unique identifier.
 *
 * @deprecated This method is deprecated as of version 0.2.55
 * Use removePublicEventByUuid(const std::string &uuid, std::string &errorMessage) instead.
 *
 * @param[in] eventId The unique ID of the event to be removed.
 * @param[out] errorMessage Reference to a string for error details.
 *
 * @return true if the event was successfully removed, false otherwise.
 */
[[deprecated("Use removePublicEvent(const std::string &uuid, std::string &errorMessage) instead")]]
bool EmployPublicEvents::removePublicEvent(int eventId, std::string &errorMessage) {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);
  query.prepare("SELECT * FROM public_events WHERE id = :eventid");
  query.bindValue(":eventid", eventId);
  if (!query.exec()) {
    errorMessage = query.lastError().text().toStdString();
    return false;
  }
  if (!query.next()) {
    errorMessage = "NOT_FOUND";
    return false;
  }

  QSqlQuery query2(db);
  query2.prepare("DELETE FROM public_events WHERE id = :eventid");
  query2.bindValue(":eventid", eventId);
  query2.exec();

  // TODO notify
  return true;
}

bool EmployPublicEvents::removePublicEventByUuid(const std::string &uuid, std::string &errorMessage) {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  // first search in mysql and then try remove from sqlite
  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);
  query.prepare("SELECT * FROM public_events WHERE uuid = :uuid");
  query.bindValue(":uuid", QString::fromStdString(uuid));
  if (!query.exec()) {
    errorMessage = query.lastError().text().toStdString();
    return false;
  }
  bool bFound = query.next();
  if (bFound) {
    QSqlQuery query2(db);
    query2.prepare("DELETE FROM public_events WHERE id = :uuid");
    query2.bindValue(":uuid", QString::fromStdString(uuid));
    query2.exec();
    return true;
  }
  auto publicEvents = pDatabase->dbPublicEvents();
  if (!publicEvents->deleteRecord(uuid, errorMessage)) {
    return false;
  }
  if (!bFound) {
    errorMessage = "NOT_FOUND";
    return false;
  }
  return true;
}

bool EmployPublicEvents::addPublicEvent(ModelPublicEvent &eventInfo, std::string &sErrorMessage) {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  EmployUuids *pUuids = findWsjcppEmploy<EmployUuids>();

  if (eventInfo.getUuid() == "") {
    eventInfo.setUuid(pUuids->generateNewUuid("public_event"));
  }

  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);
  query.prepare("INSERT INTO public_events(type,message,meta,uuid,dt) "
                "VALUES(:type,:message,:meta,:uuid,NOW())");
  query.bindValue(":type", QString::fromStdString(eventInfo.getType()));
  query.bindValue(":message", QString::fromStdString(eventInfo.getMessage()));
  query.bindValue(":uuid", QString::fromStdString(eventInfo.getUuid()));
  query.bindValue(":meta", "{}");
  if (!query.exec()) {
    sErrorMessage = query.lastError().text().toStdString();
    return false;
  }

  // TODO notify
  return true;
}

bool EmployPublicEvents::findPublicEvents(
  std::vector<ModelPublicEvent> &eventList,
  int nPage,
  int nOnPage,
  const std::string &sType,
  const std::string &sSearch,
  int &nRecordsFound,
  std::string &sErrorMessage
) {
  if (nOnPage > 50) {
    sErrorMessage = "Parameter 'onpage' could not be more then 50";
    return false;
  }

  std::vector<std::string> vFilters;
  QMap<QString, QString> filter_values;

  std::string sType2 = sType;
  WsjcppCore::trim(sType2);
  if (sType2 != "") {
    vFilters.push_back("(e.type = :type)");
    filter_values[":type"] = QString::fromStdString(sType2);
  }

  std::string sSearch2 = sSearch;
  WsjcppCore::trim(sSearch2);
  if (sSearch2 != "") {
    vFilters.push_back("(e.message LIKE :search)");
    filter_values[":search"] = "%" + QString::fromStdString(sSearch2) + "%";
  }

  // TODO redesign join
  std::string sWhere = "";
  for (int i = 0; i < vFilters.size(); i++) {
    if (sWhere.length() > 0) {
      sWhere += " AND ";
    }
    sWhere += vFilters[i];
  }

  if (sWhere.length() > 0) {
    sWhere = "WHERE " + sWhere;
  }

  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  // count quests
  QSqlDatabase db = *(pDatabase->database());

  {
    QSqlQuery query(db);
    query.prepare(
      "SELECT count(*) as cnt FROM public_events e "
      " " +
      QString::fromStdString(sWhere)
    );
    foreach (QString key, filter_values.keys()) {
      query.bindValue(key, filter_values.value(key));
    }
    if (!query.exec()) {
      sErrorMessage = query.lastError().text().toStdString();
      return false;
    }
    if (query.next()) {
      QSqlRecord record = query.record();
      nRecordsFound = record.value("cnt").toInt();
    }
  }

  // data
  {
    QSqlQuery query(db);
    query.prepare(
      "SELECT * FROM public_events e"
      " " +
      QString::fromStdString(sWhere) +
      " ORDER BY dt DESC "
      " LIMIT " +
      QString::number(nPage * nOnPage) + "," + QString::number(nOnPage)
    );
    foreach (QString key, filter_values.keys()) {
      query.bindValue(key, filter_values.value(key));
    }
    if (!query.exec()) {
      sErrorMessage = query.lastError().text().toStdString();
      return false;
    }
    while (query.next()) {
      QSqlRecord record = query.record();
      ModelPublicEvent eventInfo;
      eventInfo.setDateTime(
        QDateTime::fromString(record.value("dt").toString(), "yyyy-MM-ddThh:mm:ss.zzz").toMSecsSinceEpoch()
      );
      eventInfo.setLocalId(record.value("id").toInt());
      eventInfo.setType(record.value("type").toString().toHtmlEscaped().toStdString());       // TODO htmlspecialchars
      eventInfo.setMessage(record.value("message").toString().toHtmlEscaped().toStdString()); // TODO htmlspecialchars
      eventInfo.setMeta(record.value("meta").toString().toStdString());
      eventList.push_back(eventInfo);
    }
  }
  return true;
}