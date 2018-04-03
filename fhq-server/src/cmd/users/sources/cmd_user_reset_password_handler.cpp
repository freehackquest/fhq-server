#include <cmd_user_reset_password_handler.h>
#include <runtasks.h>
#include <log.h>
#include <model_usertoken.h>
#include <QCryptographicHash>
#include <QUuid>
#include <employ_settings.h>
#include <SmtpMime>

CmdUserResetPasswordHandler::CmdUserResetPasswordHandler(){
    TAG = "CmdUserResetPasswordHandler";

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(false);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("email").required().email_().description("E-mail"));
}

// ---------------------------------------------------------------------

std::string CmdUserResetPasswordHandler::cmd(){
    return "user_reset_password";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdUserResetPasswordHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdUserResetPasswordHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdUserResetPasswordHandler::description(){
    return "Method for reset password";
}

// ---------------------------------------------------------------------

void CmdUserResetPasswordHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;
	
    QString sEmail = jsonRequest["email"].toString();

    QSqlDatabase db = *(pRequest->server()->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE email = :email");
    query.bindValue(":email", sEmail);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
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
        Log::err(TAG, "User not found" + sEmail);
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

    RunTasks::AddPublicEvents(pRequest->server(), "users", "User comeback #" + QString::number(nUserID) + "  " + sNick);

    QString sSubject = "Reset Password from FreeHackQuest";
    QString sContext = "Welcome back to FreeHackQuest!\n"
                       "You login: " + sEmail + "\n"
                       "You password: " + sPassword + "\n";

    RunTasks::MailSend(pRequest->server(), sEmail, sSubject, sContext);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
