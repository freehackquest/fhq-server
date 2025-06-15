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

#include "fhq_server_db_useful_links.h"

#include <employ_database.h>
#include <employees.h>
// #include <algorithm>

// ---------------------------------------------------------------------
// FhqServerDbUsefulLinksUpdates

class FhqServerDbUsefulLinksUpdate_000_001 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_000_001() : FhqServerDatabaseFileUpdate("", "v001", "Init table useful_links") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    // IF NOT EXISTS
    return pDatabaseFile->executeQuery("CREATE TABLE useful_links ( "
                                       "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "  url VARCHAR(2048) NOT NULL,"
                                       "  description VARCHAR(2048) NOT NULL,"
                                       "  author VARCHAR(127) NOT NULL,"
                                       "  dt INTEGER NOT NULL,"
                                       "  user_favorites INTEGER NOT NULL,"
                                       "  user_clicks INTEGER NOT NULL,"
                                       "  user_comments INTEGER NOT NULL,"
                                       "  tags VARCHAR(2048) NOT NULL"
                                       ");");
  }
};

class FhqServerDbUsefulLinksUpdate_001_002 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_001_002()
    : FhqServerDatabaseFileUpdate("v001", "v002", "Init table useful_links_comments") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    // IF NOT EXISTS
    return pDatabaseFile->executeQuery("CREATE TABLE useful_links_comments ( "
                                       "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "  usefullinkuuid VARCHAR(36) NOT NULL,"
                                       "  usefullinkid INTEGER NOT NULL,"
                                       "  useruuid VARCHAR(36) NOT NULL,"
                                       "  userid INTEGER NOT NULL,"
                                       "  comment VARCHAR(2048) NOT NULL,"
                                       "  dt INTEGER NOT NULL"
                                       ");");
  }
};

class FhqServerDbUsefulLinksUpdate_002_003 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_002_003()
    : FhqServerDatabaseFileUpdate("v002", "v003", "Init table useful_links_tags") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    // IF NOT EXISTS
    return pDatabaseFile->executeQuery("CREATE TABLE useful_links_tags ( "
                                       "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "  usefullinkuuid VARCHAR(36) NOT NULL,"
                                       "  usefullinkid INTEGER NOT NULL,"
                                       "  tagvalue VARCHAR(36) NOT NULL"
                                       ");");
  }
};

class FhqServerDbUsefulLinksUpdate_003_004 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_003_004()
    : FhqServerDatabaseFileUpdate("v003", "v004", "Init table useful_links_user_favorites") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    // IF NOT EXISTS
    return pDatabaseFile->executeQuery("CREATE TABLE useful_links_user_favorites ( "
                                       "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "  usefullinkuuid VARCHAR(36) NOT NULL,"
                                       "  usefullinkid INTEGER NOT NULL,"
                                       "  useruuid VARCHAR(36) NOT NULL,"
                                       "  userid INTEGER NOT NULL,"
                                       "  dt INTEGER NOT NULL"
                                       ");");
  }
};

class FhqServerDbUsefulLinksUpdate_004_005 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_004_005() : FhqServerDatabaseFileUpdate("v004", "v005", "Alter table useful_links") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    return pDatabaseFile->executeQuery("ALTER TABLE useful_links ADD COLUMN "
                                       "  uuid VARCHAR(36) NOT NULL;");
  }
};

class FhqServerDbUsefulLinksUpdate_005_006 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_005_006()
    : FhqServerDatabaseFileUpdate("v005", "v006", "Add unique index for useful_links uuid") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    return pDatabaseFile->executeQuery("CREATE UNIQUE INDEX IF NOT EXISTS useful_links_col_uuid ON useful_links (uuid)"
    );
  }
};

class FhqServerDbUsefulLinksUpdate_006_007 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_006_007()
    : FhqServerDatabaseFileUpdate("v006", "v007", "Add unique index for useful_links url") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    return pDatabaseFile->executeQuery("CREATE UNIQUE INDEX IF NOT EXISTS useful_links_col_url ON useful_links (url)");
  }
};

