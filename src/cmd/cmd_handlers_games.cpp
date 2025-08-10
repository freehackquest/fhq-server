#include <QDataStream>
#include <QFile>
#include <QTemporaryDir>
#include <cmd_handlers_games.h>
#include <employ_database.h>
#include <employ_games.h>
#include <employ_images.h>
#include <employ_notify.h>
#include <fstream>
#include <iostream>
#include <quazip.h>
#include <quazipfile.h>
#include <quazipfileinfo.h>
#include <runtasks.h>
#include <validators.h>
#include <wsjcpp_hashes.h>

// ---------------------------------------------------------------------
// Create Game

REGISTRY_CMD(CmdHandlerGameCreate)

CmdHandlerGameCreate::CmdHandlerGameCreate() : CmdHandlerBase("game_create", "Create the game") {

  setActivatedFromVersion("0.2.0");

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  // validation and description input fields
  requireIntegerParam("freehackquest_game_format_version", "Format Version");
  requireStringParam("uuid", "Global Identificator of the Game").addValidator(new WsjcppValidatorUUID());
  requireStringParam("name", "Name of the Game");
  requireStringParam("description", "Description of the Game");
  requireStringParam("state", "State of the game").addValidator(new ValidatorGameState());
  requireStringParam("form", "Form of the game").addValidator(new ValidatorGameForm());
  requireStringParam("type", "Type of the game").addValidator(new ValidatorGameType());
  requireStringParam("date_start", "Date start");     // TODO validator datetime
  requireStringParam("date_stop", "Date stop");       // TODO validator datetime
  requireStringParam("date_restart", "Date restart"); // TODO validator datetime
  requireStringParam("organizators", "Organizators");
}

// ---------------------------------------------------------------------

void CmdHandlerGameCreate::handle(ModelRequest *pRequest) {
  EmployGames *pEmployGames = findWsjcppEmploy<EmployGames>();

  nlohmann::json jsonRequest = pRequest->jsonRequest();

  ModelGame pModelGame;
  std::string sError;
  if (!pModelGame.fillFrom(jsonRequest, sError)) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(400, "Game have wrong format: " + sError));
    return;
  }

  EmployResult nResult = pEmployGames->addGame(pModelGame, sError);
  switch (nResult) {

  case EmployResult::DATABASE_ERROR: {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, sError));
    break;
  }

  case EmployResult::ALREADY_EXISTS: {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(403, "Game already exists with this uuid"));
    break;
  }

  case EmployResult::ERROR_NAME_IS_EMPTY: {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(400, "Game has empty name"));
    break;
  }

  case EmployResult::OK: {
    nlohmann::json jsonResponse;
    pEmployGames->findGame(pModelGame.uuid(), pModelGame);
    jsonResponse["data"] = pModelGame.toJson();
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
    break;
  }

  default: {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, "Server error"));
  }
  }
}

// ---------------------------------------------------------------------
// Delete Game

REGISTRY_CMD(CmdHandlerGameDelete)

CmdHandlerGameDelete::CmdHandlerGameDelete() : CmdHandlerBase("game_delete", "Remove game and all quests") {

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  // validation and description input fields
  requireStringParam("uuid", "Global Identificator of the Game").addValidator(new WsjcppValidatorUUID());

  requireStringParam("admin_password", "Admin Password");
}

// ---------------------------------------------------------------------

