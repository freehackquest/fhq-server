#include <cmd_login_handler.h>
#include <runtasks.h>
#include <log.h>
#include <model_usertoken.h>
#include <QCryptographicHash>
#include <QUuid>

CmdLoginHandler::CmdLoginHandler(){
	TAG = "CmdLoginHandler";
    m_vInputs.push_back(CmdInputDef("email").required().string_().description("E-mail"));
    m_vInputs.push_back(CmdInputDef("password").required().string_().description("Password"));
}

std::string CmdLoginHandler::cmd(){
	return "login";
}

bool CmdLoginHandler::accessUnauthorized(){
	return true;
}

bool CmdLoginHandler::accessUser(){
	return true;
}

bool CmdLoginHandler::accessTester(){
	return true;
}

bool CmdLoginHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdLoginHandler::inputs(){
	return m_vInputs;
};

QString CmdLoginHandler::description(){
	return "Method for login";
}

QStringList CmdLoginHandler::errors(){
	QStringList	list;
	return list;
}

void CmdLoginHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;
	
    QString email = jsonRequest["email"].toString();
    QString password = jsonRequest["password"].toString();

    QString password_sha1 = email.toUpper() + password;

    password_sha1 = QString("%1").arg(QString(QCryptographicHash::hash(password_sha1.toUtf8(),QCryptographicHash::Sha1).toHex()));

    QSqlDatabase db = *(pRequest->server()->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE email = :email AND pass = :pass");
    query.bindValue(":email", email);
    query.bindValue(":pass", password_sha1);

    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
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
            Log::err(TAG, query_token.lastError().text());
            pRequest->sendMessageError(cmd(), Error(500, query_token.lastError().text()));
            return;
        }

        jsonResponse["token"] = token;
        jsonResponse["user"] = user;

        pRequest->server()->setUserToken(pRequest->client(), new ModelUserToken(user_token));

        // update user location
        QString lastip = pRequest->client()->peerAddress().toString();
        RunTasks::UpdateUserLocation(pRequest->server(), userid, lastip);

    }else{
        Log::err(TAG, "Invalid login or password");
        pRequest->sendMessageError(cmd(), Error(401, "Invalid login or password"));
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