class FhqServerDbUsefulLinksUpdate_007_008 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_007_008()
    : FhqServerDatabaseFileUpdate("v007", "v008", "Migrating data from mysql to sqlite") {
    TAG = "FhqServerDbUsefulLinksUpdate_007_008";
  }
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());

    std::vector<std::string> vTables;
    vTables.push_back("useful_links");
    vTables.push_back("useful_links_comments");
    vTables.push_back("useful_links_tags");
    vTables.push_back("useful_links_user_favorites");

    // check existing table in mysql
    int nTablesFound = 0;
    {
      QSqlQuery query(db);
      query.prepare("SHOW TABLES LIKE 'useful_links%';");
      if (!query.exec()) {
        WsjcppLog::throw_err(
          TAG, "FhqServerDbUsefulLinksUpdate_005_006::show table => " + query.lastError().text().toStdString()
        );
        return false;
      }
      while (query.next()) {
        QSqlRecord record = query.record();
        std::string sTableName = record.value(0).toString().toStdString();

        if (std::find(vTables.begin(), vTables.end(), sTableName) != vTables.end()) {
          nTablesFound++;
        }
      }
    }

    if (nTablesFound != 4) {
      WsjcppLog::info(TAG, "Skip migration - missing tables in mysql. ");
      return true;
    }

    // load data from useful_links
    std::map<int, std::string> vUsersIdsUuids;
    {
      QSqlQuery query(db);
      query.prepare("SELECT * FROM users");
      if (!query.exec()) {
        WsjcppLog::throw_err(TAG, " SQL-Error: " + query.lastError().text().toStdString());
        return false;
      };

      int nUsersCount = 0;
      while (query.next()) {
        nUsersCount++;
        QSqlRecord record = query.record();
        int nId = record.value("id").toInt();
        std::string sUuid = record.value("uuid").toString().toStdString();
        vUsersIdsUuids[nId] = sUuid;
      }
      WsjcppLog::info(TAG, "Found records in 'users' => " + std::to_string(nUsersCount));
    }

    // load data from useful_links
    std::vector<ModelUsefulLink> vUsefulLinks;
    std::map<int, std::string> vUsefulLinksIdsUuids;
    std::vector<std::string> vUniqueUrls;
    {
      QSqlQuery query(db);
      query.prepare("SELECT * FROM useful_links");
      if (!query.exec()) {
        WsjcppLog::throw_err(TAG, " SQL-Error: " + query.lastError().text().toStdString());
        return false;
      };

      while (query.next()) {
        ModelUsefulLink m;
        QSqlRecord record = query.record();
        int id = record.value("id").toInt();
        m.setLocalId(id);
        std::string sUuid = WsjcppCore::createUuid();
        m.setUuid(sUuid);
        std::map<int, std::string>::iterator it;
        for (it = vUsefulLinksIdsUuids.begin(); it != vUsefulLinksIdsUuids.end(); it++) {
          if (it->second == sUuid) {
            WsjcppLog::throw_err(TAG, "Uuid alredy exists " + sUuid);
            return false;
          }
        }
        std::string sUrl = record.value("url").toString().toStdString();
        if (std::find(vUniqueUrls.begin(), vUniqueUrls.end(), sUrl) != vUniqueUrls.end()) {
          WsjcppLog::info(TAG, "Skipped. Found duplicate url " + sUrl);
          continue;
        }
        vUniqueUrls.push_back(sUrl);
        m.setUrl(sUrl);
        m.setDescription(record.value("description").toString().toStdString());
        m.setAuthor(record.value("author").toString().toStdString());
        // QString sdt = record.value("dt").toString();
        // QDateTime dt = QDateTime::fromString(sdt, "yyyy-MM-ddThh:mm:ss.zzz");
        // long ndt = dt.toMSecsSinceEpoch();
        // 1749969671000
        // std::cout << "for " << id << ", sdt " << sdt.toStdString() << ", ndt " << ndt << ", dt.toString = " <<
        // dt.toString("yyyy-mm-dd hh:mm:ss").toStdString() << std::endl;
        m.setDateTimeUpdated(
          QDateTime::fromString(record.value("dt").toString(), "yyyy-MM-ddThh:mm:ss.zzz").toMSecsSinceEpoch()
        );
        m.setUserFavorites(record.value("user_favorites").toInt());
        m.setUserClicks(record.value("user_clicks").toInt());
        m.setUserComments(record.value("user_comments").toInt());
        m.setTags(record.value("tags").toString().toStdString());
        vUsefulLinks.push_back(m);

        vUsefulLinksIdsUuids[id] = sUuid;
      }
      WsjcppLog::info(TAG, "Found records in 'useful_links' => " + std::to_string(vUsefulLinks.size()));
    }

    std::vector<ModelUsefulLinkComment> vUsefulLinksComments;
    {
      QSqlQuery query(db);
      query.prepare("SELECT * FROM useful_links_comments");
      if (!query.exec()) {
        WsjcppLog::throw_err(TAG, " SQL-Error: " + query.lastError().text().toStdString());
        return false;
      };

      while (query.next()) {
        ModelUsefulLinkComment m;
        QSqlRecord record = query.record();
        m.setLocalId(record.value("id").toInt());
        int usefullinkid = record.value("usefullinkid").toInt();
        m.setUsefulLinkId(usefullinkid);
        // std::cout << "for " << usefullinkid << ", uuid" << vUsefulLinksIdsUuids[usefullinkid] << std::endl;
        if (vUsefulLinksIdsUuids.count(usefullinkid) == 0) {
          continue;
        }
        m.setUsefulLinkUuid(vUsefulLinksIdsUuids[usefullinkid]);
        int nUserId = record.value("userid").toInt();
        m.setUserId(nUserId);
        m.setUserUuid(vUsersIdsUuids[nUserId]);
        m.setComment(record.value("comment").toString().toStdString());
        m.setDateTime(
          QDateTime::fromString(record.value("dt").toString(), "yyyy-MM-ddThh:mm:ss.zzz").toMSecsSinceEpoch()
        );
        vUsefulLinksComments.push_back(m);
      }
      WsjcppLog::info(TAG, "Found records in 'useful_link_comments' => " + std::to_string(vUsefulLinksComments.size()));
    }

    std::vector<ModelUsefulLinkTag> vUsefulLinksTags;
    {
      QSqlQuery query(db);
      query.prepare("SELECT * FROM useful_links_tags");
      if (!query.exec()) {
        WsjcppLog::throw_err(TAG, " SQL-Error: " + query.lastError().text().toStdString());
        return false;
      };

      while (query.next()) {
        ModelUsefulLinkTag m;
        QSqlRecord record = query.record();
        m.setLocalId(record.value("id").toInt());
        int usefullinkid = record.value("usefullinkid").toInt();
        m.setUsefulLinkId(usefullinkid);
        if (vUsefulLinksIdsUuids.count(usefullinkid) == 0) {
          continue;
        }
        m.setUsefulLinkUuid(vUsefulLinksIdsUuids[usefullinkid]);
        m.setTagValue(record.value("tagvalue").toString().toStdString());
        vUsefulLinksTags.push_back(m);
      }
      WsjcppLog::info(TAG, "Found records in 'useful_link_tags' => " + std::to_string(vUsefulLinksTags.size()));
    }

    std::vector<ModelUsefulLinkUserFavorite> vUsefulLinksUserFavotites;
    {
      QSqlQuery query(db);
      query.prepare("SELECT * FROM useful_links_user_favorites");
      if (!query.exec()) {
        WsjcppLog::throw_err(TAG, " SQL-Error: " + query.lastError().text().toStdString());
        return false;
      };

      while (query.next()) {
        ModelUsefulLinkUserFavorite m;
        QSqlRecord record = query.record();
        m.setLocalId(record.value("id").toInt());
        int usefullinkid = record.value("usefullinkid").toInt();
        if (vUsefulLinksIdsUuids.count(usefullinkid) == 0) {
          continue;
        }
        m.setUsefulLinkId(usefullinkid);
        m.setUsefulLinkUuid(vUsefulLinksIdsUuids[usefullinkid]);
        int nUserId = record.value("userid").toInt();
        m.setUserId(nUserId);
        m.setUserUuid(vUsersIdsUuids[nUserId]);
        m.setDateTime(
          QDateTime::fromString(record.value("dt").toString(), "yyyy-MM-ddThh:mm:ss.zzz").toMSecsSinceEpoch()
        );
        vUsefulLinksUserFavotites.push_back(m);
      }
      WsjcppLog::info(
        TAG, "Found records in 'useful_links_user_favorites' => " + std::to_string(vUsefulLinksUserFavotites.size())
      );
    }

    WsjcppLog::info(TAG, "Cleanup tables");
    for (int i = 0; i < vTables.size(); i++) {
      if (!pDatabaseFile->executeQuery("DELETE FROM " + vTables[i] + ";")) {
        WsjcppLog::throw_err(TAG, "Could not cleanup " + vTables[i]);
        return false;
      }
    }

    for (int i = 0; i < vUsefulLinks.size(); i++) {
      ModelUsefulLink m = vUsefulLinks[i];
      FhqServerDatabaseSqlQueryInsert sql("useful_links");
      sql.add("uuid", m.getUuid());
      sql.add("url", m.getUrl());
      sql.add("description", m.getDescription());
      sql.add("author", m.getAuthor());
      sql.add("dt", m.getDateTimeUpdated());
      sql.add("user_favorites", m.getUserFavorites());
      sql.add("user_clicks", m.getUserClicks());
      sql.add("user_comments", m.getUserComments());
      sql.add("tags", m.getTags());

      if (!pDatabaseFile->executeQuery(sql.getSql())) {
        WsjcppLog::throw_err(TAG, "Could not insert " + sql.getSql());
        return false;
      }
    }

    for (int i = 0; i < vUsefulLinksComments.size(); i++) {
      ModelUsefulLinkComment m = vUsefulLinksComments[i];
      FhqServerDatabaseSqlQueryInsert sql("useful_links_comments");
      sql.add("usefullinkid", m.getUsefulLinkId());
      sql.add("usefullinkuuid", m.getUsefulLinkUuid());
      sql.add("userid", m.getUserId());
      sql.add("useruuid", m.getUserUuid());
      sql.add("comment", m.getComment());
      sql.add("dt", m.getDateTime());

      if (!pDatabaseFile->executeQuery(sql.getSql())) {
        WsjcppLog::throw_err(TAG, "Could not insert " + sql.getSql());
        return false;
      }
    }

    for (int i = 0; i < vUsefulLinksTags.size(); i++) {
      ModelUsefulLinkTag m = vUsefulLinksTags[i];
      FhqServerDatabaseSqlQueryInsert sql("useful_links_tags");
      sql.add("usefullinkid", m.getUsefulLinkId());
      sql.add("usefullinkuuid", m.getUsefulLinkUuid());
      sql.add("tagvalue", m.getTagValue());

      if (!pDatabaseFile->executeQuery(sql.getSql())) {
        WsjcppLog::throw_err(TAG, "Could not insert " + sql.getSql());
        return false;
      }
    }

    for (int i = 0; i < vUsefulLinksUserFavotites.size(); i++) {
      ModelUsefulLinkUserFavorite m = vUsefulLinksUserFavotites[i];
      FhqServerDatabaseSqlQueryInsert sql("useful_links_user_favorites");
      sql.add("usefullinkid", m.getUsefulLinkId());
      sql.add("usefullinkuuid", m.getUsefulLinkUuid());
      sql.add("userid", m.getUserId());
      sql.add("useruuid", m.getUserUuid());
      sql.add("dt", m.getDateTime());

      if (!pDatabaseFile->executeQuery(sql.getSql())) {
        WsjcppLog::throw_err(TAG, "Could not insert " + sql.getSql());
        return false;
      }
    }

    return true;
  }
};

