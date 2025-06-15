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

#include <cstring>
#include <mutex>
#include <vector>

class FhqServerDatabaseFileUpdateInfo {
public:
  FhqServerDatabaseFileUpdateInfo(
    const std::string &sVersionFrom, const std::string &sVersionTo, const std::string &sDescription
  );
  const std::string &versionFrom() const;
  const std::string &versionTo() const;
  const std::string &description() const;

private:
  std::string m_sVersionFrom;
  std::string m_sVersionTo;
  std::string m_sDescription;
};

class FhqServerDatabaseFile;

class FhqServerDatabaseFileUpdate {
public:
  FhqServerDatabaseFileUpdate(
    const std::string &sVersionFrom, const std::string &sVersionTo, const std::string &sDescription
  );
  const FhqServerDatabaseFileUpdateInfo &info();
  void setWeight(int nWeight);
  int getWeight();
  virtual bool applyUpdate(FhqServerDatabaseFile *pDatabaseFile) = 0;

protected:
  std::string TAG;

private:
  FhqServerDatabaseFileUpdateInfo m_updateInfo;
  int m_nWeight;
};

class FhqServerDatabaseSelectRows {
public:
  FhqServerDatabaseSelectRows();
  ~FhqServerDatabaseSelectRows();
  void setQuery(void *pQuery);
  bool next();
  std::string getString(int nColumnNumber);
  long getLong(int nColumnNumber);

private:
  // hidden type 'sqlite3_stmt *'
  void *m_pQuery;
};

enum class FhqServerDatabaseSqlQueryType { SELECT, INSERT, UPDATE };

class FhqServerDatabaseSqlQuery {
public:
  FhqServerDatabaseSqlQuery(FhqServerDatabaseSqlQueryType nSqlType, const std::string &sSqlTable);
  bool sel(const std::string &sColumnName);
  bool add(const std::string &sColumnName, const std::string &sValue);
  bool add(const std::string &sColumnName, int nValue);
  bool add(const std::string &sColumnName, long nValue);
  bool where(const std::string &sColumnName, const std::string &sValue);
  bool where(const std::string &sColumnName, int nValue);
  bool where(const std::string &sColumnName, long nValue);

  std::string getSql();
  bool isValid();
  std::string getErrorMessage();

private:
  std::string prepareStringValue(const std::string &sValue);
  bool checkName(const std::string &sColumnName);
  FhqServerDatabaseSqlQueryType m_nSqlType;
  std::string m_sSqlTable;
  std::string m_sErrorMessage;
  bool m_bValid;

  // query parts
  std::string m_sSqlQuery0;
  std::string m_sSqlQuery1;
  std::string m_sSqlQuery2;
};

class FhqServerDatabaseSqlQuerySelect : public FhqServerDatabaseSqlQuery {
public:
  FhqServerDatabaseSqlQuerySelect(const std::string &sSqlTable);
};

class FhqServerDatabaseSqlQueryInsert : public FhqServerDatabaseSqlQuery {
public:
  FhqServerDatabaseSqlQueryInsert(const std::string &sSqlTable);
};

class FhqServerDatabaseSqlQueryUpdate : public FhqServerDatabaseSqlQuery {
public:
  FhqServerDatabaseSqlQueryUpdate(const std::string &sSqlTable);
};

enum class FhqServerDatabaseSqlQueryType { INSERT, UPDATE };

class FhqServerDatabaseSqlQuery {
public:
  FhqServerDatabaseSqlQuery(FhqServerDatabaseSqlQueryType nSqlType, const std::string &sSqlTable);
  bool sel(const std::string &sColumnName);
  bool add(const std::string &sColumnName, const std::string &sValue);
  bool add(const std::string &sColumnName, int nValue);
  bool add(const std::string &sColumnName, long nValue);
  bool where(const std::string &sColumnName, const std::string &sValue);
  bool where(const std::string &sColumnName, int nValue);
  bool where(const std::string &sColumnName, long nValue);

  std::string getSql();
  bool isValid();
  std::string getErrorMessage();

private:
  std::string prepareStringValue(const std::string &sValue);
  bool checkName(const std::string &sColumnName);
  FhqServerDatabaseSqlQueryType m_nSqlType;
  std::string m_sSqlTable;
  std::string m_sErrorMessage;
  bool m_bValid;

  // query parts
  std::string m_sSqlQuery0;
  std::string m_sSqlQuery1;
  std::string m_sSqlQuery2;
};

class FhqServerDatabaseSqlQuerySelect : public FhqServerDatabaseSqlQuery {
public:
  FhqServerDatabaseSqlQuerySelect(const std::string &sSqlTable);
};

class FhqServerDatabaseSqlQueryInsert : public FhqServerDatabaseSqlQuery {
public:
  FhqServerDatabaseSqlQueryInsert(const std::string &sSqlTable);
};

class FhqServerDatabaseSqlQueryUpdate : public FhqServerDatabaseSqlQuery {
public:
  FhqServerDatabaseSqlQueryUpdate(const std::string &sSqlTable);
};

class FhqServerDatabaseFile {
public:
  FhqServerDatabaseFile(const std::string &sFilename);
  ~FhqServerDatabaseFile();
  std::string getFilename();
  std::string getFileFullpath();
  bool open();
  bool executeQuery(std::string sSqlInsert);
  int selectSumOrCount(std::string sSqlSelectCount);
  bool selectRows(std::string sSqlSelectRows, FhqServerDatabaseSelectRows &selectRows);

protected:
  bool installUpdates();
  bool insertDbVersion(const FhqServerDatabaseFileUpdateInfo &info);
  std::vector<FhqServerDatabaseFileUpdate *> m_vDbUpdates;
  std::string TAG;

private:
  void copyDatabaseToBackup();
  std::mutex m_mutex;

  // hidden type 'sqlite3 *'
  void *m_pDatabaseFile;
  std::string m_sFilename;
  std::string m_sFileFullpath;
  std::string m_sBaseFileBackupFullpath;
  int m_nLastBackupTime;
};
