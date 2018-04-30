#include <cmd_handlers_users.h>
#include <log.h>
#include <runtasks.h>
#include <log.h>
#include <iostream>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <employ_scoreboard.h>
#include <QtCore>
#include <model_usertoken.h>
#include <QCryptographicHash>
#include <QUuid>

/*********************************************
 * This handler will be return scoreboard of user
**********************************************/

CmdHandlerUsersScoreboard::CmdHandlerUsersScoreboard()
	: CmdHandlerBase("scoreboard", "Method return scoreboard"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

	// validation and description input fields
	m_vInputs.push_back(CmdInputDef("page").required().integer_().description("Number of page"));
	m_vInputs.push_back(CmdInputDef("onpage").required().integer_().description("How much rows in one page"));
}

// ---------------------------------------------------------------------

void CmdHandlerUsersScoreboard::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    int nPage = jsonRequest["page"].toInt();
    jsonResponse["page"] = nPage;
	
    int nOnPage = jsonRequest["onpage"].toInt();
	if(nOnPage > 50){
        pRequest->sendMessageError(cmd(), Errors::OnPageCouldNotBeMoreThen50());
	}
    jsonResponse["onpage"] = nOnPage;

	QStringList filters;
	QMap<QString,QString> filter_values;
	
    if(jsonRequest.contains("user")){
        QString user = jsonRequest["user"].toString().trimmed();
		filters << "(u.nick like :nick)";
		filter_values[":nick"] = "%" + user + "%";
	}

	filters << "(rating > 0)";

	QString where = filters.join(" AND "); 
	if(where.length() > 0){
		where = "WHERE " + where;
	}

    EmployScoreboard *pScoreboard = findEmploy<EmployScoreboard>();
    pScoreboard->loadSync();

    jsonResponse["count"] = pScoreboard->count();
    jsonResponse["data"] = pScoreboard->toJson();

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * This handler will be return json map users
**********************************************/

CmdHandlerGetMap::CmdHandlerGetMap()
    : CmdHandlerBase("getmap", "Returned coordinate list"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void CmdHandlerGetMap::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QJsonArray coords;
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) as cnt, latitude, longitude FROM `users` GROUP BY latitude, longitude");
    query.exec();
    while (query.next()) {
        QSqlRecord record = query.record();
        double lat = record.value("latitude").toDouble();
        double lon = record.value("longitude").toDouble();
        int count = record.value("cnt").toInt();
        if(lat == 0 && lon == 0){
            continue;
        }
        QJsonObject item;
        item["lat"] = lat;
        item["lng"] = lon;
        item["count"] = count;
        coords.push_back(item);
    }

    jsonResponse["data"] = coords;
    jsonResponse["google_map_api_key"] = pSettings->getSettString("google_map_api_key");
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * User login
**********************************************/

CmdHandlerLogin::CmdHandlerLogin()
    : CmdHandlerBase("login", "Method for login"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(false);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("email").required().string_().description("E-mail"));
    m_vInputs.push_back(CmdInputDef("password").required().string_().description("Password"));
}

// ---------------------------------------------------------------------

