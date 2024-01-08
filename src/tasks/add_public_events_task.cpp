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

#include "add_public_events_task.h"
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSqlQuery>  // TODO redesign
#include <QSqlRecord> // TODO redesign
#include <QString>
#include <QThread>
#include <employ_database.h>

AddPublicEventsTask::AddPublicEventsTask(
  const std::string &sType, const std::string &sMessage, const nlohmann::json &jsonMeta
) {
  m_sType = sType;
  m_sMessage = sMessage;
  m_jsonMeta = jsonMeta;
  TAG = "AddPublicEventsTask";
};

AddPublicEventsTask::~AddPublicEventsTask() {}

void AddPublicEventsTask::run() {
  WsjcppLog::info(TAG, "message " + m_sMessage);
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);
  query.prepare("INSERT INTO public_events(type,dt,message,meta) "
                "VALUES(:type,NOW(),:message,:meta)");
  query.bindValue(":type", QString::fromStdString(m_sType));
  query.bindValue(":message", QString::fromStdString(m_sMessage));
  std::string sMeta = m_jsonMeta.dump();
  // WsjcppLog::warn(TAG, "sMeta = " + sMeta);
  query.bindValue(":meta", QString::fromStdString(sMeta));
  if (!query.exec()) {
    WsjcppLog::err(TAG, query.lastError().text().toStdString());
  }
};
