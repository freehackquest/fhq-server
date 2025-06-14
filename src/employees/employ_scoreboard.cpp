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

#include <QMap>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QVariant>
#include <employ_database.h>
#include <employ_scoreboard.h>

REGISTRY_WJSCPP_EMPLOY(EmployScoreboard)

// ---------------------------------------------------------------------

EmployScoreboard::EmployScoreboard() : WsjcppEmployBase(EmployScoreboard::name(), {EmployDatabase::name()}) {
  TAG = "EmployScoreboard";
}

// ---------------------------------------------------------------------

bool EmployScoreboard::init() { return true; }

// ---------------------------------------------------------------------

bool EmployScoreboard::deinit() {
  // TODO
  return true;
}

// ---------------------------------------------------------------------

void EmployScoreboard::loadSync() {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  QSqlDatabase db = *(pDatabase->database());
  m_vRows.clear(); // TODO fix memory leaks

  QSqlQuery query(db);
  query.prepare("SELECT id,nick,logo,university,rating FROM users WHERE role = "
                "'user' ORDER BY rating DESC");
  if (!query.exec()) {
    WsjcppLog::throw_err(TAG, "EmployDatabase::loadAllSettings => " + query.lastError().text().toStdString());
    return;
  };
  while (query.next()) {
    QSqlRecord record = query.record();
    User *pUser = new User();
    pUser->userid = record.value("id").toInt();
    pUser->nick = record.value("nick").toString().toHtmlEscaped().toStdString();
    pUser->university = record.value("university").toString().toHtmlEscaped().toStdString();
    pUser->logo = "public/" + record.value("logo").toString().toHtmlEscaped().toStdString();
    int rating = record.value("rating").toInt();
    int nRow = findScoreboardRowByRating(rating);
    if (nRow < 0) {
      ScoreboardRow *pRow = new ScoreboardRow();
      pRow->rating = rating;
      pRow->vUsers.push_back(pUser);
      m_vRows.push_back(pRow);
    } else {
      m_vRows[nRow]->vUsers.push_back(pUser);
    }
  }

  for (unsigned int i = 0; i < m_vRows.size(); i++) {
    m_vRows[i]->place = i + 1;
  }
}

// ---------------------------------------------------------------------

int EmployScoreboard::findScoreboardRowByRating(int rating) {
  for (unsigned int i = 0; i < m_vRows.size(); i++) {
    if (m_vRows[i]->rating == rating) {
      return i;
    }
  }
  return -1;
}

// ---------------------------------------------------------------------

nlohmann::json EmployScoreboard::toJson() {
  nlohmann::json jsonScoreboard = nlohmann::json::array();

  for (unsigned int i = 0; i < m_vRows.size(); i++) {
    nlohmann::json jsonRow;
    jsonRow["rating"] = m_vRows[i]->rating;
    jsonRow["place"] = m_vRows[i]->place;

    nlohmann::json jsonUsers = nlohmann::json::array();
    ;
    for (unsigned int u = 0; u < m_vRows[i]->vUsers.size(); u++) {
      nlohmann::json jsonUser;
      jsonUser["userid"] = m_vRows[i]->vUsers[u]->userid;
      jsonUser["nick"] = m_vRows[i]->vUsers[u]->nick;
      jsonUser["logo"] = m_vRows[i]->vUsers[u]->logo;
      jsonUser["university"] = m_vRows[i]->vUsers[u]->university;
      jsonUser["rating"] = m_vRows[i]->rating;
      jsonUsers.push_back(jsonUser);
    }
    jsonRow["users"] = jsonUsers;
    jsonScoreboard.push_back(jsonRow);
  }
  return jsonScoreboard;
}

// ---------------------------------------------------------------------

int EmployScoreboard::count() { return m_vRows.size(); }

// ---------------------------------------------------------------------

void EmployScoreboard::asyncUpdatedQuestScore(int /*nQuestID*/) {
  // TODO
  // When quest score changed
}

// ---------------------------------------------------------------------

void EmployScoreboard::asyncUpdatedUserRating(int /*nUserID*/) {
  // TODO
  // When user score changed
}

// ---------------------------------------------------------------------

void EmployScoreboard::asyncUpdatedLeaksScore(int /*nLeakID*/) {
  // TODO
  // When leak score changed
}