void CmdHandlerLogin::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QString email = jsonRequest["email"].toString();
    QString password = jsonRequest["password"].toString();

    QString password_sha1 = email.toUpper() + password;

    password_sha1 = QString("%1").arg(QString(QCryptographicHash::hash(password_sha1.toUtf8(),QCryptographicHash::Sha1).toHex()));

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE email = :email AND pass = :pass");
    query.bindValue(":email", email);
    query.bindValue(":pass", password_sha1);

    if(!query.exec()){
        Log::err(TAG, query.lastError().text().toStdString());
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if (query.next()) {
        QSqlRecord record = query.record();

        int userid = record.value("id").toInt();
        QString email = record.value("email").toString();
        QString nick = record.value("nick").toString();
        QString role = record.value("role").toString();

        QJsonObject user;
        user["id"] = QString::number(userid);
        user["email"] = email;
        user["nick"] = nick;
        user["role"] = role;

        QJsonObject user_token;
        user_token["user"] = user;

        QJsonDocument doc(user_token);
        QString data = doc.toJson(QJsonDocument::Compact);

        QString token = QUuid::createUuid().toString();
        token = token.mid(1,token.length()-2);
        token = token.toUpper();

        QSqlQuery query_token(db);
        query_token.prepare("INSERT INTO users_tokens (userid, token, status, data, start_date, end_date) VALUES(:userid, :token, :status, :data, NOW(), NOW() + INTERVAL 1 DAY)");
        query_token.bindValue(":userid", userid);
        query_token.bindValue(":token", token);
        query_token.bindValue(":status", "active");
        query_token.bindValue(":data", data);

        if(!query_token.exec()){
            Log::err(TAG, query_token.lastError().text().toStdString());
            pRequest->sendMessageError(cmd(), Error(500, query_token.lastError().text()));
            return;
        }

        jsonResponse["token"] = token;
        jsonResponse["user"] = user;

        pRequest->server()->setUserToken(pRequest->client(), new ModelUserToken(user_token));

        // update user location
        QString lastip = pRequest->client()->peerAddress().toString();
        RunTasks::UpdateUserLocation(userid, lastip);

    }else{
        Log::err(TAG, "Invalid login or password");
        pRequest->sendMessageError(cmd(), Error(401, "Invalid login or password"));
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * User registration
**********************************************/

CmdHandlerRegistration::CmdHandlerRegistration()
    : CmdHandlerBase("registration", "Method for registration"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(false);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("email").required().email_().description("E-mail"));
    m_vInputs.push_back(CmdInputDef("university").required().string_().description("University"));
}

// ---------------------------------------------------------------------

void CmdHandlerRegistration::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    //EmploySettings *pSettings = findEmploy<EmploySettings>();

    QString sEmail = jsonRequest["email"].toString();

    QString sUniversity = jsonRequest["university"].toString();

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE email = :email");
    query.bindValue(":email", sEmail);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text().toStdString());
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if (query.next()) {
        Log::err(TAG, "User already exists " + sEmail.toStdString());
        pRequest->sendMessageError(cmd(), Error(403, "This email already exists"));
        return;
    }

    // // generate random password
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = 12; // assuming you want random strings of 12 characters
    QString sPassword;
    for(int i=0; i<randomStringLength; ++i){
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        sPassword.append(nextChar);
    }

    QString sPassword_sha1 = sEmail.toUpper() + sPassword;
    sPassword_sha1 = QString("%1").arg(QString(QCryptographicHash::hash(sPassword_sha1.toUtf8(),QCryptographicHash::Sha1).toHex()));

    // generate random nick
    const QString possibleCharacters2("ABCDEFH0123456789");
    const int randomStringLength2 = 8; // assuming you want random strings of 12 characters
    QString sNick;
    for(int i=0; i<randomStringLength2; ++i){
        int index = qrand() % possibleCharacters2.length();
        QChar nextChar = possibleCharacters2.at(index);
        sNick.append(nextChar);
    }
    sNick = "hacker-" + sNick;

    QSqlQuery query_insert(db);
    query_insert.prepare(""
                         "INSERT INTO users ("
                         "   uuid, "
                         "   email, "
                         "   pass, "
                         "   role, "
                         "   nick,"
                         "   logo,"
                         "   dt_create,"
                         "   dt_last_login,"
                         "   last_ip,"
                         "   status,"
                         "   country,"
                         "   region,"
                         "   city,"
                         "   university,"
                         "   latitude,"
                         "   longitude,"
                         "   rating,"
                         "   about)"
                         "VALUES("
                         "   :uuid, "
                         "   :email, "
                         "   :pass, "
                         "   :role, "
                         "   :nick,"
                         "   :logo,"
                         "   NOW(),"
                         "   NOW(),"
                         "   :last_ip,"
                         "   :status,"
                         "   :country,"
                         "   :region,"
                         "   :city,"
                         "   :university,"
                         "   :latitude,"
                         "   :longitude,"
                         "   :rating,"
                         "   :about);"
    );

    QString sLastIP = pRequest->client()->peerAddress().toString();

    QString sUuid = QUuid::createUuid().toString();
    sUuid = sUuid.mid(1,sUuid.length()-2);
    sUuid = sUuid.toUpper();

    query_insert.bindValue(":uuid", sUuid);
    query_insert.bindValue(":email", sEmail);
    query_insert.bindValue(":pass", sPassword_sha1);
    query_insert.bindValue(":role", "user");
    query_insert.bindValue(":nick", sNick);
    query_insert.bindValue(":logo", "files/users/0.png");
    query_insert.bindValue(":last_ip", "");
    query_insert.bindValue(":status", "activated");
    query_insert.bindValue(":country", "");
    query_insert.bindValue(":region", "");
    query_insert.bindValue(":city", "");
    query_insert.bindValue(":university", sUniversity);
    query_insert.bindValue(":latitude", 0);
    query_insert.bindValue(":longitude", 0);
    query_insert.bindValue(":rating", 0);
    query_insert.bindValue(":about", "");

    if(!query_insert.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query_insert.lastError().text()));
        return;
    }

    int nUserID = query_insert.lastInsertId().toInt();

    RunTasks::AddPublicEvents("users", "New user #" + QString::number(nUserID) + "  " + sNick);

    QString sSubject = "Registration on FreeHackQuest";
    QString sContext = "Welcome to FreeHackQuest 2017!\n"
                       "You login: " + sEmail + "\n"
                       "You password: " + sPassword + "\n";

    RunTasks::MailSend(pRequest->server(), sEmail, sSubject, sContext);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
    RunTasks::UpdateUserLocation(nUserID, sLastIP);
}

