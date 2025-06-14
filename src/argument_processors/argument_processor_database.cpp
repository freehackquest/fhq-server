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

#include "argument_processor_database.h"
#include <employ_database.h>
#include <employees.h>
#include <unistd.h> // getpass
#include <wsjcpp_core.h>
#include <wsjcpp_storages.h>

// ---------------------------------------------------------------------
// ArgumentProcessorDatabase

ArgumentProcessorDatabase::ArgumentProcessorDatabase()
  : WsjcppArgumentProcessor({"database", "db"}, "Database", "All what depened to database processing") {
  TAG = "ArgumentProcessorDatabase";
  registryProcessor(new ArgumentProcessorDatabaseTestConnection());
  registryProcessor(new ArgumentProcessorDatabaseStruct());
  registryProcessor(new ArgumentProcessorDatabaseCreate());
}

int ArgumentProcessorDatabase::exec(
  const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams
) {
  WsjcppLog::err(TAG, "Not implemented");
  return -10;
}

// ---------------------------------------------------------------------
// ArgumentProcessorApi

ArgumentProcessorDatabaseTestConnection::ArgumentProcessorDatabaseTestConnection()
  : WsjcppArgumentProcessor({"test-connection", "test"}, "Test connection to database", "Test connection to database") {
  TAG = "ArgumentProcessorDatabaseTestConnection";
}

int ArgumentProcessorDatabaseTestConnection::exec(
  const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams
) {
  std::cout << "\n * Check Database Connection\n\n";
  if (!WsjcppEmployees::init({})) {
    WsjcppLog::err(TAG, "Could not init database module");
    return -1;
  }
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase *db = pDatabase->database();
  if (!db->open()) {
    WsjcppLog::err(TAG, "Could not connect to database, please check config");
    return -1;
  }
  std::cout << "\n * Success\n\n";
  return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorDatabaseStruct

class FakeVersion {
public:
  FakeVersion() { m_sLastVersion = ""; };

  std::string getVersion() { return m_sLastVersion; };

  void setVersion(std::string sLastVersion) { m_sLastVersion = sLastVersion; };

private:
  std::string m_sLastVersion;
};

class FakeStorageConnection : public WsjcppStorageConnection {
public:
  FakeStorageConnection(FakeVersion *pVersion) { m_pVersion = pVersion; };
  virtual bool executeQuery(const std::string &sQuery) { return true; };
  virtual std::string lastDatabaseVersion() { return m_pVersion->getVersion(); };
  virtual std::vector<std::string> getInstalledVersions() override {
    std::vector<std::string> vVersions;
    return vVersions;
  }
  virtual bool insertUpdateInfo(const std::string &sVersion, const std::string &sDescription) {
    m_pVersion->setVersion(sVersion);
    return true;
  };

private:
  FakeVersion *m_pVersion;
};

class FakeStorage : public WsjcppStorage {
public:
  FakeStorage() { m_pVersion = new FakeVersion(); };
  ~FakeStorage() { delete m_pVersion; };
  virtual bool applyConfigFromFile(const std::string &sFilePath) { return true; };
  virtual WsjcppStorageConnection *connect() { return new FakeStorageConnection(m_pVersion); };
  virtual void clean() {
    // nothing
  };
  virtual std::string prepareStringValue(const std::string &sValue) { return sValue; };
  virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageInsert &storageInsert) {
    return std::vector<std::string>();
  };
  virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageCreateTable &storageCreateTable) {
    return std::vector<std::string>();
  };
  virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageModifyTable &storageModifyTable) {
    return std::vector<std::string>();
  };
  virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageDropTable &storageDropTable) {
    return std::vector<std::string>();
  };

private:
  FakeVersion *m_pVersion;
};

ArgumentProcessorDatabaseStruct::ArgumentProcessorDatabaseStruct()
  : WsjcppArgumentProcessor({"struct"}, "Print database structure", "Print database structure") {
  TAG = "ArgumentProcessorDatabaseStruct";
}

int ArgumentProcessorDatabaseStruct::exec(
  const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams
) {
  FakeStorage *pStorage = new FakeStorage();
  WsjcppStorageUpdates::apply2(pStorage);

  std::cout << std::endl << std::endl << " * Tables: " << std::endl;
  std::map<std::string, WsjcppStorageTable> tables = pStorage->getTables();
  std::map<std::string, WsjcppStorageTable>::iterator it = tables.begin();
  while (it != tables.end()) {
    std::string sTableName = it->first;
    WsjcppStorageTable st = it->second;
    std::cout << " + Table '" << st.getTableName() << "' " << std::endl;
    std::cout << " |---* Columns: " << std::endl;
    std::vector<WsjcppStorageColumnDef> vColumns = st.getColumns();
    for (int i = 0; i < vColumns.size(); i++) {
      WsjcppStorageColumnDef c = vColumns[i];
      std::cout << " |   | - '" << c.columnName() << "' as " << c.columnType() << " (" << c.columnTypeSize()
                << ") "
                // TODO c.columnDescription()
                << std::endl;
    }
    std::cout << " | " << std::endl;
    it++;
  }
  return 0;
}

// ---------------------------------------------------------------------
// ArgumentProcessorDatabaseCreate

ArgumentProcessorDatabaseCreate::ArgumentProcessorDatabaseCreate()
  : WsjcppArgumentProcessor(
      {"create"}, "Manual create empty database (mysql)", "Manual create empty database (mysql)"
    ) {
  TAG = "ArgumentProcessorDatabaseCreate";
}

int ArgumentProcessorDatabaseCreate::exec(
  const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams
) {
  auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  std::cout << "\n * Manual create database\n\n";
  if (!pGlobalSettings->init()) {
    std::cout << "\n * Failed on init server config\n\n";
    return -1;
  }
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  // enter mysql root password
  char *pPassword = getpass("Enter MySQL root password: ");
  std::string sRootPassword(pPassword);
  std::string sError;
  if (!pDatabase->manualCreateDatabase(sRootPassword, sError)) {
    std::cout << "\n * Failed: " << sError << "\n\n";
    return -1;
  }

  // init database
  if (!pDatabase->init()) {
    std::cout << "\n * Failed on init database structure\n\n";
    return -1;
  }

  std::cout << "\n * Done\n\n";
  return 0;
}