// ---------------------------------------------------------------------
// FhqServerDbUsefulLinksUpdates

class FhqServerDbUsefulLinksUpdate_000_001 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_000_001() : FhqServerDatabaseFileUpdate("", "v001", "Init table useful_links") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    // IF NOT EXISTS
    return pDatabaseFile->executeQuery("CREATE TABLE useful_links ( "
                                       "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "  url VARCHAR(2048) NOT NULL,"
                                       "  description VARCHAR(2048) NOT NULL,"
                                       "  author VARCHAR(127) NOT NULL,"
                                       "  dt INTEGER NOT NULL,"
                                       "  user_favorites INTEGER NOT NULL,"
                                       "  user_clicks INTEGER NOT NULL,"
                                       "  user_comments INTEGER NOT NULL,"
                                       "  tags VARCHAR(2048) NOT NULL"
                                       ");");
  }
};

class FhqServerDbUsefulLinksUpdate_001_002 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_001_002()
    : FhqServerDatabaseFileUpdate("v001", "v002", "Init table useful_links_comments") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    // IF NOT EXISTS
    return pDatabaseFile->executeQuery("CREATE TABLE useful_links_comments ( "
                                       "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "  usefullinkuuid VARCHAR(36) NOT NULL,"
                                       "  usefullinkid INTEGER NOT NULL,"
                                       "  useruuid VARCHAR(36) NOT NULL,"
                                       "  userid INTEGER NOT NULL,"
                                       "  comment VARCHAR(2048) NOT NULL,"
                                       "  dt INTEGER NOT NULL"
                                       ");");
  }
};