void CmdHandlerGameDelete::handle(ModelRequest *pRequest) {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  nlohmann::json jsonResponse;
  std::string sUuid = pRequest->getInputString("uuid", "");
  std::string sAdminPassword = pRequest->getInputString("admin_password", "");

  int nUserID = pRequest->getUserSession()->userid();

  QSqlDatabase db = *(pDatabase->database());

  // TODO easy way for check admin password
  {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE id = :userid");
    query.bindValue(":userid", nUserID);
    if (!query.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
      return;
    }

    std::string sPass = "";
    std::string sEmail = "";

    if (query.next()) {
      QSqlRecord record = query.record();
      sEmail = record.value("email").toString().toUpper().toStdString();
      sPass = record.value("pass").toString().toStdString();
    } else {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(404, "Not found user"));
      return;
    }

    std::string sAdminPasswordHash = WsjcppHashes::sha1_calc_hex(sEmail + sAdminPassword);

    if (sAdminPasswordHash != sPass) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(401, "Wrong password"));
      return;
    }
  }

  int nGameID = 0;
  std::string sName = "";

  // check existing game
  {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM games WHERE uuid = :uuid");
    query.bindValue(":uuid", QString::fromStdString(sUuid));

    if (!query.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
      return;
    }

    if (!query.next()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(404, "Game not found"));
      return;
    } else {
      QSqlRecord record = query.record();
      nGameID = record.value("id").toInt();
      sName = record.value("title").toString().toStdString();
    }
  }

  EmployGames *pEmployGames = findWsjcppEmploy<EmployGames>();
  pEmployGames->removeGame(sUuid); // TODO just removed from cache

  // delete from users_quests_answers
  {
    QSqlQuery query_del(db);
    query_del.prepare("DELETE FROM users_quests_answers WHERE questid IN "
                      "(SELECT idquest FROM quest q WHERE q.gameid = :gameid)");
    query_del.bindValue(":gameid", nGameID);
    if (!query_del.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query_del.lastError().text().toStdString()));
      return;
    }
  }

  // delete from users_quests
  {
    QSqlQuery query_del(db);
    query_del.prepare("DELETE FROM users_quests WHERE questid IN (SELECT "
                      "idquest FROM quest q WHERE q.gameid = :gameid)");
    query_del.bindValue(":gameid", nGameID);
    if (!query_del.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query_del.lastError().text().toStdString()));
      return;
    }
  }

  // delete from quests
  {
    QSqlQuery query_del(db);
    query_del.prepare("DELETE FROM quest WHERE gameid = :gameid");
    query_del.bindValue(":gameid", nGameID);
    if (!query_del.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query_del.lastError().text().toStdString()));
      return;
    }
  }

  // delete from games
  {
    QSqlQuery query_del(db);
    query_del.prepare("DELETE FROM games WHERE id = :gameid");
    query_del.bindValue(":gameid", nGameID);
    if (!query_del.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query_del.lastError().text().toStdString()));
      return;
    }
  }

  // delete game logo if exists
  std::string sGameLogoFilename = "";
  {
    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    std::string sBasePath = pGlobalSettings->get("web_public_folder").getDirPathValue();
    sGameLogoFilename = sBasePath + "games/" + std::to_string(nGameID) + ".png";
    if (remove(sGameLogoFilename.c_str()) != 0) {
      WsjcppLog::err(TAG, "Could not delete file " + sGameLogoFilename);
    }
  }

  EmployNotify *pNotify = findWsjcppEmploy<EmployNotify>();
  ModelNotification notification("warning", "games", "Removed [game#" + sUuid + "] " + sName);
  pNotify->sendNotification(notification);
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Export Game

REGISTRY_CMD(CmdHandlerGameExport)

CmdHandlerGameExport::CmdHandlerGameExport() : CmdHandlerBase("game_export", "Export the game") {

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  // validation and description input fields
  requireStringParam("uuid", "Global Identificator of the Game").addValidator(new WsjcppValidatorUUID());
}

// ---------------------------------------------------------------------