/*********************************************
 * Users chat
**********************************************/

CmdHandlerSendChatMessage::CmdHandlerSendChatMessage()
    : CmdHandlerBase("sendchatmessage", "Method will be keep message and it sent to another users"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void CmdHandlerSendChatMessage::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    IUserToken *pUserToken = pRequest->userToken();
    QString username = "";
    if(pUserToken != NULL){
        username = pUserToken->nick();
    }else{
        username = "Guest";
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("INSERT INTO chatmessages(user, message, dt) VALUES(:user,:message, NOW())");
    query.bindValue(":user", username);
    query.bindValue(":message", jsonRequest["message"].toString());
    query.exec();

    QJsonObject jsonData2;
    jsonData2["cmd"] = QJsonValue("chat");
    jsonData2["type"] = jsonRequest["type"];
    jsonData2["user"] = username;
    jsonData2["message"] = jsonRequest["message"];
    jsonData2["dt"] = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss");

    pRequest->server()->sendToAll(jsonData2);
}

/*********************************************
 * User login by token
**********************************************/

CmdHandlerToken::CmdHandlerToken()
    : CmdHandlerBase("token", "Method for login by token"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(false);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("token").string_().optional().description("Auth token"));
}

// ---------------------------------------------------------------------

void CmdHandlerToken::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));

    if(!jsonRequest.contains("token")){
        pRequest->sendMessageError(cmd(), Errors::NotFound("requred parameter token"));
        return;
    }
    QString token = jsonRequest["token"].toString();
    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);
    query.prepare("SELECT * FROM users_tokens WHERE token = :token");
    query.bindValue(":token", token);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text().toStdString());
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if (query.next()) {
        QSqlRecord record = query.record();
        int userid = record.value("userid").toInt();
        QString status = record.value("status").toString();
        QString data = record.value("data").toString();
        QString start_date = record.value("start_date").toString();
        QString end_date = record.value("end_date").toString();
        QString lastip = pRequest->client()->peerAddress().toString();
        pRequest->server()->setUserToken(pRequest->client(), new ModelUserToken(data));
        Log::info(TAG, "userid: " + QString::number(userid).toStdString());
        // TODO redesign this
        RunTasks::UpdateUserLocation(userid, lastip);
    }else{
        Log::err(TAG, "Invalid token " + token.toStdString());
        // ["error"]
        pRequest->sendMessageError(cmd(), Errors::InvalidToken());
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * User location update
**********************************************/

CmdHandlerUpdateUserLocation::CmdHandlerUpdateUserLocation()
    : CmdHandlerBase("updateuserlocation", "This method will be try update user location by lastip"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("userid").required().integer_().description("User ID"));
}

// ---------------------------------------------------------------------

void CmdHandlerUpdateUserLocation::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    // bool bConvert = false;

    int userid = jsonRequest["userid"].toInt();

    // TODO redesign
    if(userid == 0){
        pRequest->sendMessageError(cmd(), Errors::QuestIDMustBeNotZero());
        return;
    }

    QSqlDatabase db = *(pDatabase->database());
    QString lastip = "";
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE id = :userid");
        query.bindValue(":userid", userid);
        query.exec();
        if (query.next()) {
            QSqlRecord record = query.record();
            lastip = record.value("last_ip").toString();
        }
    }

    if(lastip == "" || lastip == NULL){
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users_ips WHERE userid = :userid ORDER BY id DESC");
        query.bindValue(":userid", userid);
        query.exec();
        if (query.next()) {
            QSqlRecord record = query.record();
            lastip = record.value("ip").toString();
        }
    }

    RunTasks::UpdateUserLocation(userid, lastip);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * User change password