class FhqServerDbUsefulLinksUpdate_002_003 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_002_003()
    : FhqServerDatabaseFileUpdate("v002", "v003", "Init table useful_links_tags") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    // IF NOT EXISTS
    return pDatabaseFile->executeQuery("CREATE TABLE useful_links_tags ( "
                                       "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "  usefullinkuuid VARCHAR(36) NOT NULL,"
                                       "  usefullinkid INTEGER NOT NULL,"
                                       "  tagvalue VARCHAR(36) NOT NULL"
                                       ");");
  }
};

class FhqServerDbUsefulLinksUpdate_003_004 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_003_004()
    : FhqServerDatabaseFileUpdate("v003", "v004", "Init table useful_links_user_favorites") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    // IF NOT EXISTS
    return pDatabaseFile->executeQuery("CREATE TABLE useful_links_user_favorites ( "
                                       "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                       "  usefullinkuuid VARCHAR(36) NOT NULL,"
                                       "  usefullinkid INTEGER NOT NULL,"
                                       "  useruuid VARCHAR(36) NOT NULL,"
                                       "  userid INTEGER NOT NULL,"
                                       "  dt INTEGER NOT NULL"
                                       ");");
  }
};

class FhqServerDbUsefulLinksUpdate_004_005 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_004_005() : FhqServerDatabaseFileUpdate("v004", "v005", "Alter table useful_links") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    return pDatabaseFile->executeQuery("ALTER TABLE useful_links ADD COLUMN "
                                       "  uuid VARCHAR(36) NOT NULL;");
  }
};