void CmdHandlerGameExport::handle(ModelRequest *pRequest) {
  // EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  EmployGames *pEmployGames = findWsjcppEmploy<EmployGames>();

  nlohmann::json jsonResponse;

  std::string sUuid = pRequest->getInputString("uuid", "");

  ModelGame modelGame;
  if (!pEmployGames->findGame(sUuid, modelGame)) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(404, "Game not found"));
    return;
  }

  // find logo for game
  QString sGameLogoFilename = "";
  {
    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    QString sBasePath = QString::fromStdString(pGlobalSettings->get("web_public_folder").getDirPathValue() + "games/");
    sGameLogoFilename = sBasePath + QString::number(modelGame.localId()) + ".png";
  }

  QString tmpDir = QDir::tempPath();
  quint64 ts = QDateTime::currentMSecsSinceEpoch();
  QString tmpZipFile = tmpDir + "/fhq-server-game_" + QString::number(ts) + ".zip";

  // prepare zip archive
  QuaZip zip(tmpZipFile);
  zip.open(QuaZip::mdCreate);
  QuaZipFile export_zipfile(&zip);

  // pack logo
  {
    QFile fileLogo(sGameLogoFilename);
    if (fileLogo.exists() && fileLogo.open(QIODevice::ReadOnly)) {
      export_zipfile.open(QIODevice::WriteOnly, QuaZipNewInfo(QString::fromStdString(sUuid + ".png")));
      // After .toString(), you should specify a text codec to use to encode the
      // string data into the (binary) file. Here, I use UTF-8:
      QByteArray baLogo = fileLogo.readAll();
      export_zipfile.write(baLogo);
      export_zipfile.close();
    } else {
      WsjcppLog::warn(TAG, QString("Logo not found " + sGameLogoFilename).toStdString());
    }
  }

  // pack json file
  {
    export_zipfile.open(QIODevice::WriteOnly, QuaZipNewInfo(QString::fromStdString(sUuid + ".json")));
    std::string message = modelGame.toJson().dump();
    export_zipfile.write(QString(message.c_str()).toUtf8());
    export_zipfile.close();
  }
  zip.close();

  // preapre zip base64
  {
    QFile fileZip(tmpZipFile);
    if (!fileZip.open(QIODevice::ReadOnly)) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, "Could not open zip file"));
      return;
    }
    QByteArray baZip = fileZip.readAll();
    nlohmann::json jsonData;
    jsonData["zipfile_base64"] = QString(baZip.toBase64()).toStdString();
    jsonData["zipfile_name"] = "game_" + sUuid + ".zip";
    jsonResponse["data"] = jsonData;
    fileZip.close();
    fileZip.remove();
  }

  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Import Game

REGISTRY_CMD(CmdHandlerGameImport)

CmdHandlerGameImport::CmdHandlerGameImport() : CmdHandlerBase("game_import", "Import game") {

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  requireStringParam("uuid", "Global Identificator of the Game").addValidator(new WsjcppValidatorUUID());
}

// ---------------------------------------------------------------------

void CmdHandlerGameImport::handle(ModelRequest *pRequest) {
  // nlohmann::json jsonRequest = pRequest->jsonRequest();
  // nlohmann::json jsonResponse;

  pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(501, "Not Implemented Yet"));
  return;

  // TODO
  // pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Info Game

REGISTRY_CMD(CmdHandlerGameInfo)

CmdHandlerGameInfo::CmdHandlerGameInfo() : CmdHandlerBase("game_info", "Return game info") {

  setAccessUnauthorized(true);
  setAccessUser(true);
  setAccessAdmin(true);

  setDeprecatedFromVersion("0.2.39");

  // validation and description input fields
  requireStringParam("uuid", "Global Identificator of the Game").addValidator(new WsjcppValidatorUUID());
}

// ---------------------------------------------------------------------

void CmdHandlerGameInfo::handle(ModelRequest *pRequest) {
  EmployGames *pEmployGames = findWsjcppEmploy<EmployGames>();

  std::string sUuid = pRequest->getInputString("uuid", "");

  ModelGame modelGame;
  if (!pEmployGames->findGame(sUuid, modelGame)) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(404, "Game not found"));
    return;
  }

  nlohmann::json jsonResponse;
  jsonResponse["data"] = modelGame.toJson();
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Info Game

REGISTRY_CMD(CmdHandlerGamesInfo)

CmdHandlerGamesInfo::CmdHandlerGamesInfo() : CmdHandlerBase("games.info", "Return info about game by uuid") {

  setAccessUnauthorized(true);
  setAccessUser(true);
  setAccessAdmin(true);

  setActivatedFromVersion("0.2.39");

  // validation and description input fields
  requireStringParam("uuid", "Global Identificator of the Game").addValidator(new WsjcppValidatorUUID());
}

