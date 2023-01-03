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
    : WsjcppStorageChanges("quests_files"){

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