**********************************************/

CmdHandlerUserChangePassword::CmdHandlerUserChangePassword()
    : CmdHandlerBase("user_change_password", "This method for change user password"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("password_old").required().string_().description("Old password"));
    m_vInputs.push_back(CmdInputDef("password_new").required().string_().description("New password"));
}

// ---------------------------------------------------------------------

void CmdHandlerUserChangePassword::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    IUserToken *pUserToken = pRequest->userToken();
    int nUserID = pUserToken->userid();

    QSqlDatabase db = *(pDatabase->database());

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

    QString sOldPassword = jsonRequest["password_old"].toString();
    QString sNewPassword = jsonRequest["password_new"].toString();

    QString sOldPassword_sha1 = sEmail.toUpper() + sOldPassword;
    sOldPassword_sha1 = QString("%1").arg(QString(QCryptographicHash::hash(sOldPassword_sha1.toUtf8(),QCryptographicHash::Sha1).toHex()));

    if(sOldPassword_sha1 != sPass){
        pRequest->sendMessageError(cmd(), Error(401, "Wrong password"));
        return;
    }

    QString sNewPassword_sha1 = sEmail.toUpper() + sNewPassword;
    sNewPassword_sha1 = QString("%1").arg(QString(QCryptographicHash::hash(sNewPassword_sha1.toUtf8(),QCryptographicHash::Sha1).toHex()));

    QSqlQuery query_update(db);
    query_update.prepare("UPDATE users SET pass = :pass WHERE id = :userid AND email = :email");
    query_update.bindValue(":pass", sNewPassword_sha1);
    query_update.bindValue(":userid", nUserID);
    query_update.bindValue(":email", sEmail);

    if(!query_update.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query_update.lastError().text()));
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * User create
**********************************************/

CmdHandlerUserCreate::CmdHandlerUserCreate()
    : CmdHandlerBase("user_create", "Method for creating a user"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("email").required().string_().description("User's E-mail"));
    m_vInputs.push_back(CmdInputDef("nick").required().string_().description("User's nick"));
    m_vInputs.push_back(CmdInputDef("password").required().string_().description("Password"));
    m_vInputs.push_back(CmdInputDef("role").required().string_().description("User's role"));
    m_vInputs.push_back(CmdInputDef("university").optional().string_().description("University"));

}

// ---------------------------------------------------------------------

