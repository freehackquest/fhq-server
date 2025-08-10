/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2024 FreeHackQuest <freehackquest@gmail.com>
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

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <employ_database.h>
#include <employ_games.h>
#include <employ_notify.h>
#include <fstream>
#include <json.hpp>
#include <sys/stat.h>

REGISTRY_WJSCPP_EMPLOY(EmployGames)

// ---------------------------------------------------------------------

EmployGames::EmployGames()
    : WsjcppEmployBase(EmployGames::name(),
                       {EmployDatabase::name(), EmployGlobalSettings::name(), EmployNotify::name()}) {
  TAG = EmployGames::name();
}

// ---------------------------------------------------------------------

bool EmployGames::init() {
  // TODO mutex
  // check the access to games folder
  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  std::string sBasePath = pGlobalSettings->get("web_public_folder").getDirPathValue();
  std::string sFileStoragePath = pGlobalSettings->get("file_storage").getDirPathValue();

  if (!testCreateFile(sBasePath)) {
    return false;
  }

  if (!testCreateFile(sFileStoragePath)) {
    return false;
  }

  // load list of games to cache
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());

  QSqlQuery query(db);
  query.prepare("SELECT * FROM games");

  if (!query.exec()) {
    WsjcppLog::err(TAG, query.lastError().text().toStdString());
    return false;
  }

  while (query.next()) {
    QSqlRecord record = query.record();
    ModelGame *pModelGame = new ModelGame();
    pModelGame->setLocalId(record.value("id").toInt());
    std::string sUuid = record.value("uuid").toString().toStdString();
    pModelGame->setUuid(sUuid);

    pModelGame->setName(record.value("title").toString().toStdString());
    pModelGame->setDescription(record.value("description").toString().toStdString());
    pModelGame->setOrganizators(record.value("organizators").toString().toStdString());
    pModelGame->setDateStart(record.value("date_start").toString().toStdString());
    pModelGame->setDateStop(record.value("date_stop").toString().toStdString());
    pModelGame->setDateRestart(record.value("date_restart").toString().toStdString());
    pModelGame->setState(record.value("state").toString().toStdString());
    pModelGame->setForm(record.value("form").toString().toStdString());
    pModelGame->setType(record.value("type_game").toString().toStdString());

    // TODO must be cached all fields

    if (m_mapCacheGames.count(sUuid)) {
      WsjcppLog::err(TAG, "Inconsistent list games in database uuid: " + sUuid);
      return false;
    } else {
      m_mapCacheGames.insert(std::pair<std::string, ModelGame *>(sUuid, pModelGame));
      m_vectCacheGame.push_back(pModelGame);
    }
  }

  // load from file_storage
  std::string sFileStorageGamesPath = WsjcppCore::doNormalizePath(sFileStoragePath + "/games");
  std::vector<std::string> vGames = WsjcppCore::getListOfDirs(sFileStorageGamesPath);
  for (int i = 0; i < vGames.size(); i++) {
    std::string sGameDataPath = sFileStorageGamesPath + "/" + vGames[i];
    sGameDataPath = WsjcppCore::doNormalizePath(sGameDataPath);
    WsjcppLog::info(TAG, "Found folder with game info: " + sGameDataPath);

    std::string sGameJsonDataPath = sGameDataPath + "/game.json";
    sGameJsonDataPath = WsjcppCore::doNormalizePath(sGameJsonDataPath);
    nlohmann::json jsonGame;
    try {
      std::ifstream ifs(sGameJsonDataPath);
      jsonGame = nlohmann::json::parse(ifs);
    } catch (nlohmann::json::parse_error &ex) {
      WsjcppLog::err(TAG, "Could not load ort parse file: " + sGameJsonDataPath);
      continue;
    }

    ModelGame *pModelGame = new ModelGame();
    std::string sError;
    if (!pModelGame->fillFrom(jsonGame, sError)) {
      WsjcppLog::err(TAG, "Game has wrong format: " + sError);
      continue;
    }
    WsjcppLog::ok(TAG, "Readed new game info from " + sGameJsonDataPath);
    std::string sUuid = pModelGame->uuid();
    if (m_mapCacheGames.count(sUuid)) {
      WsjcppLog::err(TAG, "Already defined in a list: " + sUuid);
      continue;
    }

    m_mapCacheGames.insert(std::pair<std::string, ModelGame *>(sUuid, pModelGame));
    m_vectCacheGame.push_back(pModelGame);
    // TODO
  }

  return true;
}