class FhqServerDbUsefulLinksUpdate_005_006 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_005_006()
    : FhqServerDatabaseFileUpdate("v005", "v006", "Add unique index for useful_links uuid") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    return pDatabaseFile->executeQuery("CREATE UNIQUE INDEX IF NOT EXISTS useful_links_col_uuid ON useful_links (uuid)"
    );
  }
};

class FhqServerDbUsefulLinksUpdate_006_007 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_006_007()
    : FhqServerDatabaseFileUpdate("v006", "v007", "Add unique index for useful_links url") {}
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {
    return pDatabaseFile->executeQuery("CREATE UNIQUE INDEX IF NOT EXISTS useful_links_col_url ON useful_links (url)");
  }
};

class FhqServerDbUsefulLinksUpdate_007_008 : public FhqServerDatabaseFileUpdate {
public:
  FhqServerDbUsefulLinksUpdate_007_008()
    : FhqServerDatabaseFileUpdate("v007", "v008", "Migrating data from mysql to sqlite") {
    TAG = "FhqServerDbUsefulLinksUpdate_007_008";
  }
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) override {

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());

    std::vector<std::string> vTables;
    vTables.push_back("useful_links");
    vTables.push_back("useful_links_comments");
    vTables.push_back("useful_links_tags");
    vTables.push_back("useful_links_user_favorites");

    // check existing table in mysql
    int nTablesFound = 0;
    {
      QSqlQuery query(db);
      query.prepare("SHOW TABLES LIKE 'useful_links%';");
      if (!query.exec()) {
        WsjcppLog::throw_err(
          TAG, "FhqServerDbUsefulLinksUpdate_005_006::show table => " + query.lastError().text().toStdString()
        );
        return false;
      }
      while (query.next()) {
        QSqlRecord record = query.record();
        std::string sTableName = record.value(0).toString().toStdString();

        if (std::find(vTables.begin(), vTables.end(), sTableName) != vTables.end()) {
          nTablesFound++;
        }
      }
    }

    if (nTablesFound != 4) {
      WsjcppLog::info(TAG, "Skip migration - missing tables in mysql. ");
      return true;
    }

    // load data from useful_links
    std::map<int, std::string> vUsersIdsUuids;
    {
      QSqlQuery query(db);
      query.prepare("SELECT * FROM users");
      if (!query.exec()) {
        WsjcppLog::throw_err(TAG, " SQL-Error: " + query.lastError().text().toStdString());
        return false;
      };

      int nUsersCount = 0;
      while (query.next()) {
        nUsersCount++;
        QSqlRecord record = query.record();
        int nId = record.value("id").toInt();
        std::string sUuid = record.value("uuid").toString().toStdString();
        vUsersIdsUuids[nId] = sUuid;
      }
      WsjcppLog::info(TAG, "Found records in 'users' => " + std::to_string(nUsersCount));
    }

    // load data from useful_links
    std::vector<ModelUsefulLink> vUsefulLinks;
    std::map<int, std::string> vUsefulLinksIdsUuids;
    std::vector<std::string> vUniqueUrls;
    {
      QSqlQuery query(db);
      query.prepare("SELECT * FROM useful_links");
      if (!query.exec()) {
        WsjcppLog::throw_err(TAG, " SQL-Error: " + query.lastError().text().toStdString());
        return false;
      };

      while (query.next()) {
        ModelUsefulLink m;
        QSqlRecord record = query.record();
        int id = record.value("id").toInt();
        m.setLocalId(id);
        std::string sUuid = WsjcppCore::createUuid();
        m.setUuid(sUuid);
        std::map<int, std::string>::iterator it;
        for (it = vUsefulLinksIdsUuids.begin(); it != vUsefulLinksIdsUuids.end(); it++) {
          if (it->second == sUuid) {
            WsjcppLog::throw_err(TAG, "Uuid alredy exists " + sUuid);
            return false;
          }
        }
        std::string sUrl = record.value("url").toString().toStdString();
        if (std::find(vUniqueUrls.begin(), vUniqueUrls.end(), sUrl) != vUniqueUrls.end()) {
          WsjcppLog::info(TAG, "Skipped. Found duplicate url " + sUrl);
          continue;
        }
        vUniqueUrls.push_back(sUrl);
        m.setUrl(sUrl);
        m.setDescription(record.value("description").toString().toStdString());
        m.setAuthor(record.value("author").toString().toStdString());
        // QString sdt = record.value("dt").toString();
        // QDateTime dt = QDateTime::fromString(sdt, "yyyy-MM-ddThh:mm:ss.zzz");
        // long ndt = dt.toMSecsSinceEpoch();
        // 1749969671000
        // std::cout << "for " << id << ", sdt " << sdt.toStdString() << ", ndt " << ndt << ", dt.toString = " << dt.toString("yyyy-mm-dd hh:mm:ss").toStdString() << std::endl;
        m.setDateTimeUpdated(QDateTime::fromString(record.value("dt").toString(), "yyyy-MM-ddThh:mm:ss.zzz").toMSecsSinceEpoch());
        m.setUserFavorites(record.value("user_favorites").toInt());
        m.setUserClicks(record.value("user_clicks").toInt());
        m.setUserComments(record.value("user_comments").toInt());
        m.setTags(record.value("tags").toString().toStdString());
        vUsefulLinks.push_back(m);

        vUsefulLinksIdsUuids[id] = sUuid;
      }
      WsjcppLog::info(TAG, "Found records in 'useful_links' => " + std::to_string(vUsefulLinks.size()));
    }

    std::vector<ModelUsefulLinkComment> vUsefulLinksComments;
    {
      QSqlQuery query(db);
      query.prepare("SELECT * FROM useful_links_comments");
      if (!query.exec()) {
        WsjcppLog::throw_err(TAG, " SQL-Error: " + query.lastError().text().toStdString());
        return false;
      };

      while (query.next()) {
        ModelUsefulLinkComment m;
        QSqlRecord record = query.record();
        m.setLocalId(record.value("id").toInt());
        int usefullinkid = record.value("usefullinkid").toInt();
        m.setUsefulLinkId(usefullinkid);
        // std::cout << "for " << usefullinkid << ", uuid" << vUsefulLinksIdsUuids[usefullinkid] << std::endl;
        if (vUsefulLinksIdsUuids.count(usefullinkid) == 0) {
          continue;
        }
        m.setUsefulLinkUuid(vUsefulLinksIdsUuids[usefullinkid]);
        int nUserId = record.value("userid").toInt();
        m.setUserId(nUserId);
        m.setUserUuid(vUsersIdsUuids[nUserId]);
        m.setComment(record.value("comment").toString().toStdString());
        m.setDateTime(
          QDateTime::fromString(record.value("dt").toString(), "yyyy-MM-ddThh:mm:ss.zzz").toMSecsSinceEpoch()
        );
        vUsefulLinksComments.push_back(m);
      }
      WsjcppLog::info(TAG, "Found records in 'useful_link_comments' => " + std::to_string(vUsefulLinksComments.size()));
    }

    std::vector<ModelUsefulLinkTag> vUsefulLinksTags;
    {
      QSqlQuery query(db);
      query.prepare("SELECT * FROM useful_links_tags");
      if (!query.exec()) {
        WsjcppLog::throw_err(TAG, " SQL-Error: " + query.lastError().text().toStdString());
        return false;
      };

      while (query.next()) {
        ModelUsefulLinkTag m;
        QSqlRecord record = query.record();
        m.setLocalId(record.value("id").toInt());
        int usefullinkid = record.value("usefullinkid").toInt();
        m.setUsefulLinkId(usefullinkid);
        if (vUsefulLinksIdsUuids.count(usefullinkid) == 0) {
          continue;
        }
        m.setUsefulLinkUuid(vUsefulLinksIdsUuids[usefullinkid]);
        m.setTagValue(record.value("tagvalue").toString().toStdString());
        vUsefulLinksTags.push_back(m);
      }
      WsjcppLog::info(TAG, "Found records in 'useful_link_tags' => " + std::to_string(vUsefulLinksTags.size()));
    }

    std::vector<ModelUsefulLinkUserFavorite> vUsefulLinksUserFavotites;
    {
      QSqlQuery query(db);
      query.prepare("SELECT * FROM useful_links_user_favorites");
      if (!query.exec()) {
        WsjcppLog::throw_err(TAG, " SQL-Error: " + query.lastError().text().toStdString());
        return false;
      };

      while (query.next()) {
        ModelUsefulLinkUserFavorite m;
        QSqlRecord record = query.record();
        m.setLocalId(record.value("id").toInt());
        int usefullinkid = record.value("usefullinkid").toInt();
        if (vUsefulLinksIdsUuids.count(usefullinkid) == 0) {
          continue;
        }
        m.setUsefulLinkId(usefullinkid);
        m.setUsefulLinkUuid(vUsefulLinksIdsUuids[usefullinkid]);
        int nUserId = record.value("userid").toInt();
        m.setUserId(nUserId);
        m.setUserUuid(vUsersIdsUuids[nUserId]);
        m.setDateTime(
          QDateTime::fromString(record.value("dt").toString(), "yyyy-MM-ddThh:mm:ss.zzz").toMSecsSinceEpoch()
        );
        vUsefulLinksUserFavotites.push_back(m);
      }
      WsjcppLog::info(
        TAG, "Found records in 'useful_links_user_favorites' => " + std::to_string(vUsefulLinksUserFavotites.size())
      );
    }

    WsjcppLog::info(TAG, "Cleanup tables");
    for (int i = 0; i < vTables.size(); i++) {
      if (!pDatabaseFile->executeQuery("DELETE FROM " + vTables[i] + ";")) {
        WsjcppLog::throw_err(TAG, "Could not cleanup " + vTables[i]);
        return false;
      }
    }

    for (int i = 0; i < vUsefulLinks.size(); i++) {
      ModelUsefulLink m = vUsefulLinks[i];
      FhqServerDatabaseSqlQueryInsert sql("useful_links");
      sql.add("uuid", m.getUuid());
      sql.add("url", m.getUrl());
      sql.add("description", m.getDescription());
      sql.add("author", m.getAuthor());
      sql.add("dt", m.getDateTimeUpdated());
      sql.add("user_favorites", m.getUserFavorites());
      sql.add("user_clicks", m.getUserClicks());
      sql.add("user_comments", m.getUserComments());
      sql.add("tags", m.getTags());

      if (!pDatabaseFile->executeQuery(sql.getSql())) {
        WsjcppLog::throw_err(TAG, "Could not insert " + sql.getSql());
        return false;
      }
    }

    for (int i = 0; i < vUsefulLinksComments.size(); i++) {
      ModelUsefulLinkComment m = vUsefulLinksComments[i];
      FhqServerDatabaseSqlQueryInsert sql("useful_links_comments");
      sql.add("usefullinkid", m.getUsefulLinkId());
      sql.add("usefullinkuuid", m.getUsefulLinkUuid());
      sql.add("userid", m.getUserId());
      sql.add("useruuid", m.getUserUuid());
      sql.add("comment", m.getComment());
      sql.add("dt", m.getDateTime());

      if (!pDatabaseFile->executeQuery(sql.getSql())) {
        WsjcppLog::throw_err(TAG, "Could not insert " + sql.getSql());
        return false;
      }
    }

    for (int i = 0; i < vUsefulLinksTags.size(); i++) {
      ModelUsefulLinkTag m = vUsefulLinksTags[i];
      FhqServerDatabaseSqlQueryInsert sql("useful_links_tags");
      sql.add("usefullinkid", m.getUsefulLinkId());
      sql.add("usefullinkuuid", m.getUsefulLinkUuid());
      sql.add("tagvalue", m.getTagValue());

      if (!pDatabaseFile->executeQuery(sql.getSql())) {
        WsjcppLog::throw_err(TAG, "Could not insert " + sql.getSql());
        return false;
      }
    }

    for (int i = 0; i < vUsefulLinksUserFavotites.size(); i++) {
      ModelUsefulLinkUserFavorite m = vUsefulLinksUserFavotites[i];
      FhqServerDatabaseSqlQueryInsert sql("useful_links_user_favorites");
      sql.add("usefullinkid", m.getUsefulLinkId());
      sql.add("usefullinkuuid", m.getUsefulLinkUuid());
      sql.add("userid", m.getUserId());
      sql.add("useruuid", m.getUserUuid());
      sql.add("dt", m.getDateTime());

      if (!pDatabaseFile->executeQuery(sql.getSql())) {
        WsjcppLog::throw_err(TAG, "Could not insert " + sql.getSql());
        return false;
      }
    }

    return false;
  }
};

