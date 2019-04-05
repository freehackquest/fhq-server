#include <cmd_handlers_games.h>
#include <utils_logger.h>
#include <runtasks.h>
#include <QJsonArray>
#include <QTemporaryDir>
#include <QDataStream>
#include <quazip.h>
#include <quazipfile.h>
#include <quazipfileinfo.h>
#include <iostream>
#include <employ_database.h>
#include <employ_settings.h>
#include <employ_images.h>
#include <employ_games.h>
#include <employ_notify.h>
#include <sha1_wrapper.h>
#include <QFile>
#include <fstream>

/*********************************************
 * Create Game
**********************************************/


CmdHandlerGameCreate::CmdHandlerGameCreate()
   : CmdHandlerBase("game_create", "Create the game"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    addInputDef("uuid").uuid_().required().description("Global Identificator of the Game");
    addInputDef("name").string_().required().description("Name of the Game");
    addInputDef("description").string_().required().description("Description of the Game");
    addInputDef("state").string_().required().description("State of the game");
    addInputDef("form").string_().required().description("Form of the game");
    addInputDef("type").string_().required().description("Type of the game");
    addInputDef("date_start").string_().required().description("Date start");
    addInputDef("date_stop").string_().required().description("Date stop");
    addInputDef("date_restart").string_().required().description("Date restart");
    addInputDef("organizators").string_().required().description("Organizators");
}


// ---------------------------------------------------------------------

void CmdHandlerGameCreate::handle(ModelRequest *pRequest){
    EmployGames *pEmployGames = findEmploy<EmployGames>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();

    ModelGame pModelGame;
    pModelGame.fillFrom(jsonRequest);

    std::string sError = "";
    EmployResult nResult = pEmployGames->addGame(pModelGame, sError);
    switch (nResult) {

        case EmployResult::DATABASE_ERROR: {
            pRequest->sendMessageError(cmd(), Error(500, sError));
            break;
        }

        case EmployResult::ALREADY_EXISTS: {
            pRequest->sendMessageError(cmd(), Error(403, "Game already exists with this uuid"));
            break;
        }

        case EmployResult::ERROR_NAME_IS_EMPTY: {
            pRequest->sendMessageError(cmd(), Error(400, "Game has empty name"));
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
            pRequest->sendMessageError(cmd(), Error(500, "Server error"));
        }
    }
}

/*********************************************
 * Delete Game
**********************************************/

CmdHandlerGameDelete::CmdHandlerGameDelete()
    : CmdHandlerBase("game_delete", "Remove game and all quests"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    addInputDef("uuid").uuid_().required().description("Global Identificator of the Game");
    addInputDef("admin_password").string_().required().description("Admin Password");
}

// ---------------------------------------------------------------------

