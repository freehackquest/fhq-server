#ifndef MODEL_GAME_H
#define MODEL_GAME_H

#include <json.hpp>
#include <string>
#include <validators.h>

const int MAX_FREEHACKQUEST_GAME_FORMAT_VERSION = 1;

class ModelGame {
public:
  ModelGame();
  ~ModelGame();

  int localId() const;
  void setLocalId(int nLocalId);
  const std::string &uuid() const;
  void setUuid(const std::string &sUuid);
  const std::string &name() const;
  void setName(std::string sName);
  const std::string &description() const;
  void setDescription(std::string sDescription);
  const std::string &state() const;
  void setState(const std::string &sState);
  const std::string &form() const;
  void setForm(std::string sForm);
  const std::string &type() const;
  void setType(std::string sType);
  const std::string &dateStart() const;
  void setDateStart(std::string sDateStart);
  const std::string &dateStop() const;
  void setDateStop(std::string sDateStop);
  const std::string &dateRestart() const;
  void setDateRestart(std::string sDateRestart);
  const std::string &organizators() const;
  void setOrganizators(std::string sOrganizators);
  int maxScore() const;
  void setMaxScore(int nMaxScore);

  void copy(const ModelGame &modelGame);
  ModelGame *clone() const;
  nlohmann::json toJson();
  bool fillFrom(const nlohmann::json &jsonGame, std::string &sError);
  bool fillFromFreeHackQuestFormatVersion1(const nlohmann::json &jsonGame, std::string &sError);

private:
  std::string TAG;
  int m_nLocalId; // deprecated
  int m_nFreeHackQuestGameFormatVersion;
  std::string m_sUuid;
  std::string m_sName;
  std::string m_sDescription;
  std::string m_sState;
  std::string m_sForm;
  std::string m_sType;
  std::string m_sDateStart;
  std::string m_sDateStop;
  std::string m_sDateRestart;
  std::string m_sOrganizators;
  int m_nMaxScore;
  ValidatorGameType *m_pValidatorGameType;
  ValidatorGameState *m_pValidatorGameState;
  ValidatorGameForm *m_pValidatorGameForm;
  WsjcppValidatorUUID *m_pValidatorUuid;

  bool readFieldFreeHackQuestGameFormatVersionFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldUuidRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldNameRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldDescriptionOptionalFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldStateRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldFormRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldTypeRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldDateStartRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldDateStopRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldDateRestartRequiredFromJson(const nlohmann::json &jsonGame, std::string &sError);
  bool readFieldOrganizatorsOptionalFromJson(const nlohmann::json &jsonGame, std::string &sError);
};

#endif // MODEL_GAME_H
