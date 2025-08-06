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

#pragma once

#include <employees.h>
#include <model_database_connection.h>

#include <QMap>
#include <fhq_server_db_email_delivery.h>
#include <fhq_server_db_useful_links.h>
#include <fhq_server_db_uuids.h>
#include <mutex>
#include <wsjcpp_storages.h>

class EmployDatabase : public WsjcppEmployBase, public WsjcppSettingsStore {
public:
  EmployDatabase();
  static std::string name() { return "EmployDatabase"; }
  virtual bool init();
  virtual bool deinit();

  FhqServerDbUuids *databaseUuids();
  FhqServerDbUsefulLinks *databaseUsefulLinks();

  QSqlDatabase *database();
  bool manualCreateDatabase(const std::string &sRootPassword, std::string &sError);
  WsjcppStorageConnection *getStorageConnection();

  // WsjcppSettingsStore
  virtual std::map<std::string, std::string> loadAllSettings();
  virtual void updateSettingItem(const WsjcppSettingItem *pSettingItem);
  virtual void initSettingItem(WsjcppSettingItem *pSettingItem);

private:
  std::string TAG;
  std::string m_sStorageType;

  bool initUuidsDatabase();
  bool initUsefulLinksDatabase();

  bool initEmailDelivery();
  int m_nConnectionOutdatedAfterSeconds;
  WsjcppStorage *m_pStorage;

  // new new
  std::map<std::string, WsjcppStorageConnection *> m_mapStorageConnections;
  std::vector<WsjcppStorageConnection *> m_vDoRemoveStorageConnections;
  std::mutex m_mtxStorageConnections;

  // db two connections
  std::mutex m_mtxSwapConenctions;

  // old
  QMap<long long, ModelDatabaseConnection *> m_mDatabaseConnections;
  QMap<long long, ModelDatabaseConnection *> m_mDatabaseConnections_older;
  ModelDatabaseConnection *m_pDBConnection;
  ModelDatabaseConnection *m_pDBConnection_older;

  // sqlite3 database files
  FhqServerDbUuids *m_pUuids;
  FhqServerDbUsefulLinks *m_pUsefulLinks;
  FhqServerDbEmailDelivery *m_pEmailDelivery;
};