void CmdHandlerGameDelete::handle(ModelRequest *pRequest){
	EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
	
    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    QString sUuid = jsonRequest["uuid"].toString().trimmed();
    QString sAdminPassword = jsonRequest["admin_password"].toString();

    IUserToken *pUserToken = pRequest->userToken();
    int nUserID = pUserToken->userid();

    QSqlDatabase db = *(pDatabase->database());

    // check admin password
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE id = :userid");
        query.bindValue(":userid", nUserID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }

        QString sPass = "";
        QString sEmail = "";

        if (query.next()) {
            QSqlRecord record = query.record();
            sEmail = record.value("email").toString();
            sPass = record.value("pass").toString();
        }else{
            pRequest->sendMessageError(cmd(), Errors::NotFound("user"));
            return;
        }

        QString sAdminPasswordHash = sEmail.toUpper() + sAdminPassword;
        std::string _sAdminPasswordHash = sha1::calc_string_to_hex(sAdminPasswordHash.toStdString());
        sAdminPasswordHash = QString(_sAdminPasswordHash.c_str());

        if(sAdminPasswordHash != sPass){
            pRequest->sendMessageError(cmd(), Error(401, "Wrong password"));
            return;
        }
    }

    int nGameID = 0;
    std::string sName = "";

    // check existing game
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE uuid = :uuid");
        query.bindValue(":uuid", sUuid);

        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }

        if(!query.next()) {
            pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
            return;
        }else{
            QSqlRecord record = query.record();
            nGameID = record.value("id").toInt();
            sName = record.value("title").toString().toStdString();
        }
    }

    EmployGames *pEmployGames = findEmploy<EmployGames>();
    pEmployGames->removeGame(sUuid.toStdString()); // TODO just removed from cache

    // delete from users_games
    {
        QSqlQuery query_del(db);
        query_del.prepare("DELETE FROM users_games WHERE gameid = :gameid");
        query_del.bindValue(":gameid", nGameID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text().toStdString()));
            return;
        }
    }

    // delete from users_quests_answers
    {
        QSqlQuery query_del(db);
        query_del.prepare("DELETE FROM users_quests_answers WHERE questid IN (SELECT idquest FROM quest q WHERE q.gameid = :gameid)");
        query_del.bindValue(":gameid", nGameID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text().toStdString()));
            return;
        }
    }

    // delete from users_quests
    {
        QSqlQuery query_del(db);
        query_del.prepare("DELETE FROM users_quests WHERE questid IN (SELECT idquest FROM quest q WHERE q.gameid = :gameid)");
        query_del.bindValue(":gameid", nGameID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text().toStdString()));
            return;
        }
    }

    // delete from quests
    {
        QSqlQuery query_del(db);
        query_del.prepare("DELETE FROM quest WHERE gameid = :gameid");
        query_del.bindValue(":gameid", nGameID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text().toStdString()));
            return;
        }
    }

    // delete from games
    {
        QSqlQuery query_del(db);
        query_del.prepare("DELETE FROM games WHERE id = :gameid");
        query_del.bindValue(":gameid", nGameID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text().toStdString()));
            return;
        }
    }

    // delete game logo if exists
    std::string sGameLogoFilename = "";
    {
        EmploySettings *pSettings = findEmploy<EmploySettings>();
        QString sBasePath = pSettings->getSettString(EmploySettings::SERVER_FOLDER_PUBLIC);
        sGameLogoFilename = sBasePath.toStdString() + "games/" + std::to_string(nGameID) + ".png";
        if( remove( sGameLogoFilename.c_str() ) != 0 ){
            Log::err(TAG, "Could not delete file " + sGameLogoFilename);
        }
    }

    EmployNotify *pNotify = findEmploy<EmployNotify>();
    ModelNotification notification("warning", "games", "Removed [game#" + sUuid.toStdString() + "] " + sName);
    pNotify->sendNotification(notification);
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


/*********************************************
 * Export Game
**********************************************/


CmdHandlerGameExport::CmdHandlerGameExport()
    : CmdHandlerBase("game_export", "Export the game") {

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    addInputDef("uuid").uuid_().required().description("Global Identificator of the Game");
}

// ---------------------------------------------------------------------