void CmdHandlerUserCreate::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QRegularExpression regexEmail("^[0-9a-zA-Z-._@]{3,128}$");
    QString sEmail = jsonRequest["email"].toString();

    if(!regexEmail.match(sEmail).hasMatch()){
        Log::err(TAG, "Invalid email format " + sEmail.toStdString());
        pRequest->sendMessageError(cmd(), Error(400, "Expected email format"));
        return;
    }

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE email = :email");
    query.bindValue(":email", sEmail);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text().toStdString());
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if (query.next()) {
        Log::err(TAG, "User already exists " + sEmail.toStdString());
        pRequest->sendMessageError(cmd(), Error(403, "This email already exists"));
        return;
    }

    QString sNick = jsonRequest["nick"].toString().trimmed();

    if(sNick.size() < 3 && sNick.size() > 128){
        Log::err(TAG, "Invalid nick format " + sNick.toStdString());
        pRequest->sendMessageError(cmd(), Error(400, "Expected nick format"));
        return;
    }

    QRegularExpression regexPassword("^[0-9a-zA-Z]{3,128}$");
    QString sPassword = jsonRequest["password"].toString();

    if(!regexPassword.match(sPassword).hasMatch()){
        Log::err(TAG, "Invalid password format");
        pRequest->sendMessageError(cmd(), Error(400, "Expected password format"));
        return;
    }

    QString sPassword_sha1 = sEmail.toUpper() + sPassword;
    sPassword_sha1 = QString("%1").arg(QString(QCryptographicHash::hash(sPassword_sha1.toUtf8(),QCryptographicHash::Sha1).toHex()));

    QString sRole = jsonRequest.value("role").toString();
    if(sRole != "user" && sRole != "admin"){
        Log::err(TAG, "Invalid role format " + sRole.toStdString());
        pRequest->sendMessageError(cmd(), Error(400, "This role doesn't exist"));
        return;
    }

    QString sUniversity;
    if(jsonRequest.contains("university"))
        sUniversity = jsonRequest.value("university").toString();
    else
        sUniversity = "";

    QSqlQuery query_insert(db);
    query_insert.prepare(""
                         "INSERT INTO users ("
                         "   uuid, "
                         "   email, "
                         "   pass, "
                         "   role, "
                         "   nick,"
                         "   logo,"
                         "   dt_create,"
                         "   dt_last_login,"
                         "   last_ip,"
                         "   status,"
                         "   country,"
                         "   region,"
                         "   city,"
                         "   university,"
                         "   latitude,"
                         "   longitude,"
                         "   rating,"
                         "   about)"
                         "VALUES("
                         "   :uuid, "
                         "   :email, "
                         "   :pass, "
                         "   :role, "
                         "   :nick,"
                         "   :logo,"
                         "   NOW(),"
                         "   NOW(),"
                         "   :last_ip,"
                         "   :status,"
                         "   :country,"
                         "   :region,"
                         "   :city,"
                         "   :university,"
                         "   :latitude,"
                         "   :longitude,"
                         "   :rating,"
                         "   :about);"
    );

    QString sLastIP = pRequest->client()->peerAddress().toString();

    QString sUuid = QUuid::createUuid().toString();
    sUuid = sUuid.mid(1,sUuid.length()-2);
    sUuid = sUuid.toUpper();

    query_insert.bindValue(":uuid", sUuid);
    query_insert.bindValue(":email", sEmail);
    query_insert.bindValue(":pass", sPassword_sha1);
    query_insert.bindValue(":role", sRole);
    query_insert.bindValue(":nick", sNick);
    query_insert.bindValue(":logo", "files/users/0.png");
    query_insert.bindValue(":last_ip", "");
    query_insert.bindValue(":status", "activated");
    query_insert.bindValue(":country", "");
    query_insert.bindValue(":region", "");
    query_insert.bindValue(":city", "");
    query_insert.bindValue(":university", sUniversity);
    query_insert.bindValue(":latitude", 0);
    query_insert.bindValue(":longitude", 0);
    query_insert.bindValue(":rating", 0);
    query_insert.bindValue(":about", "");

    if(!query_insert.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query_insert.lastError().text()));
        return;
    }

    int nUserID = query_insert.lastInsertId().toInt();

    RunTasks::AddPublicEvents("users", "New user #" + QString::number(nUserID) + "  " + sNick);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * User info
**********************************************/

CmdHandlerUser::CmdHandlerUser()
    : CmdHandlerBase("user", "Return user info"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("userid").optional().integer_().description("Id of user"));
}

// ---------------------------------------------------------------------

void CmdHandlerUser::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    IUserToken *pUserToken = pRequest->userToken();

    if(!jsonRequest.contains("userid") && pUserToken == NULL){
        pRequest->sendMessageError(cmd(), Errors::NotAuthorizedRequest());
        return;
    }

    bool bCurrentUserOrAdmin = false;

    int nUserID = 0;
    if(pUserToken != NULL){
        nUserID = pUserToken->userid();
        bCurrentUserOrAdmin = true;
    }

    if(jsonRequest.contains("userid")){
        int nUserID_ = jsonRequest["userid"].toInt();
        if(nUserID_ != nUserID){
            bCurrentUserOrAdmin = false;
            if(pUserToken != NULL){
                bCurrentUserOrAdmin = pUserToken->isAdmin();
            }
        }
        nUserID = nUserID_;
    }

    QJsonObject data;
    QJsonObject profile;
    QSqlDatabase db = *(pDatabase->database());

    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE id = :userid");
        query.bindValue(":userid", nUserID);
        query.exec();

        if (query.next()) {
            QSqlRecord record = query.record();
            data["id"] = record.value("id").toInt();
            data["uuid"] = record.value("uuid").toString();
            data["nick"] = record.value("nick").toString().toHtmlEscaped();
            data["role"] = record.value("role").toString();
            data["logo"] = record.value("logo").toString().toHtmlEscaped();
            data["about"] = record.value("about").toString().toHtmlEscaped();
            data["status"] = record.value("status").toString();
            data["rating"] = record.value("rating").toString();
            data["university"] = record.value("university").toString().toHtmlEscaped();

            if(bCurrentUserOrAdmin){
                data["email"] = record.value("email").toString();
                data["dt_create"] = record.value("dt_create").toString();
                data["dt_last_login"] = record.value("dt_last_login").toString();
                data["last_ip"] = record.value("last_ip").toString();
                data["country"] = record.value("country").toString();
                data["region"] = record.value("region").toString();
                data["city"] = record.value("city").toString();
            }
        }else{
            pRequest->sendMessageError(cmd(), Errors::NotFound("user"));
            return;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare("SELECT name, value, date_change FROM users_profile WHERE userid = :userid");
        query.bindValue(":userid", nUserID);
        query.exec();

        while (query.next()) {
            QSqlRecord record = query.record();
            QString name = record.value("name").toString();
            QString value = record.value("value").toString();
            profile[name] = value;

            // TODO clenup 'template' from user profiles
        }
    }

    jsonResponse["data"] = data;
    jsonResponse["profile"] = profile;
    jsonResponse["access"] = bCurrentUserOrAdmin;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * User reset password