// ---------------------------------------------------------------------

bool EmployGames::deinit() {
  // TODO
  return true;
}

bool EmployGames::findGame(int nLocalId, ModelGame &modelGame) {
  // TODO mutex
  for (int i = 0; i < m_vectCacheGame.size(); i++) { // TODO create map with index
    if (m_vectCacheGame[i]->localId() == nLocalId) {
      modelGame.copy(*m_vectCacheGame[i]);
      return true;
    }
  }
  return false;
}

bool EmployGames::findGame(const std::string &sUuid, ModelGame &modelGame) {
  if (!m_mapCacheGames.count(sUuid)) {
    return false;
  }
  modelGame.copy(*m_mapCacheGames.at(sUuid));
  return true;
}

EmployResult EmployGames::addGame(const ModelGame &modelGame, std::string &sError) {
  // TODO save to file_storage/games/
  // TODO mutex
  if (m_mapCacheGames.count(modelGame.uuid())) {
    return EmployResult::ALREADY_EXISTS;
  }

  if (modelGame.name().length() == 0) {
    return EmployResult::ERROR_NAME_IS_EMPTY;
  }

  ModelGame *pModelGame = modelGame.clone(); // clone of original game

  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());

  {
    QSqlQuery query(db);
    query.prepare("INSERT INTO games("
                  "       uuid, title, type_game,"
                  "       date_start, date_stop, date_restart,"
                  "       description,"
                  "       organizators,"
                  "       state, form, "
                  "       maxscore,"
                  "       owner,"
                  "       date_create, " // TODO change created
                  "       date_change "  // TODO change updated
                  "   )"
                  "   VALUES("
                  "       :uuid, :name, :type_game,"
                  "       :date_start, :date_stop, :date_restart,"
                  "       :description,"
                  "       :organizators,"
                  "       :state, :form, "
                  "       :maxscore,"
                  "       :owner,"
                  "       NOW(),"
                  "       NOW()"
                  "   )");
    query.bindValue(":uuid", QString::fromStdString(pModelGame->uuid()));
    query.bindValue(":name", QString(pModelGame->name().c_str()));
    query.bindValue(":type_game", QString(pModelGame->type().c_str()));
    query.bindValue(":date_start", QString(pModelGame->dateStart().c_str()));
    query.bindValue(":date_stop", QString(pModelGame->dateStop().c_str()));
    query.bindValue(":date_restart", QString(pModelGame->dateRestart().c_str()));
    query.bindValue(":description", QString(pModelGame->description().c_str()));
    query.bindValue(":organizators", QString(pModelGame->organizators().c_str()));
    query.bindValue(":state", QString(pModelGame->state().c_str()));
    query.bindValue(":form", QString(pModelGame->form().c_str()));
    query.bindValue(":maxscore", 0);
    query.bindValue(":owner", 0);

    if (!query.exec()) {
      delete pModelGame;
      sError = query.lastError().text().toStdString();
      return EmployResult::DATABASE_ERROR;
    }

    int rowid = query.lastInsertId().toInt();
    pModelGame->setLocalId(rowid);
  }

  m_mapCacheGames.insert(std::pair<std::string, ModelGame *>(pModelGame->uuid(), pModelGame));
  m_vectCacheGame.push_back(pModelGame);

  EmployNotify *pEmployNotify = findWsjcppEmploy<EmployNotify>();
  pEmployNotify->notifyInfo("games", "New [game#" + pModelGame->uuid() + "] " + pModelGame->name());
  return EmployResult::OK;
}