// ---------------------------------------------------------------------

void CmdHandlerGamesInfo::handle(ModelRequest *pRequest) {
  EmployGames *pEmployGames = findWsjcppEmploy<EmployGames>();

  std::string sUuid = pRequest->getInputString("uuid", "");

  ModelGame modelGame;
  if (!pEmployGames->findGame(sUuid, modelGame)) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(404, "Game not found"));
    return;
  }
  // TODO split for user and for admin
  nlohmann::json jsonResponse;
  jsonResponse["data"] = modelGame.toJson();
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Update Game

REGISTRY_CMD(CmdHandlerGameUpdate)

CmdHandlerGameUpdate::CmdHandlerGameUpdate() : CmdHandlerBase("game_update", "Update game info") {

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  // validation and description input fields
  requireStringParam("uuid", "Global Identificator of the Game").addValidator(new WsjcppValidatorUUID());
  optionalStringParam("name", "Name of the Game");
  optionalStringParam("description", "Description of the Game");
  optionalStringParam("state", "State of the game").addValidator(new ValidatorGameState());
  optionalStringParam("form", "Form of the game").addValidator(new ValidatorGameForm());
  optionalStringParam("type", "Type of the game").addValidator(new ValidatorGameType());
  optionalStringParam("date_start", "Date start"); // TODO validator datetime
  optionalStringParam("date_stop", "Date stop");   // TODO validator datetime
  optionalStringParam("date_restart",
                      "Date restart"); // TODO validator datetime
  optionalStringParam("organizators", "Organizators");
}

// ---------------------------------------------------------------------

void CmdHandlerGameUpdate::handle(ModelRequest *pRequest) {
  EmployGames *pEmployGames = findWsjcppEmploy<EmployGames>();

  ModelGame updatedModelGame;
  std::string sError;
  if (!updatedModelGame.fillFrom(pRequest->jsonRequest(), sError)) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(400, "Game have wrong format: " + sError));
    return;
  }

  ModelGame modelGame;
  if (!pEmployGames->findGame(updatedModelGame.uuid(), modelGame)) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(404, "Game not found"));
    return;
  }

  updatedModelGame.copy(modelGame);
  if (!updatedModelGame.fillFrom(pRequest->jsonRequest(), sError)) { // will be replaced values for existing fields
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(400, "Game have wrong format: " + sError));
    return;
  }

  EmployResult nResult = pEmployGames->updateGame(updatedModelGame, sError);

  switch (nResult) {

  case EmployResult::DATABASE_ERROR: {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, sError));
    break;
  }

  case EmployResult::GAME_NOT_FOUND: {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(404, "Game not found"));
    break;
  }

  case EmployResult::OK: {
    nlohmann::json jsonResponse;
    pEmployGames->findGame(modelGame.uuid(), modelGame);
    jsonResponse["data"] = modelGame.toJson();
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
    break;
  }

  default: {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, "Server error"));
  }
  }
}

// ---------------------------------------------------------------------
// Update Logo Game

REGISTRY_CMD(CmdHandlerGameUpdateLogo)

CmdHandlerGameUpdateLogo::CmdHandlerGameUpdateLogo() : CmdHandlerBase("game_update_logo", "Update game logo") {

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  // validation and description input fields
  requireStringParam("uuid", "Global Identificator of the Game").addValidator(new WsjcppValidatorUUID());

  requireStringParam("image_png_base64", "Image PNG in Base64");
}

