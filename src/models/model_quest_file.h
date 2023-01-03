#ifndef MODEL_QUEST_FILE_H
#define MODEL_QUEST_FILE_H

#include <json.hpp>
#include <string>
#include <validators.h>

class ModelQuestFile {
public:
  ModelQuestFile();
  ~ModelQuestFile();

  int getLocalId() const;
  void setLocalId(int nVal);
  const std::string &getUuid() const;
  void setUuid(std::string sUuid);
  void setQuestLocalId(int nVal);
  int getQuestLocalId() const;
  void setDownloadsCounter(int nVal);
  int getDownloadsCounter() const;
  const std::string &getFileName() const;
  void setFileName(std::string sVal);
  void setFileSize(int nVal);
  int getFileSize() const;
  const std::string &getDatetime() const;
  void setDatetime(std::string sVal);
  const std::string &getFilePath() const;
  void setFilePath(std::string sVal);
  void setMd5(std::string sVal);
  const std::string &getMd5() const;

  void copy(const ModelQuestFile &ModelQuestFile);
  ModelQuestFile *clone() const;
  nlohmann::json toJson();

private:
  std::string TAG;
  int m_nLocalId;
  std::string m_sUuid;
  int m_nQuestLocalId;
  int m_nDownloadsCounter;
  std::string m_sFileName;
  int m_nFileSize;
  std::string m_sFileHumanSize;
  std::string m_sDatetime;
  std::string m_sFilePath;
  std::string m_sMd5;
};

#endif // MODEL_QUEST_FILE_H