// ---------------------------------------------------------------------
// FhqServerDbUsefulLinks

FhqServerDbUsefulLinks::FhqServerDbUsefulLinks() : FhqServerDatabaseFile("useful_links.db") {
  TAG = "FhqServerDbUsefulLinks";
  m_vDbUpdates.push_back(new FhqServerDbUsefulLinksUpdate_000_001());
  m_vDbUpdates.push_back(new FhqServerDbUsefulLinksUpdate_001_002());
  m_vDbUpdates.push_back(new FhqServerDbUsefulLinksUpdate_002_003());
  m_vDbUpdates.push_back(new FhqServerDbUsefulLinksUpdate_003_004());
  m_vDbUpdates.push_back(new FhqServerDbUsefulLinksUpdate_004_005());
  m_vDbUpdates.push_back(new FhqServerDbUsefulLinksUpdate_005_006());
  m_vDbUpdates.push_back(new FhqServerDbUsefulLinksUpdate_006_007());
  // m_vDbUpdates.push_back(new FhqServerDbUsefulLinksUpdate_007_008());
};

FhqServerDbUsefulLinks::~FhqServerDbUsefulLinks() {}

std::vector<ModelUsefulLink> FhqServerDbUsefulLinks::getAllRecords() {
  std::vector<ModelUsefulLink> records;
  FhqServerDatabaseSelectRows cur;
  std::string sSql =
    "SELECT uuid, url, description, author, dt, user_favorites, user_clicks, user_comments, tags FROM useful_links;";
  if (this->selectRows(sSql, cur)) {
    while (cur.next()) {
      ModelUsefulLink m;
      m.setUuid(cur.getString(0));
      m.setUrl(cur.getString(1));
      m.setDescription(cur.getString(2));
      m.setAuthor(cur.getString(3));
      m.setDateTimeUpdated(cur.getLong(4));
      m.setUserFavorites(cur.getLong(5));
      m.setUserClicks(cur.getLong(6));
      m.setUserComments(cur.getLong(7));
      m.setTags(cur.getString(8));
      records.push_back(m);
    }
  }
  return records;
}