void CmdHandlerGameUpdateLogo::handle(ModelRequest *pRequest) {
  EmployGames *pEmployGames = findWsjcppEmploy<EmployGames>();

  ModelGame modelGame;
  std::string sError;
  if (!modelGame.fillFrom(pRequest->jsonRequest(), sError)) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(400, "Game have wrong format: " + sError));
    return;
  }
  if (!pEmployGames->findGame(modelGame.uuid(), modelGame)) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(404, "Game not found"));
    return;
  }

  int nGameID = modelGame.localId();

  nlohmann::json jsonResponse;

  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  EmployImages *pImages = findWsjcppEmploy<EmployImages>();

  QString sBasePath = QString::fromStdString(pGlobalSettings->get("web_public_folder").getDirPathValue() + "games/");

  QString sFilename = sBasePath + QString::number(nGameID) + ".png";

  QByteArray baImagePNGBase64;

  // TODO can be crash
  nlohmann::json jsonRequest = pRequest->jsonRequest();
  QString sImagePngBase64 = QString::fromStdString(jsonRequest["image_png_base64"]);
  baImagePNGBase64.append(sImagePngBase64);
  // TODO replace decode base64 to std
  QByteArray baImagePNG = QByteArray::fromBase64(baImagePNGBase64); // .fromBase64(baImagePNGBase64);

  if (baImagePNG.size() == 0) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(400, "Could not decode base64"));
    return;
  }

  FILE *pFile;
  char *sSIF = new char[L_tmpnam];
  strcpy(sSIF, "/tmp/fileXXXXXX");
  int fd = mkstemp(sSIF);
  std::string sSourceImageFile(sSIF);
  std::cout << "temporary file name: " << sSourceImageFile << '\n';

  int nLen = baImagePNG.size();
  pFile = fdopen(fd, "wb");
  fwrite(baImagePNG.constData(), sizeof(char), nLen, pFile);
  fclose(pFile);

  std::string targetImageFile = sFilename.toStdString();
  // WsjcppLog::info(TAG, "targetImageFile " + targetImageFile);
  if (!pImages->doThumbnailImagePng(sSourceImageFile, targetImageFile, 100, 100)) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(400, "Could not decode bytearray to png"));
    // cleanup - redesign try finnaly
    remove(sSourceImageFile.c_str());
    return;
  }

  if (remove(sSourceImageFile.c_str()) != 0) {
    WsjcppLog::err(TAG, "Could not delete file " + sSourceImageFile);
  }

  if (FILE *file = fopen(targetImageFile.c_str(), "r")) {
    fclose(file);
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
  } else {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, "Problem with creation file"));
  }
}

// ---------------------------------------------------------------------
// List of Games

REGISTRY_CMD(CmdHandlerGamesList)

CmdHandlerGamesList::CmdHandlerGamesList() : CmdHandlerBase("games_list", "Method returned list of games") {

  setAccessUnauthorized(true);
  setAccessUser(true);
  setAccessAdmin(true);

  // validation and description input fields
  // optionalStringParam("filter_text", "Filter by user email or nick");
  // optionalStringParam("filter_role", "Filter by user role");
}

// ---------------------------------------------------------------------

void CmdHandlerGamesList::handle(ModelRequest *pRequest) {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  nlohmann::json jsonResponse;

  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  EmployGames *pGames = findWsjcppEmploy<EmployGames>();

  std::string sBaseUrl = pGlobalSettings->get("web_public_folder_url").getStringValue() + "games/";

  nlohmann::json jsonGames = nlohmann::json::array();

  // copy of list for sorting
  std::vector<ModelGame *> vGames = pGames->getListOfGames();

  for (auto it = vGames.begin(); it != vGames.end(); ++it) {
    ModelGame *pGame = *it;
    nlohmann::json jsonGame = pGame->toJson();
    int nLocalId = pGame->getLocalId();
    std::string sLogoUrl = sBaseUrl + std::to_string(nLocalId) + ".png";
    if (nLocalId != 0) {
      sLogoUrl = sBaseUrl + std::to_string(nLocalId) + ".png";
    } else {
      sLogoUrl = sBaseUrl + pGame->getUuid() + "/game.png";
    }
    
    jsonGame["logo"] = sLogoUrl;
    jsonGames.push_back(jsonGame);
  }

  jsonResponse["data"] = jsonGames;
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
