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

#include "update_215438peu8_9gwc2tm5ah.h"
#include <employ_database.h>

REGISTRY_WSJCPP_STORAGE_UPDATE(Update_215438peu8_9gwc2tm5ah)

class UpdateRecords1 : public WsjcppStorageChanges {
public:
  UpdateRecords1();
  virtual WsjcppStorageChangesType getType() const override;
  virtual std::string getStartApply() const override;
  virtual std::string getAppliedSuccess() const override;
  virtual std::string getAppliedFailed() const override;
  virtual bool applyCustomChanges() const override;
};

UpdateRecords1::UpdateRecords1()
  : WsjcppStorageChanges("quests_files") {
      // nothing
    };

WsjcppStorageChangesType UpdateRecords1::getType() const {
  return WsjcppStorageChangesType::WSJCPP_STORAGE_CHANGES_TYPE_CUSTOM_CHANGES;
}

std::string UpdateRecords1::getStartApply() const { return "Appling changes for table " + m_sTableName; }

std::string UpdateRecords1::getAppliedSuccess() const {
  return "Changes (removed public/ from filepath) success applied for table " + m_sTableName;
}

std::string UpdateRecords1::getAppliedFailed() const {
  return "Could not apply changes (removed public/ from filepath) in table " + m_sTableName;
}

bool UpdateRecords1::applyCustomChanges() const {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);
  query.prepare("SELECT * FROM quests_files");

  if (!query.exec()) {
    WsjcppLog::err(TAG, query.lastError().text().toStdString());
    return false;
  }

  while (query.next()) {
    QSqlRecord record = query.record();
    int nId = record.value("id").toInt();
    std::string sFilePath = record.value("filepath").toString().toStdString();
    WsjcppLog::info(TAG, std::to_string(nId) + " => " + sFilePath);
    if (sFilePath.rfind("public/", 0) == 0) {
      sFilePath = sFilePath.substr(7); // remove "public/"
      sFilePath = "/" + sFilePath;
      WsjcppLog::info(TAG, "    -> new: " + sFilePath);
      QSqlQuery queryUpdate(db);
      queryUpdate.prepare("UPDATE quests_files SET filepath = :filepath WHERE id = :id");
      queryUpdate.bindValue(":filepath", QString::fromStdString(sFilePath));
      queryUpdate.bindValue(":id", nId);
      if (!queryUpdate.exec()) {
        WsjcppLog::err(TAG, queryUpdate.lastError().text().toStdString());
        return false;
      }
    }
  }
  return true;
}

Update_215438peu8_9gwc2tm5ah::Update_215438peu8_9gwc2tm5ah()
  : WsjcppStorageUpdateBase("215438peu8", "9gwc2tm5ah", "Update quests_files (remove prefix public/)") {

  m_vStorageChanges.push_back(new UpdateRecords1());
}
