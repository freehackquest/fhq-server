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

#include <fhq/employees/employ_public_events.h>

namespace fhq {

// ---------------------------------------------------------------------
// DbPublicEventsUpdates

class DbPublicEventsUpdate_000_001 : public FhqServerDatabaseFileUpdate {
public:
  DbPublicEventsUpdate_000_001() : FhqServerDatabaseFileUpdate("", "v001", "Init table public_events") {}
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

class DbPublicEventsUpdate_001_002 : public FhqServerDatabaseFileUpdate {
public:
  DbPublicEventsUpdate_001_002() : FhqServerDatabaseFileUpdate("v001", "v002", "Create uniq index") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    return pDatabaseFile->executeQuery("CREATE UNIQUE INDEX IF NOT EXISTS uuids_col_uuid ON public_events (uuid)");
  }
};

// ---------------------------------------------------------------------
// DbPublicEvents

DbPublicEvents::DbPublicEvents() : FhqServerDatabaseFile("public_events.db") {
  TAG = "DbPublicEvents";
  m_vDbUpdates.push_back(new DbPublicEventsUpdate_000_001());
  m_vDbUpdates.push_back(new DbPublicEventsUpdate_001_002());
};

DbPublicEvents::~DbPublicEvents() {}

bool DbPublicEvents::deleteRecord(const std::string &uuid, std::string &errorMessage) {
  std::lock_guard<std::mutex> lock(m_mutex);

  bool bFound = false;

  std::string sql = "SELECT uuid FROM public_events;";
  FhqServerDatabaseSelectRows cur;
  if (this->selectRows(sql, cur)) {
    while (cur.next()) {
      bFound = true;
    }
  }
  if (!bFound) {
    errorMessage = "NOT_FOUND";
    return bFound;
  }
  sql = "DELETE FROM public_events uuid = '" + uuid + "';";
  if (!this->executeQuery(sql)) {
    errorMessage = "Could not delete record from public_events by uuid '" + uuid + "'";
    WsjcppLog::err(TAG, errorMessage);
    return false;
  }
  return true;
}

} // namespace fhq