EmployResult EmployGames::updateGame(const ModelGame &modelGame, std::string &sError) {
  // TODO mutex

  std::string sUuid = modelGame.uuid();

  if (!m_mapCacheGames.count(sUuid)) {
    return EmployResult::GAME_NOT_FOUND;
  }

  ModelGame *pOrigModelGame = m_mapCacheGames[sUuid];

  EmployNotify *pEmployNotify = findWsjcppEmploy<EmployNotify>();
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());

  // game name
  if (pOrigModelGame->name() != modelGame.name()) {
    QSqlQuery query(db);
    query.prepare("UPDATE games SET title = :name WHERE uuid = :gameuuid");
    query.bindValue(":name", QString::fromStdString(modelGame.name()));
    query.bindValue(":gameuuid", QString::fromStdString(sUuid));
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      return EmployResult::DATABASE_ERROR;
    }
    ModelNotification notification("info", "games",
                                   "Updated name of game [game#" + sUuid + "] from [" + pOrigModelGame->name() +
                                       "] to [" + modelGame.name() + "]");
    pOrigModelGame->setName(modelGame.name());
    pEmployNotify->sendNotification(notification);
  }

  // game type
  if (pOrigModelGame->type() != modelGame.type()) {
    QSqlQuery query(db);
    query.prepare("UPDATE games SET type_game = :type_game WHERE uuid = :gameuuid");
    query.bindValue(":type_game", QString::fromStdString(modelGame.type()));
    query.bindValue(":gameuuid", QString::fromStdString(sUuid));
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      return EmployResult::DATABASE_ERROR;
    }
    ModelNotification notification("info", "games",
                                   "Updated type of game {" + sUuid + "} from [" + pOrigModelGame->type() + "] to [" +
                                       modelGame.type() + "]");
    pOrigModelGame->setType(modelGame.type());
    pEmployNotify->sendNotification(notification);
  }

  // description
  if (pOrigModelGame->description() != modelGame.description()) {
    QSqlQuery query(db);
    query.prepare("UPDATE games SET description = :description WHERE uuid = :gameuuid");
    query.bindValue(":description", QString::fromStdString(modelGame.description()));
    query.bindValue(":gameuuid", QString::fromStdString(sUuid));
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      return EmployResult::DATABASE_ERROR;
    }
    ModelNotification notification("info", "games", "Updated description of the game {" + sUuid + "}");
    pOrigModelGame->setDescription(modelGame.description());
    pEmployNotify->sendNotification(notification);
  }

  // state
  if (pOrigModelGame->state() != modelGame.state()) {
    QSqlQuery query(db);
    query.prepare("UPDATE games SET `state` = :state WHERE uuid = :gameuuid");
    query.bindValue(":state", QString::fromStdString(modelGame.state()));
    query.bindValue(":gameuuid", QString::fromStdString(sUuid));
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      return EmployResult::DATABASE_ERROR;
    }
    ModelNotification notification("info", "games",
                                   "Updated state of game {" + sUuid + "} from [" + pOrigModelGame->state() + "] to [" +
                                       modelGame.state() + "]");
    pOrigModelGame->setState(modelGame.state());
    pEmployNotify->sendNotification(notification);
  }

  // form
  if (pOrigModelGame->form() != modelGame.form()) {
    QSqlQuery query(db);
    query.prepare("UPDATE games SET `form` = :form WHERE uuid = :gameuuid");
    query.bindValue(":form", QString::fromStdString(modelGame.form()));
    query.bindValue(":gameuuid", QString::fromStdString(sUuid));
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      return EmployResult::DATABASE_ERROR;
    }
    ModelNotification notification("info", "games",
                                   "Updated form of game {" + sUuid + "} from [" + pOrigModelGame->form() + "] to [" +
                                       modelGame.form() + "]");
    pOrigModelGame->setForm(modelGame.form());
    pEmployNotify->sendNotification(notification);
  }

  // organizators
  if (pOrigModelGame->organizators() != modelGame.organizators()) {
    QSqlQuery query(db);
    query.prepare("UPDATE games SET `organizators` = :organizators WHERE uuid "
                  "= :gameuuid");
    query.bindValue(":organizators", QString::fromStdString(modelGame.organizators()));
    query.bindValue(":gameuuid", QString::fromStdString(sUuid));
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      return EmployResult::DATABASE_ERROR;
    }
    ModelNotification notification("info", "games", "Updated organizators of game {" + sUuid + "}");
    pOrigModelGame->setOrganizators(modelGame.organizators());
    pEmployNotify->sendNotification(notification);
  }

  // date_start
  if (pOrigModelGame->dateStart() != modelGame.dateStart()) {
    QSqlQuery query(db);
    query.prepare("UPDATE games SET `date_start` = :date_start WHERE uuid = :gameuuid");
    query.bindValue(":date_start", QString::fromStdString(modelGame.dateStart()));
    query.bindValue(":gameuuid", QString::fromStdString(sUuid));
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      return EmployResult::DATABASE_ERROR;
    }
    ModelNotification notification("info", "games",
                                   "Updated date start of game {" + sUuid + "} from [" + pOrigModelGame->dateStart() +
                                       "] to [" + modelGame.dateStart() + "]");
    pOrigModelGame->setDateStart(modelGame.dateStart());
    pEmployNotify->sendNotification(notification);
  }

  // date_stop
  if (pOrigModelGame->dateStop() != modelGame.dateStop()) {
    QSqlQuery query(db);
    query.prepare("UPDATE games SET `date_stop` = :date_stop WHERE uuid = :gameuuid");
    query.bindValue(":date_stop", QString::fromStdString(modelGame.dateStop()));
    query.bindValue(":gameuuid", QString::fromStdString(sUuid));
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      return EmployResult::DATABASE_ERROR;
    }
    ModelNotification notification("info", "games",
                                   "Updated date stop of game {" + sUuid + "} from [" + pOrigModelGame->dateStop() +
                                       "] to [" + modelGame.dateStop() + "]");
    pOrigModelGame->setDateStop(modelGame.dateStop());
    pEmployNotify->sendNotification(notification);
  }

  // date_restart
  if (pOrigModelGame->dateRestart() != modelGame.dateRestart()) {
    QSqlQuery query(db);
    query.prepare("UPDATE games SET `date_restart` = :date_restart WHERE uuid "
                  "= :gameuuid");
    query.bindValue(":date_restart", QString::fromStdString(modelGame.dateRestart()));
    query.bindValue(":gameuuid", QString::fromStdString(sUuid));
    if (!query.exec()) {
      sError = query.lastError().text().toStdString();
      return EmployResult::DATABASE_ERROR;
    }
    ModelNotification notification("info", "games",
                                   "Updated date restart of game {" + sUuid + "} from [" +
                                       pOrigModelGame->dateRestart() + "] to [" + modelGame.dateRestart() + "]");
    pOrigModelGame->setDateRestart(modelGame.dateRestart());
    pEmployNotify->sendNotification(notification);
  }

  return EmployResult::OK;
}

EmployResult EmployGames::removeGame(const std::string &sUuid) {
  // TODO mutex
  if (!m_mapCacheGames.count(sUuid)) {
    return EmployResult::GAME_NOT_FOUND;
  }
  m_mapCacheGames.erase(sUuid);

  // TODO also remove from vect

  // TODO remove from database

  // TODO remove from filesystem
  return EmployResult::OK;
}

const std::vector<ModelGame *> &EmployGames::getListOfGames() { return m_vectCacheGame; }

bool EmployGames::testCreateFile(const std::string &sPath) {

  std::string sTargetTestFile = sPath + "/games/test"; // normalize path
  sTargetTestFile = WsjcppCore::doNormalizePath(sTargetTestFile);
  if (!WsjcppCore::createEmptyFile(sTargetTestFile)) {
    WsjcppLog::err(TAG, "Cannot access to write " + sTargetTestFile);
    return false;
  }
  if (!WsjcppCore::removeFile(sTargetTestFile)) {
    WsjcppLog::err(TAG, "Could not delete file " + sTargetTestFile);
    return false;
  }
  return true;
}