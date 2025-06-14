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

#include <map>
#include <mutex>
#include <mysql/mysql.h>
#include <wsjcpp_storages.h>

class MySqlStorageConnection : public WsjcppStorageConnection {
public:
  MySqlStorageConnection(MYSQL *pConn, WsjcppStorage *pStorage);
  virtual ~MySqlStorageConnection();
  virtual bool executeQuery(const std::string &sQuery);
  virtual std::string lastDatabaseVersion();
  virtual std::vector<std::string> getInstalledVersions() override;
  virtual bool insertUpdateInfo(const std::string &sVersion, const std::string &sDescription);

private:
  MYSQL *m_pConnection;
  std::mutex m_mtxConn;
  WsjcppStorage *m_pStorage;
};

class MySqlStorage : public WsjcppStorage {
public:
  MySqlStorage();
  static std::string type() { return "mysql"; };
  virtual bool applyConfigFromFile(const std::string &sFilePath);
  virtual WsjcppStorageConnection *connect();
  virtual void clean();
  virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageInsert &storageInsert);
  virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageCreateTable &storageCreateTable);
  virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageModifyTable &storageModifyTable);
  virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageDropTable &storageDropTable);
  virtual std::string prepareStringValue(const std::string &sValue);

private:
  std::string generateLineColumnForSql(WsjcppStorageColumnDef &c);
  std::string TAG;
  std::string m_sDatabaseHost;
  std::string m_sDatabaseName;
  std::string m_sDatabaseUser;
  std::string m_sDatabasePass;
  int m_nDatabasePort;
};