**********************************************/

CmdHandlerUserResetPassword::CmdHandlerUserResetPassword()
    : CmdHandlerBase("user_reset_password", "Method for reset password"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(false);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("email").required().email_().description("E-mail"));
}

// ---------------------------------------------------------------------

void CmdHandlerUserResetPassword::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QString sEmail = jsonRequest["email"].toString();

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE email = :email");
    query.bindValue(":email", sEmail);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text().toStdString());
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    int nUserID = 0;
    QString sNick = "";
    if (query.next()) {
        QSqlRecord record = query.record();
        nUserID = record.value("id").toInt();
        sNick = record.value("nick").toString().toHtmlEscaped();
    }else{
        Log::err(TAG, "User not found" + sEmail.toStdString());
        pRequest->sendMessageError(cmd(), Error(403, "This email not exists"));
        return;
    }

    // // generate random password
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = 12; // assuming you want random strings of 12 characters
    QString sPassword;
    for(int i=0; i<randomStringLength; ++i){
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        sPassword.append(nextChar);
    }

    QString sPassword_sha1 = sEmail.toUpper() + sPassword;
    sPassword_sha1 = QString("%1").arg(QString(QCryptographicHash::hash(sPassword_sha1.toUtf8(),QCryptographicHash::Sha1).toHex()));

    QSqlQuery query_update(db);
    query_update.prepare("UPDATE users SET pass = :pass WHERE id = :userid AND email = :email;");

    query_update.bindValue(":pass", sPassword_sha1);
    query_update.bindValue(":userid", nUserID);
    query_update.bindValue(":email", sEmail);

    if(!query_update.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query_update.lastError().text()));
        return;
    }

    RunTasks::AddPublicEvents("users", "User comeback #" + QString::number(nUserID) + "  " + sNick);

    QString sSubject = "Reset Password from FreeHackQuest";
    QString sContext = "Welcome back to FreeHackQuest!\n"
                       "You login: " + sEmail + "\n"
                       "You password: " + sPassword + "\n";

    RunTasks::MailSend(pRequest->server(), sEmail, sSubject, sContext);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * User skill
**********************************************/

CmdHandlerUserSkills::CmdHandlerUserSkills()
    : CmdHandlerBase("user_skills", "Return user skills info"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("userid").required().integer_().description("Id of user"));
}

// ---------------------------------------------------------------------

void CmdHandlerUserSkills::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;


    QJsonObject skills_max;
    QJsonObject skills_user;

    QSqlDatabase db = *(pDatabase->database());

    {
        QSqlQuery query(db);
        query.prepare("SELECT q.subject, sum(q.score) as sum_subject FROM quest q WHERE ! ISNULL( q.subject ) GROUP BY q.subject");
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Errors::DatabaseError(query.lastError().text()));
            return;
        };

        while(query.next()) {
            QSqlRecord record = query.record();
            QString subject = record.value("subject").toString();
            skills_max[subject] = record.value("sum_subject").toInt();
        }
    }


    int nUserID = jsonRequest["userid"].toInt();
    {
        QSqlQuery query(db);
        query.prepare("SELECT uq.userid, q.subject, SUM( q.score ) as sum_score FROM users_quests uq INNER JOIN quest q ON uq.questid = q.idquest WHERE ! ISNULL( q.subject ) AND uq.userid = :userid GROUP BY uq.userid, q.subject");
        query.bindValue(":userid", nUserID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Errors::DatabaseError(query.lastError().text()));
            return;
        };

        while(query.next()) {
            QSqlRecord record = query.record();
            QString subject = record.value("subject").toString();
            skills_user[subject] = record.value("sum_score").toInt();
        }
    }

    jsonResponse["skills_max"] = skills_max;
    jsonResponse["skills_user"] = skills_user;

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * User update
**********************************************/

