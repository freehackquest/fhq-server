#include <cmd_user_change_password_handler.h>
#include <log.h>
#include <QJsonArray>

CmdUserChangePasswordHandler::CmdUserChangePasswordHandler(){
    TAG = "CmdUserChangePasswordHandler";
    m_vInputs.push_back(CmdInputDef("password_old").required().string_().description("Old password"));
    m_vInputs.push_back(CmdInputDef("password_new").required().string_().description("New password"));
}

QString CmdUserChangePasswordHandler::cmd(){
    return "user_change_password";
}

bool CmdUserChangePasswordHandler::accessUnauthorized(){
	return false;
}

bool CmdUserChangePasswordHandler::accessUser(){
	return true;
}

bool CmdUserChangePasswordHandler::accessTester(){
	return true;
}

bool CmdUserChangePasswordHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdUserChangePasswordHandler::inputs(){
	return m_vInputs;
};

QString CmdUserChangePasswordHandler::description(){
	return "Return user answer list";
}

QStringList CmdUserChangePasswordHandler::errors(){
	QStringList	list;
	return list;
}

void CmdUserChangePasswordHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
    int nUserID = pUserToken->userid();

    QSqlDatabase db = *(pWebSocketServer->database());

    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE id = :userid");
    query.bindValue(":userid", nUserID);
    if(!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }

    QString sPass = "";
    QString sEmail = "";

    if (query.next()) {
        QSqlRecord record = query.record();
        sEmail = record.value("email").toString();
        sPass = record.value("pass").toString();
    }else{
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotFound("user"));
        return;
    }

    QString sOldPassword = obj["password_old"].toString();
    QString sNewPassword = obj["password_new"].toString();

    QString sOldPassword_sha1 = sEmail.toUpper() + sOldPassword;
    sOldPassword_sha1 = QString("%1").arg(QString(QCryptographicHash::hash(sOldPassword_sha1.toUtf8(),QCryptographicHash::Sha1).toHex()));

    if(sOldPassword_sha1 != sPass){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(401, "Wrong password"));
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
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query_update.lastError().text()));
        return;
    }

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	pWebSocketServer->sendMessage(pClient, jsonData);
}
