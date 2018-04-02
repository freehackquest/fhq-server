#include <cmd_handlers_games.h>
#include <log.h>
#include <runtasks.h>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QTemporaryDir>
#include <QDataStream>
#include <quazip.h>
#include <quazipfile.h>
#include <quazipfileinfo.h>
#include <log.h>
#include <iostream>
#include <employ_settings.h>
#include <QFile>
#include <QImage>

/*********************************************
 * Create Game
**********************************************/


CmdGameCreateHandler::CmdGameCreateHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
    m_vInputs.push_back(CmdInputDef("name").string_().required().description("Name of the Game"));
    m_vInputs.push_back(CmdInputDef("description").string_().required().description("Description of the Game"));
    m_vInputs.push_back(CmdInputDef("state").string_().required().description("State of the game"));
    m_vInputs.push_back(CmdInputDef("form").string_().required().description("Form of the game"));
    m_vInputs.push_back(CmdInputDef("type").string_().required().description("Type of the game"));
    m_vInputs.push_back(CmdInputDef("date_start").string_().required().description("Date start"));
    m_vInputs.push_back(CmdInputDef("date_stop").string_().required().description("Date stop"));
    m_vInputs.push_back(CmdInputDef("date_restart").string_().required().description("Date restart"));
    m_vInputs.push_back(CmdInputDef("organizators").string_().required().description("Organizators"));
}

// ---------------------------------------------------------------------

std::string CmdGameCreateHandler::cmd(){
    return "game_create";
}
// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGameCreateHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGameCreateHandler::inputs(){
    return m_vInputs;
}

std::string CmdGameCreateHandler::description(){
    return "Create the game";
}

// ---------------------------------------------------------------------

void CmdGameCreateHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());

    QString sUUID = jsonRequest["uuid"].toString().trimmed();
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE uuid = :uuid");
        query.bindValue(":uuid", sUUID);
        query.exec();
        if (query.next()) {
            pRequest->sendMessageError(cmd(), Error(403, "Game with uuid {" + sUUID + "} already exists"));
            return;
        }
    }

    QString sName = jsonRequest["name"].toString().trimmed();
    if(sName.length() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Name could not be empty"));
        return;
    }

    QString sDescription = jsonRequest["description"].toString().trimmed();
    QString sState = jsonRequest["state"].toString().trimmed();
    QString sForm = jsonRequest["form"].toString().trimmed();
    QString sType = jsonRequest["type"].toString().trimmed();
    QString sDateStart = jsonRequest["date_start"].toString().trimmed();
    QString sDateStop = jsonRequest["date_stop"].toString().trimmed();
    QString sDateRestart = jsonRequest["date_restart"].toString().trimmed();
    QString sOrganizators = jsonRequest["organizators"].toString().trimmed();

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO games("
        "		uuid,"
        "		title,"
        "		type_game,"
        "		date_create,"
        "		date_change,"
        "		date_start,"
        "		date_stop,"
        "		date_restart,"
        "		description,"
        "		organizators,"
        "		state,"
        "		form,"
        "		rules,"
        "		maxscore"
        "	)"
        "	VALUES("
        "		:uuid,"
        "		:title,"
        "		:type_game,"
        "		NOW(),"
        "		NOW(),"
        "		:date_start,"
        "		:date_stop,"
        "		:date_restart,"
        "		:description,"
        "		:organizators,"
        "		:state,"
        "		:form,"
        "		:rules,"
        "		:maxscore"
        "	)");
    query.bindValue(":uuid", sUUID);
    query.bindValue(":title", sName);
    query.bindValue(":type_game", sType);
    query.bindValue(":date_start", sDateStart);
    query.bindValue(":date_stop", sDateStop);
    query.bindValue(":date_restart", sDateRestart);
    query.bindValue(":description", sDescription);
    query.bindValue(":organizators", sOrganizators);
    query.bindValue(":state", sState);
    query.bindValue(":form", sForm);
    query.bindValue(":rules", "");
    query.bindValue(":maxscore", 0);

    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    int rowid = query.lastInsertId().toInt();
    jsonResponse["questid"] = QJsonValue(rowid);

    RunTasks::AddPublicEvents(pRequest->server(), "games", "New game #" + QString::number(rowid) + " " + sName);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * Delete Game