CmdHandlerUserUpdate::CmdHandlerUserUpdate()
    : CmdHandlerBase("user_update", "Update user info"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("userid").required().integer_().description("Id of user"));
    m_vInputs.push_back(CmdInputDef("nick").optional().string_().description("Nick of user"));
    m_vInputs.push_back(CmdInputDef("university").optional().string_().description("University of user"));
    m_vInputs.push_back(CmdInputDef("about").optional().string_().description("About of user"));
}

// ---------------------------------------------------------------------

void CmdHandlerUserUpdate::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;
    QJsonObject data;

    IUserToken *pUserToken = pRequest->userToken();
    int nUserIDFromToken = pUserToken->userid();
    int nUserID = jsonRequest["userid"].toInt();

    if(nUserIDFromToken != nUserID && !pUserToken->isAdmin()){
        pRequest->sendMessageError(cmd(), Error(403, "Deny change inmormation about user"));
        return;
    }

    QString sNick = "";
    QString sUniversity = "";
    QString sAbout = "";

    QSqlDatabase db = *(pDatabase->database());
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE id = :userid");
        query.bindValue(":userid", nUserID);

        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Errors::DatabaseError(query.lastError().text()));
            return;
        };

        if(!query.next()) {
            pRequest->sendMessageError(cmd(), Error(404, "User not found"));
            return;
        }else{
            QSqlRecord record = query.record();
            sNick = record.value("nick").toString();
            sUniversity = record.value("university").toString();
            sAbout = record.value("about").toString();
        }
    }

    if(jsonRequest.contains("nick")){
        sNick = jsonRequest["nick"].toString();
    }

    if(jsonRequest.contains("university")){
        sUniversity = jsonRequest["university"].toString();
    }

    if(jsonRequest.contains("about")){
        sAbout = jsonRequest["about"].toString();
    }

    // update
    {
        QSqlQuery query(db);
        query.prepare("UPDATE users SET nick = :nick, university = :university, about = :about WHERE id = :userid");
        query.bindValue(":nick", sNick);
        query.bindValue(":university", sUniversity);
        query.bindValue(":about", sAbout);
        query.bindValue(":userid", nUserID);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        };
    }

    pUserToken->setNick(sNick);
    RunTasks::AddPublicEvents("users", "User #" + QString::number(nUserID) + "  " + sNick
                              + " updated info");

    data["id"] = nUserID;
    data["nick"] = sNick.toHtmlEscaped();
    data["university"] = sUniversity.toHtmlEscaped();
    data["about"] = sAbout.toHtmlEscaped();
    jsonResponse["data"] = data;

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * User delete
**********************************************/

CmdHandlerUserDelete::CmdHandlerUserDelete()
    : CmdHandlerBase("user_delete", "Method for deleting a user"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("userid").required().integer_().description("User's id"));
    m_vInputs.push_back(CmdInputDef("password").required().string_().description("Admin's password"));
}

// ---------------------------------------------------------------------

