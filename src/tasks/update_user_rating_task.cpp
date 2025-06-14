/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2025 FreeHackQuest <freehackquest@gmail.com>
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

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSqlQuery>  // TODO redesign
#include <QSqlRecord> // TODO redesign
#include <QThread>
#include <employ_database.h>
#include <update_user_rating_task.h>

UpdateUserRatingTask::UpdateUserRatingTask(int nUserID) {
  m_nUserID = nUserID;
  TAG = "UpdateUserRatingTask";
}

UpdateUserRatingTask::~UpdateUserRatingTask() {}

void UpdateUserRatingTask::run() {
  WsjcppLog::info(TAG, "userid " + std::to_string(m_nUserID));
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);
  query.prepare(""
                "SELECT "
                "    ifnull(SUM(quest.score),0) as sum_score  "
                "FROM "
                "    users_quests "
                "INNER JOIN "
                "    quest ON quest.idquest = users_quests.questid "
                "WHERE"
                "    (users_quests.userid = :userid) ");
  query.bindValue(":userid", m_nUserID);
  if (!query.exec()) {
    WsjcppLog::err(TAG, query.lastError().text().toStdString());
    return;
  }

  int nNewRating = 0;
  if (query.next()) {
    QSqlRecord record = query.record();
    nNewRating = record.value("sum_score").toInt();
  }

  QSqlQuery query_update(db);
  query_update.prepare("UPDATE users SET rating = :rating WHERE id = :userid");
  query_update.bindValue(":rating", nNewRating);
  query_update.bindValue(":userid", m_nUserID);

  if (!query_update.exec()) {
    WsjcppLog::err(TAG, query_update.lastError().text().toStdString());
    return;
  }
}
