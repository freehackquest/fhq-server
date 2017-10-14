#include <cmd_user_update_handler.h>
#include <QJsonArray>
#include <runtasks.h>
#include <log.h>

CmdUserUpdateHandler::CmdUserUpdateHandler(){
    m_vInputs.push_back(CmdInputDef("userid").required().integer_().description("Id of user"));
    m_vInputs.push_back(CmdInputDef("nick").optional().string_().description("Nick of user"));
    m_vInputs.push_back(CmdInputDef("university").optional().string_().description("University of user"));
    m_vInputs.push_back(CmdInputDef("about").optional().string_().description("About of user"));
}

QString CmdUserUpdateHandler::cmd(){
    return "user_update";
}

bool CmdUserUpdateHandler::accessUnauthorized(){
    return false;
}

bool CmdUserUpdateHandler::accessUser(){
    return true;
}

bool CmdUserUpdateHandler::accessTester(){
	return true;
}

bool CmdUserUpdateHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdUserUpdateHandler::inputs(){
	return m_vInputs;
};

QString CmdUserUpdateHandler::description(){
    return "Update user info";
}

QStringList CmdUserUpdateHandler::errors(){
	QStringList	list;
	return list;
}

void CmdUserUpdateHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
    int nUserIDFromToken = pUserToken->userid();
    int nUserID = obj["userid"].toInt();

    if(nUserIDFromToken != nUserID){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(403, "Deny change inmormation about user"));
        return;
    }

    QString sNick = "";
    QString sUniversity = "";
    QString sAbout = "";


    QSqlDatabase db = *(pWebSocketServer->database());
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users WHERE id = :userid");
        query.bindValue(":userid", nUserIDFromToken);

        if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::DatabaseError(query.lastError().text()));
            return;
        };

        if(!query.next()) {
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "User not found"));
            return;
        }else{
            QSqlRecord record = query.record();
            sNick = record.value("nick").toString();
            sUniversity = record.value("university").toString();
            sAbout = record.value("about").toString();
        }
    }

    if(obj.contains("nick")){
        sNick = obj["nick"].toString();
    }

    if(obj.contains("university")){
        sUniversity = obj["university"].toString();
    }

    if(obj.contains("about")){
        sAbout = obj["about"].toString();
    }

    // update
	{
		QSqlQuery query(db);
        query.prepare("UPDATE users SET nick = :nick, university = :university, about = :about WHERE id = :userid");
        query.bindValue(":nick", sNick);
        query.bindValue(":university", sUniversity);
        query.bindValue(":about", sAbout);
        query.bindValue(":userid", nUserIDFromToken);
		if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
		};
	}

    pUserToken->setNick(sNick);
    RunTasks::AddPublicEvents(pWebSocketServer, "users", "User #" + QString::number(nUserIDFromToken) + "  " + sNick
                              + " updated info about self");

    QJsonObject jsonData;
    jsonData["nick"] = sNick.toHtmlEscaped();
    jsonData["university"] = sUniversity.toHtmlEscaped();
    jsonData["about"] = sAbout.toHtmlEscaped();

	jsonData["cmd"] = QJsonValue(cmd());
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
    jsonData["data"] = jsonData;

	pWebSocketServer->sendMessage(pClient, jsonData);
}
