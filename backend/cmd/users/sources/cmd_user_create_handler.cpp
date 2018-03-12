#include <cmd_user_create_handler.h>
#include <runtasks.h>
#include <log.h>
#include <model_usertoken.h>
#include <QCryptographicHash>
#include <QUuid>
#include <QRegularExpression>
#include <employ_settings.h>

CmdUserCreateHandler::CmdUserCreateHandler(){
    TAG = "CmdUsersCreateHandler";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("login").required().string_().description("User's login"));
    m_vInputs.push_back(CmdInputDef("nick").required().string_().description("User's nick"));
    m_vInputs.push_back(CmdInputDef("pass").required().string_().description("Password"));
}

// ---------------------------------------------------------------------

std::string CmdUserCreateHandler::cmd(){
    return "user_create";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdUserCreateHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdUserCreateHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdUserCreateHandler::description(){
    return "Method for creating a user";
}

// ---------------------------------------------------------------------

void CmdUserCreateHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    //EmploySettings *pSettings = findEmploy<EmploySettings>();

    QRegularExpression regexLogin("^[0-9a-zA-Z-._]{3,128}$");
    QString sLogin = jsonRequest["login"].toString();

    if(!regexLogin.match(sLogin).hasMatch()){
        Log::err(TAG, "Invalid login format " + sLogin);
        pRequest->sendMessageError(cmd(), Error(400, "Expected login format"));
        return;
    }

    QSqlDatabase db = *(pRequest->server()->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE email = :login");
    query.bindValue(":login", sLogin);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if (query.next()) {
        Log::err(TAG, "User already exists " + sLogin);
        pRequest->sendMessageError(cmd(), Error(403, "This login is already in use"));
        return;
    }

    QRegularExpression regexNick("^[0-9A-H]{3,128}$");
    QString sNick = jsonRequest["nick"].toString();

    if(!regexNick.match(sNick).hasMatch()){
        Log::err(TAG, "Invalid nick format " + sNick);
        pRequest->sendMessageError(cmd(), Error(400, "Expected nick format"));
        return;
    }

    sNick = "hacker-" + sNick;

    QRegularExpression regexPass("^[0-9a-zA-Z]{3,128}$");
    QString sPass = jsonRequest["pass"].toString();

    if(!regexPass.match(sPass).hasMatch()){
        Log::err(TAG, "Invalid password format");
        pRequest->sendMessageError(cmd(), Error(400, "Expected password format"));
        return;
    }

    QString sPassword_sha1 = sLogin.toUpper() + sPass;
    sPassword_sha1 = QString("%1").arg(QString(QCryptographicHash::hash(sPassword_sha1.toUtf8(),QCryptographicHash::Sha1).toHex()));

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
                         "   :login, "
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
    query_insert.bindValue(":login", sLogin);
    query_insert.bindValue(":pass", sPassword_sha1);
    query_insert.bindValue(":role", "user");
    query_insert.bindValue(":nick", sNick);
    query_insert.bindValue(":logo", "files/users/0.png");
    query_insert.bindValue(":last_ip", "");
    query_insert.bindValue(":status", "activated");
    query_insert.bindValue(":country", "");
    query_insert.bindValue(":region", "");
    query_insert.bindValue(":city", "");
    query_insert.bindValue(":university", "");
    query_insert.bindValue(":latitude", 0);
    query_insert.bindValue(":longitude", 0);
    query_insert.bindValue(":rating", 0);
    query_insert.bindValue(":about", "");

    if(!query_insert.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query_insert.lastError().text()));
        return;
    }

    int nUserID = query_insert.lastInsertId().toInt();

    RunTasks::AddPublicEvents(pRequest->server(), "users", "New user #" + QString::number(nUserID) + "  " + sNick);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