void CmdHandlerUserDelete::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QString sAdminPassword = jsonRequest["password"].toString();

    IUserToken *pUserToken = pRequest->userToken();
    int nAdminUserID = pUserToken->userid();

    QSqlDatabase db = *(pDatabase->database());

    // check admin password
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE id = :userid");
        query.bindValue(":userid", nAdminUserID);
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

    int nUserID = jsonRequest["userid"].toInt();

    // check if the user exists
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE id = :id");
        query.bindValue(":id", nUserID);

        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }

        if(!query.next()) {
            pRequest->sendMessageError(cmd(), Error(404, "User not found"));
            return;
        }
    }

    QSqlQuery query_del(db);

    // delete from feedback
    {
        query_del.prepare("DELETE FROM feedback WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from feedback_msg
    {
        query_del.prepare("DELETE FROM feedback_msg WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from quest
    {
        query_del.prepare("DELETE FROM quest WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_games
    {
        query_del.prepare("DELETE FROM users_games WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_profile
    {
        query_del.prepare("DELETE FROM users_profile WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_quests
    {
        query_del.prepare("DELETE FROM users_quests WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_tokens
    {
        query_del.prepare("DELETE FROM users_tokens WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_tokens_invalid
    {
        query_del.prepare("DELETE FROM users_tokens_invalid WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_offers
    {
        query_del.prepare("DELETE FROM users_offers WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from quests_proposal
    {
        query_del.prepare("DELETE FROM quests_proposal WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users_quests_answers
    {
        query_del.prepare("DELETE FROM users_quests_answers WHERE userid = :userid");
        query_del.bindValue(":userid", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    // delete from users
    {
        query_del.prepare("DELETE FROM users WHERE id = :id");
        query_del.bindValue(":id", nUserID);
        if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * Users
**********************************************/

CmdHandlerUsers::CmdHandlerUsers()
    : CmdHandlerBase("users", "Method return list of users"){

    TAG = "CmdUsersHandler";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("filter_text").string_().optional().description("Filter by user email or nick"));
    m_vInputs.push_back(CmdInputDef("filter_role").string_().optional().description("Filter by user role"));
    m_vInputs.push_back(CmdInputDef("onpage").integer_().optional().description("On page"));
    m_vInputs.push_back(CmdInputDef("page").integer_().optional().description("page"));
}

// ---------------------------------------------------------------------

void CmdHandlerUsers::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;


    QStringList filters;
    QMap<QString,QString> filter_values;

    int nPage = 0;
    int nOnPage = 5;
    int nCount = 0;

    if(jsonRequest.contains("filter_text")){
        QString text = jsonRequest["filter_text"].toString().trimmed();
        if(text != ""){
            filters << "(email LIKE :email OR nick LIKE :nick)";
            filter_values[":email"] = "%" + text + "%";
            filter_values[":nick"] = "%" + text + "%";
        }
    }
    if(jsonRequest.contains("filter_role")){
        QString role = jsonRequest["filter_role"].toString().trimmed();
        if(role != ""){
            filters << "role = :role";
            filter_values[":role"] = role;
        }
    }

    if(jsonRequest.contains("page")){
        nPage = jsonRequest["page"].toInt();
    }

    if(jsonRequest.contains("onpage")){
        nOnPage = jsonRequest["onpage"].toInt();
    }

    QJsonArray users;
    QSqlDatabase db = *(pDatabase->database());
    QString where = filters.join(" AND ");
    if(where.length() > 0){
        where = "WHERE " + where;
    }

    // calculate count
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM users " + where);
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if(query.next()) {
            QSqlRecord record = query.record();
            nCount = record.value("cnt").toInt();
        }
    }

    // users
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users " + where + " ORDER BY dt_last_login DESC LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage));
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        query.exec();
        while (query.next()) {
            QSqlRecord record = query.record();
            int userid = record.value("id").toInt();
            QString sUuid = record.value("uuid").toString();
            QString sEmail = record.value("email").toString();
            QString sNick = record.value("nick").toString().toHtmlEscaped();
            QString sUniversity = record.value("university").toString().toHtmlEscaped();
            int nRating = record.value("rating").toInt();
            QString sCreated = record.value("dt_created").toString().toHtmlEscaped();
            QString sDTLastLogin = record.value("dt_last_login").toString().toHtmlEscaped();
            QString sLastIP = record.value("last_ip").toString().toHtmlEscaped();
            QString sCountry = record.value("country").toString().toHtmlEscaped();
            QString sRegion = record.value("region").toString().toHtmlEscaped();
            QString sCity = record.value("city").toString().toHtmlEscaped();
            QString sRole = record.value("role").toString().toHtmlEscaped();
            QJsonObject user;
            user["id"] = userid;
            user["uuid"] = sUuid;
            user["nick"] = sNick;
            user["email"] = sEmail;
            user["created"] = sCreated;
            user["dt_last_login"] = sDTLastLogin;
            user["last_ip"] = sLastIP;
            user["university"] = sUniversity;
            user["rating"] = nRating;
            user["country"] = sCountry;
            user["region"] = sRegion;
            user["city"] = sCity;
            user["role"] = sRole;
            users.push_back(user);
        }
    }

    jsonResponse["data"] = users;
    jsonResponse["onpage"] = nOnPage;
    jsonResponse["page"] = nPage;
    jsonResponse["count"] = nCount;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
