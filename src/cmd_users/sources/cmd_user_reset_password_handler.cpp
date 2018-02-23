#include <cmd_user_reset_password_handler.h>
#include <runtasks.h>
#include <log.h>
#include <usertoken.h>
#include <QCryptographicHash>
#include <QUuid>
#include <QRegularExpression>
#include <memory_cache_serversettings.h>
#include <SmtpMime>

CmdUserResetPasswordHandler::CmdUserResetPasswordHandler(){
    TAG = "CmdUserResetPasswordHandler";
    m_vInputs.push_back(CmdInputDef("email").required().string_().description("E-mail"));
}

std::string CmdUserResetPasswordHandler::cmd(){
    return "user_reset_password";
}

bool CmdUserResetPasswordHandler::accessUnauthorized(){
	return true;
}

bool CmdUserResetPasswordHandler::accessUser(){
    return false;
}

bool CmdUserResetPasswordHandler::accessTester(){
    return false;
}

bool CmdUserResetPasswordHandler::accessAdmin(){
    return false;
}

const QVector<CmdInputDef> &CmdUserResetPasswordHandler::inputs(){
	return m_vInputs;
};

QString CmdUserResetPasswordHandler::description(){
    return "Method for reset password";
}

QStringList CmdUserResetPasswordHandler::errors(){
	QStringList	list;
	return list;
}

void CmdUserResetPasswordHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
	
    IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serversettings");
    if(pMemoryCache == NULL){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
        return;
    }
    MemoryCacheServerSettings *pMemoryCacheServerSettings = dynamic_cast<MemoryCacheServerSettings*>(pMemoryCache);

    QRegularExpression regexEmail("^[0-9a-zA-Z]{1}[0-9a-zA-Z-._]*[0-9a-zA-Z]{1}@[0-9a-zA-Z]{1}[-.0-9a-zA-Z]*[0-9a-zA-Z]{1}\\.[a-zA-Z]{2,6}$");
    QString sEmail = obj["email"].toString();

    if(!regexEmail.match(sEmail).hasMatch()){
        Log::err(TAG, "Invalid email format " + sEmail);
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(400, "Expected email format"));
        return;
    }

    QSqlDatabase db = *(pWebSocketServer->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE email = :email");
    query.bindValue(":email", sEmail);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
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
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(403, "This email not exists"));
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
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query_update.lastError().text()));
        return;
    }

    RunTasks::AddPublicEvents(pWebSocketServer, "users", "User comeback #" + QString::number(nUserID) + "  " + sNick);

    // TODO move to tasks
    QString sMailHost = pMemoryCacheServerSettings->getSettString("mail_host");
    int nMailPort = pMemoryCacheServerSettings->getSettInteger("mail_port");
    QString sMailPassword = pMemoryCacheServerSettings->getSettPassword("mail_password");
    QString sMailFrom = pMemoryCacheServerSettings->getSettString("mail_from");

    SmtpClient smtp(sMailHost, nMailPort, SmtpClient::SslConnection);
    smtp.setUser(sMailFrom);
    smtp.setPassword(sMailPassword);

    MimeMessage message;

    EmailAddress sender(sMailFrom, "FreeHackQuest");
    message.setSender(&sender);

    EmailAddress to(sEmail, "");
    message.addRecipient(&to);

    message.setSubject("Reset Password from FreeHackQuest");

    MimeText text;
    text.setText("Welcome back to FreeHackQuest!\n"
                 "You login: " + sEmail + "\n"
                 "You password: " + sPassword + "\n"
              );

    message.addPart(&text);

    // Now we can send the mail
    if (!smtp.connectToHost()) {
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, "[MAIL] Failed to connect to host!"));
        return;
    }

    if (!smtp.login()) {
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, "[MAIL] Failed to login!"));
        return;
    }

    if (!smtp.sendMail(message)) {
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, "[MAIL] Failed to send mail!"));
        return;
    }
    smtp.quit();



    jsonData["result"] = QJsonValue("DONE");
    jsonData["m"] = QJsonValue(m);
    pWebSocketServer->sendMessage(pClient, jsonData);
}
