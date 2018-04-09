#include <cmd_user_change_password_handler.h>
#include <log.h>
#include <QJsonArray>
#include <employ_database.h>

CmdUserChangePasswordHandler::CmdUserChangePasswordHandler(){
    TAG = "CmdUserChangePasswordHandler";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("password_old").required().string_().description("Old password"));
    m_vInputs.push_back(CmdInputDef("password_new").required().string_().description("New password"));
}

// ---------------------------------------------------------------------

std::string CmdUserChangePasswordHandler::cmd(){
    return "user_change_password";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdUserChangePasswordHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdUserChangePasswordHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdUserChangePasswordHandler::description(){
	return "Return user answer list";
}

// ---------------------------------------------------------------------

void CmdUserChangePasswordHandler::handle(ModelRequest *pRequest){
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
