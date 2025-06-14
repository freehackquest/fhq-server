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

#include "update_max_score_game_task.h"
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSqlQuery>  // TODO redesign
#include <QSqlRecord> // TODO redesign
#include <QThread>
#include <employ_database.h>

UpdateMaxScoreGameTask::UpdateMaxScoreGameTask(int gameid) {
  TAG = "UpdateMaxScoreGameTask";
  m_nGameID = gameid;
}

UpdateMaxScoreGameTask::~UpdateMaxScoreGameTask() {}

void UpdateMaxScoreGameTask::run() {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  WsjcppStorageConnection *pConn = pDatabase->getStorageConnection();
  WsjcppLog::info(TAG, "gameid " + std::to_string(m_nGameID));

  /*
  int nSummary = 0;
  {
      StorageSelect query = pConn->prepareSelect("quest");
      query.column("score").agregateSummary().as("sm1");
      query.where("state", "open");
      query.where("gameid", m_nGameID);
      if (!query.exec()) {
          WsjcppLog::err(TAG, select.getError());
          return;
      }
      StorageRow r = select.getRow();
      nSummary = r.getIntValue("sm1");
  }

  {
      StorageUpdate query2 = pConn->prepareUpdate("games");
      query2.column("maxscore", nSummary); // TODO throw
      query2.where("id", m_nGameID); // TODO throw
      if (!query2.exec()) { // Will be not need here...
          WsjcppLog::err(TAG, query2.getError());
      }
  }

  */

  // pConn->selectSingleIntValue(SELECT SUM(score) FROM quest WHERE state =
  // :state AND gameid = :gameid); UPDATE games SET maxscore =

  WsjcppLog::info(TAG, "gameid " + std::to_string(m_nGameID));

  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);
  query.prepare("UPDATE games SET maxscore = (SELECT SUM(score) FROM quest "
                "WHERE state = :state AND gameid = :gameid) WHERE id = :id");
  query.bindValue(":state", "open");
  query.bindValue(":gameid", m_nGameID);
  query.bindValue(":id", m_nGameID);
  if (!query.exec()) {
    WsjcppLog::err(TAG, query.lastError().text().toStdString());
  }
};
