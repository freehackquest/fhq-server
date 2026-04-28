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

#include "fhq_server_db_public_events.h"

#include <employ_database.h>
#include <employees.h>

// ---------------------------------------------------------------------
// FhqServerDbPublicEventsUpdates

class FhqServerDbPublicEventsUpdate_000_001 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbPublicEventsUpdate_000_001() : FhqServerDatabaseFileUpdate("", "v001", "Init table public_events") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    // IF NOT EXISTS
    return pDatabaseFile->executeQuery("CREATE TABLE public_events ( "
                                       "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "  uuid VARCHAR(36) NOT NULL,"
                                       "  type VARCHAR(255) NOT NULL,"
                                       "  dt INTEGER NOT NULL,"
                                       "  message VARCHAR(2048) NOT NULL,"
                                       "  meta VARCHAR(4096) NOT NULL"
                                       ");");
  }
};

class FhqServerDbPublicEventsUpdate_001_002 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbPublicEventsUpdate_001_002() : FhqServerDatabaseFileUpdate("v001", "v002", "Create uniq index") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    return pDatabaseFile->executeQuery("CREATE UNIQUE INDEX IF NOT EXISTS uuids_col_uuid ON public_events (uuid)");
  }
};

// ---------------------------------------------------------------------
// FhqServerDbPublicEvents

FhqServerDbPublicEvents::FhqServerDbPublicEvents() : FhqServerDatabaseFile("public_events.db") {
  TAG = "FhqServerDbPublicEvents";
  m_vDbUpdates.push_back(new FhqServerDbPublicEventsUpdate_000_001());
  m_vDbUpdates.push_back(new FhqServerDbPublicEventsUpdate_001_002());
};

FhqServerDbPublicEvents::~FhqServerDbPublicEvents() {}

// std::map<std::string, std::string> FhqServerDbPublicEvents::getAllRecords() {
//   std::lock_guard<std::mutex> lock(m_mutex);

//   std::map<std::string, std::string> mapUuids;
//   std::string sSql = "SELECT uuid, typeobj FROM uuids;";
//   FhqServerDatabaseSelectRows cur;
//   if (this->selectRows(sSql, cur)) {
//     while (cur.next()) {
//       mapUuids[cur.getString(0)] = cur.getString(1);
//     }
//   }
//   return mapUuids;
// }

// bool FhqServerDbPublicEvents::insertUuid(const std::string &sUuid, const std::string &sTypeOfObject) {
//   std::lock_guard<std::mutex> lock(m_mutex);

//   FhqServerDatabaseSqlQueryInsert sql("uuids");
//   sql.add("uuid", sUuid);
//   sql.add("typeobj", sTypeOfObject);
//   sql.add("dt", WsjcppCore::getCurrentTimeInMilliseconds());

//   if (!this->executeQuery(sql.getSql())) {
//     WsjcppLog::err(TAG, "Could not insert " + sql.getSql());
//     return false;
//   }
//   return true;
// }
