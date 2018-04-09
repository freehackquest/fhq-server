#include <cmd_user_create_handler.h>
#include <runtasks.h>
#include <log.h>
#include <model_usertoken.h>
#include <QCryptographicHash>
#include <QUuid>
#include <QRegularExpression>
#include <employ_database.h>

CmdUserCreateHandler::CmdUserCreateHandler(){
    TAG = "CmdUsersCreateHandler";

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
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QRegularExpression regexEmail("^[0-9a-zA-Z-._@]{3,128}$");
    QString sEmail = jsonRequest["email"].toString();

    if(!regexEmail.match(sEmail).hasMatch()){
        Log::err(TAG, "Invalid email format " + sEmail);
        pRequest->sendMessageError(cmd(), Error(400, "Expected email format"));
        return;
    }

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE email = :email");
    query.bindValue(":email", sEmail);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if (query.next()) {
        Log::err(TAG, "User already exists " + sEmail);
        pRequest->sendMessageError(cmd(), Error(403, "This email already exists"));
        return;
    }

    QString sNick = jsonRequest["nick"].toString().trimmed();

    if(sNick.size() < 3 && sNick.size() > 128){
        Log::err(TAG, "Invalid nick format " + sNick);
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
        Log::err(TAG, "Invalid role format " + sRole);
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