**********************************************/

CmdGameDeleteHandler::CmdGameDeleteHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
    m_vInputs.push_back(CmdInputDef("admin_password").string_().required().description("Admin Password"));
}

// ---------------------------------------------------------------------

std::string CmdGameDeleteHandler::cmd(){
    return "game_delete";
}
// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGameDeleteHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGameDeleteHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdGameDeleteHandler::description(){
    return "Remove game and all quests";
}

// ---------------------------------------------------------------------

void CmdGameDeleteHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QString sUuid = jsonRequest["uuid"].toString().trimmed();
    QString sAdminPassword = jsonRequest["admin_password"].toString();

    IUserToken *pUserToken = pRequest->userToken();
    int nUserID = pUserToken->userid();

    QSqlDatabase db = *(pRequest->server()->database());

    // check admin password
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE id = :userid");
        query.bindValue(":userid", nUserID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
        sAdminPasswordHash = QString("%1").arg(QString(QCryptographicHash::hash(sAdminPasswordHash.toUtf8(),QCryptographicHash::Sha1).toHex()));

        if(sAdminPasswordHash != sPass){
            pRequest->sendMessageError(cmd(), Error(401, "Wrong password"));
            return;
        }
    }

    int nGameID = 0;

    // check existing game
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE uuid = :uuid");
        query.bindValue(":uuid", sUuid);

        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }

        if(!query.next()) {
            pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
            return;
        }else{
            QSqlRecord record = query.record();
            nGameID = record.value("id").toInt();
        }
    }

    // delete from users_games
    {
        QSqlQuery query_del(db);
        query_del.prepare("DELETE FROM users_games WHERE gameid = :gameid");
        query_del.bindValue(":gameid", nGameID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_quests_answers
    {
        QSqlQuery query_del(db);
        query_del.prepare("DELETE FROM users_quests_answers WHERE questid IN (SELECT idquest FROM quest q WHERE q.gameid = :gameid)");
        query_del.bindValue(":gameid", nGameID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_quests
    {
        QSqlQuery query_del(db);
        query_del.prepare("DELETE FROM users_quests WHERE questid IN (SELECT idquest FROM quest q WHERE q.gameid = :gameid)");
        query_del.bindValue(":gameid", nGameID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from quests
    {
        QSqlQuery query_del(db);
        query_del.prepare("DELETE FROM quest WHERE gameid = :gameid");
        query_del.bindValue(":gameid", nGameID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from games
    {
        QSqlQuery query_del(db);
        query_del.prepare("DELETE FROM games WHERE id = :gameid");
        query_del.bindValue(":gameid", nGameID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


/*********************************************
 * Export Game
**********************************************/


CmdGameExportHandler::CmdGameExportHandler(){
    TAG = "CmdGameExportHandler";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
}

// ---------------------------------------------------------------------

std::string CmdGameExportHandler::cmd(){
    return "game_export";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGameExportHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGameExportHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdGameExportHandler::description(){
    return "Export the game";
}

// ---------------------------------------------------------------------

void CmdGameExportHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QString sUuid = jsonRequest["uuid"].toString();
    QSqlDatabase db = *(pRequest->server()->database());

    QSqlQuery query(db);
    query.prepare("SELECT * FROM games WHERE uuid = :gameuuid");
    query.bindValue(":gameuuid", sUuid);

    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    QJsonObject jsonGame;
    int nGameID = 0;
    if (query.next()) {
        QSqlRecord record = query.record();
        nGameID  = record.value("id").toInt();
        jsonGame["uuid"] = record.value("uuid").toString();
        jsonGame["title"] = record.value("title").toString();
        jsonGame["type_game"] = record.value("type_game").toString();
        jsonGame["date_start"] = record.value("date_start").toString();
        jsonGame["date_stop"] = record.value("date_stop").toString();
        jsonGame["date_restart"] = record.value("date_restart").toString();
        jsonGame["description"] = record.value("description").toString();
        jsonGame["state"] = record.value("state").toString();
        jsonGame["form"] = record.value("form").toString();
        jsonGame["organizators"] = record.value("organizators").toString();
        jsonGame["maxscore"] = record.value("maxscore").toInt();
    } else {
        pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
        return;
    }

    // find logo for game
    QString sGameLogoFilename = "";
    {
        EmploySettings *pSettings = findEmploy<EmploySettings>();
        QString sBasePath = pSettings->getSettString("server_folder_games");
        sGameLogoFilename = sBasePath + QString::number(nGameID) + ".png";
    }

    QString tmpDir = QDir::tempPath();
    quint64 ts = QDateTime::currentMSecsSinceEpoch();
    QString tmpZipFile = tmpDir + "/freehackquest-backend-game_" + QString::number(ts) + ".zip";

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
            Log::warn(TAG, "Logo not found " + sGameLogoFilename);
        }
    }

    // pack json file
    {
        export_zipfile.open(QIODevice::WriteOnly, QuaZipNewInfo(sUuid.toLower() + ".json"));
        QJsonDocument doc(jsonGame);
        QString message = doc.toJson(QJsonDocument::Compact);
        export_zipfile.write(message.toUtf8());
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
        QJsonObject jsonData;
        jsonData["zipfile_base64"] = QString(baZip.toBase64());
        jsonData["zipfile_name"] = "game_" + sUuid.toLower() + ".zip";
        jsonResponse["data"] = jsonData;
        fileZip.close();
        fileZip.remove();
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * Import Game
**********************************************/


CmdGameImportHandler::CmdGameImportHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
}

// ---------------------------------------------------------------------

std::string CmdGameImportHandler::cmd(){
    return "game_import";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGameImportHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGameImportHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdGameImportHandler::description(){
    return "Import game";
}

// ---------------------------------------------------------------------

void CmdGameImportHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    pRequest->sendMessageError(cmd(), Errors::NotImplementedYet());
    return;

    // TODO
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


/*********************************************
 * Info Game
**********************************************/


CmdGameInfoHandler::CmdGameInfoHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
}

// ---------------------------------------------------------------------

std::string CmdGameInfoHandler::cmd(){
    return "game_info";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGameInfoHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGameInfoHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdGameInfoHandler::description(){
    return "Return game info";
}

// ---------------------------------------------------------------------

void CmdGameInfoHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QJsonObject data;

    QString sUuid = jsonRequest["uuid"].toString().trimmed();

    QSqlDatabase db = *(pRequest->server()->database());

    QSqlQuery query(db);
    query.prepare("SELECT * FROM games WHERE uuid = :gameuuid");
    query.bindValue(":gameuuid", sUuid);

    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    if (query.next()) {
        QSqlRecord record = query.record();
        data["uuid"] = record.value("uuid").toString();
        data["title"] = record.value("title").toString();
        data["type_game"] = record.value("type_game").toString();
        data["date_start"] = record.value("date_start").toString();
        data["date_stop"] = record.value("date_stop").toString();
        data["date_restart"] = record.value("date_restart").toString();
        data["description"] = record.value("description").toString();
        data["state"] = record.value("state").toString();
        data["form"] = record.value("form").toString();
        data["organizators"] = record.value("organizators").toString();
        data["maxscore"] = record.value("maxscore").toInt();
    } else {
        pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
        return;
    }

    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * Update Game
**********************************************/


CmdGameUpdateHandler::CmdGameUpdateHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
    m_vInputs.push_back(CmdInputDef("name").string_().required().description("Name of the Game"));
    m_vInputs.push_back(CmdInputDef("description").string_().required().description("Description of the Game"));
    m_vInputs.push_back(CmdInputDef("state").string_().required().description("State of the game"));
    m_vInputs.push_back(CmdInputDef("form").string_().required().description("Form of the game"));
    m_vInputs.push_back(CmdInputDef("type").string_().required().description("Type of the game"));
    m_vInputs.push_back(CmdInputDef("date_start").string_().required().description("Date start"));
    m_vInputs.push_back(CmdInputDef("date_stop").string_().required().description("Date stop"));
    m_vInputs.push_back(CmdInputDef("date_restart").string_().required().description("Date restart"));
    m_vInputs.push_back(CmdInputDef("organizators").string_().required().description("Organizators"));
}

// ---------------------------------------------------------------------

std::string CmdGameUpdateHandler::cmd(){
    return "game_update";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGameUpdateHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGameUpdateHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdGameUpdateHandler::description(){
    return "Create the quest";
}

// ---------------------------------------------------------------------

void CmdGameUpdateHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    // QJsonObject data;

    QString sUuid = jsonRequest["uuid"].toString().trimmed();
    QString sName = jsonRequest["name"].toString().trimmed();
    QString sDescription = jsonRequest["description"].toString().trimmed();
    QString sState = jsonRequest["state"].toString().trimmed();
    QString sForm = jsonRequest["form"].toString().trimmed();
    QString sType = jsonRequest["type"].toString().trimmed();
    QString sDateStart = jsonRequest["date_start"].toString().trimmed();
    QString sDateStop = jsonRequest["date_stop"].toString().trimmed();
    QString sDateRestart = jsonRequest["date_restart"].toString().trimmed();
    QString sOrganizators = jsonRequest["organizators"].toString().trimmed();


    // original values
    QString sNamePrev = "";
    QString sTypePrev = "";
    QString sDescriptionPrev = "";
    QString sStatePrev = "";
    QString sFormPrev = "";
    QString sOrganizatorsPrev = "";
    QString sDateStartPrev = "";
    QString sDateStopPrev = "";
    QString sDateRestartPrev = "";

    QSqlDatabase db = *(pRequest->server()->database());
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE uuid = :gameuuid");
        query.bindValue(":gameuuid", sUuid);

        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }

        if (query.next()) {
            QSqlRecord record = query.record();
            sNamePrev = record.value("title").toString();
            sTypePrev = record.value("type_game").toString();
            sDescriptionPrev = record.value("description").toString();
            sStatePrev = record.value("state").toString();
            sFormPrev = record.value("form").toString();
            sOrganizatorsPrev = record.value("organizators").toString();
            sDateStartPrev = record.value("date_start").toString();
            sDateStopPrev = record.value("date_stop").toString();
            sDateRestartPrev = record.value("date_restart").toString();
        } else {
            pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
            return;
        }
    }

    if(sNamePrev != sName){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET title = :name WHERE uuid = :gameuuid");
        query.bindValue(":name", sName);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pRequest->server(), "quests", "Updated name of game {" + sUuid + "} from [" + sNamePrev + "] to [" + sName + "]");
        sNamePrev = sName;
    }

    if(sTypePrev != sType){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET type_game = :type_game WHERE uuid = :gameuuid");
        query.bindValue(":type_game", sType);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pRequest->server(), "quests", "Updated type of game {" + sUuid + "} from [" + sTypePrev + "] to [" + sType + "]");
        sTypePrev = sType;
    }

    if(sDescriptionPrev != sDescription){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET description = :description WHERE uuid = :gameuuid");
        query.bindValue(":description", sDescription);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pRequest->server(), "quests", "Updated description of the game {" + sUuid + "}");
        sDescriptionPrev = sDescription;
    }

    if(sStatePrev != sState){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `state` = :state WHERE uuid = :gameuuid");
        query.bindValue(":state", sState);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pRequest->server(), "quests", "Updated state of game {" + sUuid + "} from [" + sStatePrev + "] to [" + sState + "]");
        sStatePrev = sState;
    }

    if(sFormPrev != sForm){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `form` = :form WHERE uuid = :gameuuid");
        query.bindValue(":form", sForm);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pRequest->server(), "quests", "Updated form of game {" + sUuid + "} from [" + sFormPrev + "] to [" + sForm + "]");
        sFormPrev = sForm;
    }

    if(sOrganizatorsPrev != sOrganizators){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `organizators` = :organizators WHERE uuid = :gameuuid");
        query.bindValue(":organizators", sOrganizators);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pRequest->server(), "quests", "Updated organizators of game {" + sUuid + "}");
        sOrganizatorsPrev = sOrganizators;
    }

    if(sDateStartPrev != sDateStart){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `date_start` = :date_start WHERE uuid = :gameuuid");
        query.bindValue(":date_start", sDateStart);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pRequest->server(), "quests", "Updated date start of game {" + sUuid + "} from [" + sDateStartPrev + "] to [" + sDateStart + "]");
        sDateStartPrev = sDateStart;
    }

    if(sDateStopPrev != sDateStop){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `date_stop` = :date_stop WHERE uuid = :gameuuid");
        query.bindValue(":date_stop", sDateStop);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pRequest->server(), "quests", "Updated date stop of game {" + sUuid + "} from [" + sDateStopPrev + "] to [" + sDateStop + "]");
        sDateStopPrev = sDateStop;
    }

    if(sDateRestartPrev != sDateRestart){
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `date_restart` = :date_restart WHERE uuid = :gameuuid");
        query.bindValue(":date_restart", sDateRestart);
        query.bindValue(":gameuuid", sUuid);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        RunTasks::AddPublicEvents(pRequest->server(), "quests", "Updated date stop of game {" + sUuid + "} from [" + sDateRestartPrev + "] to [" + sDateRestart + "]");
        sDateRestartPrev = sDateRestart;
    }

    // jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}