void CmdHandlerGameExport::handle(ModelRequest *pRequest){
    // EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    EmployGames *pEmployGames = findEmploy<EmployGames>();
	
    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    QString sUuid = jsonRequest["uuid"].toString();

    ModelGame modelGame;
    if(!pEmployGames->findGame(sUuid.toStdString(), modelGame)){
        pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
        return;
    }

    // find logo for game
    QString sGameLogoFilename = "";
    {
        EmploySettings *pSettings = findEmploy<EmploySettings>();
        QString sBasePath = pSettings->getSettString("server_folder_public") + "games/";
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
        if(fileLogo.exists() && fileLogo.open(QIODevice::ReadOnly)){
            export_zipfile.open(QIODevice::WriteOnly, QuaZipNewInfo(sUuid.toLower() + ".png"));
            // After .toString(), you should specify a text codec to use to encode the
            // string data into the (binary) file. Here, I use UTF-8:
            QByteArray baLogo = fileLogo.readAll();
            export_zipfile.write(baLogo);
            export_zipfile.close();
        }else{
            Log::warn(TAG, QString("Logo not found " + sGameLogoFilename).toStdString());
        }
    }

    // pack json file
    {
        export_zipfile.open(QIODevice::WriteOnly, QuaZipNewInfo(sUuid.toLower() + ".json"));
        std::string message = modelGame.toJson().dump();
        export_zipfile.write(QString(message.c_str()).toUtf8());
        export_zipfile.close();
    }
    zip.close();

    // preapre zip base64
    {
        QFile fileZip(tmpZipFile);
        if (!fileZip.open(QIODevice::ReadOnly)){
            pRequest->sendMessageError(cmd(), Error(500, "Could not open zip file"));
            return;
        }
        QByteArray baZip = fileZip.readAll();
        nlohmann::json jsonData;
        jsonData["zipfile_base64"] = QString(baZip.toBase64()).toStdString();
        jsonData["zipfile_name"] = QString("game_" + sUuid.toLower() + ".zip").toStdString();
        jsonResponse["data"] = jsonData;
        fileZip.close();
        fileZip.remove();
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * Import Game
**********************************************/


CmdHandlerGameImport::CmdHandlerGameImport()
    : CmdHandlerBase("game_import", "Import game") {

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    addInputDef("uuid").uuid_().required().description("Global Identificator of the Game");
}

// ---------------------------------------------------------------------

void CmdHandlerGameImport::handle(ModelRequest *pRequest){
    // nlohmann::json jsonRequest = pRequest->jsonRequest();
    // nlohmann::json jsonResponse;

    pRequest->sendMessageError(cmd(), Errors::NotImplementedYet());
    return;

    // TODO
    // pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


/*********************************************
 * Info Game
**********************************************/


CmdHandlerGameInfo::CmdHandlerGameInfo()
    : CmdHandlerBase("game_info", "Return game info"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    addInputDef("uuid").uuid_().required().description("Global Identificator of the Game");
}

// ---------------------------------------------------------------------

void CmdHandlerGameInfo::handle(ModelRequest *pRequest){
    EmployGames *pEmployGames = findEmploy<EmployGames>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    QString sUuid = jsonRequest["uuid"].toString().trimmed();

    ModelGame modelGame;
    if(!pEmployGames->findGame(sUuid.toStdString(), modelGame)){
        pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
        return;
    }

    jsonResponse["data"] = modelGame.toJson();
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * Update Game
**********************************************/

CmdHandlerGameUpdate::CmdHandlerGameUpdate()
    : CmdHandlerBase("game_update", "Update game info"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    addInputDef("uuid").uuid_().required().description("Global Identificator of the Game");
    addInputDef("name").string_().optional().description("Name of the Game");
    addInputDef("description").string_().optional().description("Description of the Game");
    addInputDef("state").string_().optional().description("State of the game");
    addInputDef("form").string_().optional().description("Form of the game");
    addInputDef("type").string_().optional().description("Type of the game");
    addInputDef("date_start").string_().optional().description("Date start");
    addInputDef("date_stop").string_().optional().description("Date stop");
    addInputDef("date_restart").string_().optional().description("Date restart");
    addInputDef("organizators").string_().optional().description("Organizators");
}

// ---------------------------------------------------------------------

void CmdHandlerGameUpdate::handle(ModelRequest *pRequest){
    EmployGames *pEmployGames = findEmploy<EmployGames>();

    ModelGame updatedModelGame;
    updatedModelGame.fillFrom(pRequest->jsonRequest());

    ModelGame modelGame;
    if(!pEmployGames->findGame(updatedModelGame.uuid(), modelGame)){
        pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
        return;
    }

    updatedModelGame.copy(modelGame);
    updatedModelGame.fillFrom(pRequest->jsonRequest()); // will be replaced values for existing fields

    std::string sError = "";
    EmployResult nResult = pEmployGames->updateGame(updatedModelGame, sError);

    switch (nResult) {

        case EmployResult::DATABASE_ERROR: {
            pRequest->sendMessageError(cmd(), Error(500, sError));
            break;
        }
        

        case EmployResult::GAME_NOT_FOUND: {
            pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
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
            pRequest->sendMessageError(cmd(), Error(500, "Server error"));
        }
    }
}


/*********************************************
 * Update Logo Game
**********************************************/


CmdHandlerGameUpdateLogo::CmdHandlerGameUpdateLogo()
    : CmdHandlerBase("game_update_logo", "Update game logo"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    addInputDef("uuid").uuid_().required().description("Global Identificator of the Game");
    addInputDef("image_png_base64").string_().required().description("Image PNG in Base64");
}

// ---------------------------------------------------------------------

void CmdHandlerGameUpdateLogo::handle(ModelRequest *pRequest){
    EmployGames *pEmployGames = findEmploy<EmployGames>();

    ModelGame modelGame;
    modelGame.fillFrom(pRequest->jsonRequest());
    if(!pEmployGames->findGame(modelGame.uuid(), modelGame)){
        pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
        return;
    }

    int nGameID = modelGame.localId();

    nlohmann::json jsonResponse;

    EmploySettings *pSettings = findEmploy<EmploySettings>();
    EmployImages *pImages = findEmploy<EmployImages>();

    QString sBasePath = pSettings->getSettString("server_folder_public") + "games/";

    QString sFilename = sBasePath + QString::number(nGameID) + ".png";

    QByteArray baImagePNGBase64;

    // TODO can be crash
    nlohmann::json jsonRequest = pRequest->jsonRequest();
    QString sImagePngBase64 = QString::fromStdString(jsonRequest["image_png_base64"]);
    baImagePNGBase64.append(sImagePngBase64);
    // TODO replace decode base64 to std
    QByteArray baImagePNG = QByteArray::fromBase64(baImagePNGBase64); // .fromBase64(baImagePNGBase64);

    if (baImagePNG.size() == 0) {
        pRequest->sendMessageError(cmd(), Error(400, "Could not decode base64"));
        return;
    }

    FILE * pFile;
    char *sSIF = new char[L_tmpnam];
    strcpy(sSIF, "/tmp/fileXXXXXX");
    int fd = mkstemp(sSIF);
    std::string sSourceImageFile(sSIF);
    std::cout << "temporary file name: " << sSourceImageFile << '\n';
    
    int nLen = baImagePNG.size();
    pFile = fdopen(fd, "wb");
    fwrite (baImagePNG.constData(), sizeof(char), nLen, pFile);
    fclose (pFile);

    std::string targetImageFile = sFilename.toStdString();
    // Log::info(TAG, "targetImageFile " + targetImageFile);
    if(!pImages->doThumbnailImagePng(sSourceImageFile, targetImageFile, 100, 100)){
        pRequest->sendMessageError(cmd(), Error(400, "Could not decode bytearray to png"));
        // cleanup - redesign try finnaly
        remove( sSourceImageFile.c_str());
        return;
    }

    if( remove( sSourceImageFile.c_str() ) != 0 ){
        Log::err(TAG, "Could not delete file " + sSourceImageFile);
    }

    if (FILE *file = fopen(targetImageFile.c_str(), "r")) {
        fclose(file);
        pRequest->sendMessageSuccess(cmd(), jsonResponse);
    } else {
        pRequest->sendMessageError(cmd(), Error(500, "Problem with creation file"));
    }
}


/*********************************************
 * List of Games
**********************************************/


CmdHandlerGames::CmdHandlerGames()
    : CmdHandlerBase("games", "Method returned list of games"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    // addInputDef("filter_text").string_().optional().description("Filter by user email or nick");
    // addInputDef("filter_role").string_().optional().description("Filter by user role");
}

// ---------------------------------------------------------------------

void CmdHandlerGames::handle(ModelRequest *pRequest){
	EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
	
    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QString base_url = pSettings->getSettString("server_folder_public_url") + "games/";

    auto jsonGames = nlohmann::json::array();

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    query.prepare("SELECT * FROM games ORDER BY games.date_start");

    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
        return;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        nlohmann::json jsonGame;
        int nGameID = record.value("id").toInt();
        jsonGame["local_id"] = nGameID; // deprecated
        jsonGame["uuid"] = record.value("uuid").toString().toStdString();
        jsonGame["name"] = record.value("title").toString().toStdString();
        jsonGame["type_game"] = record.value("type_game").toString().toStdString();
        jsonGame["date_start"] = record.value("date_start").toString().toStdString();
        jsonGame["date_stop"] = record.value("date_stop").toString().toStdString();
        jsonGame["date_restart"] = record.value("date_restart").toString().toStdString();
        jsonGame["description"] = record.value("description").toString().toStdString();
        jsonGame["state"] = record.value("state").toString().toStdString();
        jsonGame["form"] = record.value("form").toString().toStdString();
        jsonGame["logo"] = QString(base_url + QString::number(nGameID) + ".png").toStdString();
        jsonGame["organizators"] = record.value("organizators").toString().toStdString();
        jsonGame["maxscore"] = record.value("maxscore").toInt();
        jsonGames.push_back(jsonGame);
    }

    jsonResponse["data"] = jsonGames;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