bool FhqServerDbUsefulLinks::insertRecord(const ModelUsefulLink &m) {
  std::lock_guard<std::mutex> lock(m_mutex);

  FhqServerDatabaseSqlQueryInsert sql("useful_links");
  sql.add("uuid", m.getUuid());
  sql.add("url", m.getUrl());
  sql.add("description", m.getDescription());
  sql.add("author", m.getAuthor());
  sql.add("dt", m.getDateTimeUpdated());
  sql.add("user_favorites", m.getUserFavorites());
  sql.add("user_clicks", m.getUserClicks());
  sql.add("user_comments", m.getUserComments());
  sql.add("tags", m.getTags());

  // TODO safe execute
  if (!this->executeQuery(sql.getSql())) {
    WsjcppLog::err(TAG, "Could not insert " + sql.getSql());
    return false;
  }
  return true;
}

bool FhqServerDbUsefulLinks::deleteRecord(const std::string &sUuid) {
  std::lock_guard<std::mutex> lock(m_mutex);

  // TODO safe execute
  std::string sSQL = "DELETE FROM useful_links WHERE uuid = '" + sUuid + "';";
  if (!this->executeQuery(sSQL)) {
    WsjcppLog::err(TAG, "Could not delete " + sSQL);
    return false;
  }

  sSQL = "DELETE FROM useful_links_comments WHERE usefullinkuuid = '" + sUuid + "';";
  if (!this->executeQuery(sSQL)) {
    WsjcppLog::err(TAG, "Could not delete " + sSQL);
    return false;
  }

  sSQL = "DELETE FROM useful_links_user_favorites WHERE usefullinkuuid = '" + sUuid + "';";
  if (!this->executeQuery(sSQL)) {
    WsjcppLog::err(TAG, "Could not delete " + sSQL);
    return false;
  }

  sSQL = "DELETE FROM useful_links_tags WHERE usefullinkuuid = '" + sUuid + "';";
  if (!this->executeQuery(sSQL)) {
    WsjcppLog::err(TAG, "Could not delete " + sSQL);
    return false;
  }

  return true;
}