/*********************************************
 * Update Logo Game
**********************************************/


CmdGameUpdateLogoHandler::CmdGameUpdateLogoHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("gameid").integer_().required().description("GameID"));
    m_vInputs.push_back(CmdInputDef("image_png_base64").string_().required().description("Image PNG in Base64"));
}

// ---------------------------------------------------------------------

std::string CmdGameUpdateLogoHandler::cmd(){
    return "game_update_logo";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGameUpdateLogoHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGameUpdateLogoHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdGameUpdateLogoHandler::description(){
    return "Update game logo";
}

// ---------------------------------------------------------------------

void CmdGameUpdateLogoHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;


    int nGameID = jsonRequest["gameid"].toInt();

    // TODO check existing game

    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QString sBasePath = pSettings->getSettString("server_folder_games");

    QString sFilename = sBasePath + QString::number(nGameID) + ".png";

    QByteArray baImagePNGBase64;

    QString sImagePngBase64 = jsonRequest["image_png_base64"].toString();
    baImagePNGBase64.append(sImagePngBase64);
    QByteArray baImagePNG = QByteArray::fromBase64(baImagePNGBase64); // .fromBase64(baImagePNGBase64);

    if(baImagePNG.size() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Could not decode base64"));
        return;
    }

    QImage img = QImage::fromData(baImagePNG,"PNG");
    if(img.height() == 0 && img.width() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Could not decode bytearray to png"));
        return;
    }

    // TODO resize icon of game

    QFile file(sFilename);

    if(file.exists()){
        if(!file.remove()){
            pRequest->sendMessageError(cmd(), Error(403, "Could not remove old file"));
            return;
        }
    }
    if(!img.save(sFilename)){
        pRequest->sendMessageError(cmd(), Error(403, "Could not save new file"));
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


/*********************************************
 * List of Games
**********************************************/


CmdGamesHandler::CmdGamesHandler(){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    // m_vInputs.push_back(CmdInputDef("filter_text").string_().optional().description("Filter by user email or nick"));
    // m_vInputs.push_back(CmdInputDef("filter_role").string_().optional().description("Filter by user role"));
}

// ---------------------------------------------------------------------

std::string CmdGamesHandler::cmd(){
    return "games";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGamesHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGamesHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdGamesHandler::description(){
    return "Method returned list of games";
}

// ---------------------------------------------------------------------

void CmdGamesHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;


    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QString base_url = pSettings->getSettString("server_folder_games_url");

    QJsonArray games;
    QSqlDatabase db = *(pRequest->server()->database());

    QSqlQuery query(db);
    query.prepare("SELECT * FROM games ORDER BY games.date_start");

    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        QJsonObject game;
        int nGameID = record.value("id").toInt();
        game["id"] = nGameID;
        game["uuid"] = record.value("uuid").toString();
        game["title"] = record.value("title").toString();
        game["type_game"] = record.value("type_game").toString();
        game["date_start"] = record.value("date_start").toString();
        game["date_stop"] = record.value("date_stop").toString();
        game["date_restart"] = record.value("date_restart").toString();
        game["description"] = record.value("description").toString();
        game["state"] = record.value("state").toString();
        game["form"] = record.value("form").toString();
        game["logo"] = base_url + QString::number(nGameID) + ".png";
        game["organizators"] = record.value("organizators").toString();
        game["maxscore"] = record.value("maxscore").toInt();
        games.push_back(game);
    }

    jsonResponse["data"] = games;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