bool FhqServerDbUsefulLinks::updateRecord(const ModelUsefulLink &m) {
  std::lock_guard<std::mutex> lock(m_mutex);

  FhqServerDatabaseSqlQueryUpdate sql("useful_links");
  sql.add("url", m.getUrl());
  sql.add("description", m.getDescription());
  sql.add("author", m.getAuthor());
  sql.where("uuid", m.getUuid());

  if (!this->executeQuery(sql.getSql())) {
    WsjcppLog::err(TAG, "Could not delete " + sql.getSql());
    return false;
  }
  return true;
}

std::vector<std::string> FhqServerDbUsefulLinks::getUserFavorites(const std::string &sUserUuid) {
  std::lock_guard<std::mutex> lock(m_mutex);

  std::vector<std::string> records;
  FhqServerDatabaseSelectRows cur;
  FhqServerDatabaseSqlQuerySelect sql("useful_links_user_favorites");
  sql.sel("usefullinkuuid");
  sql.where("useruuid", sUserUuid);

  // TODO make a special struct like a uuid (fo fix security issues sql inejction or redesign to FhqServerDatabaseSqlQuery)
  // std::string sSQL = "SELECT * FROM useful_links_user_favorites WHERE  = '" + sUserUuid + "';";
  if (this->selectRows(sql.getSql(), cur)) {
    while (cur.next()) {
      records.push_back(cur.getString(0));
    }
  }
  return records;
}
