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

std::string CmdUserUpdateHandler::cmd(){
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

const std::vector<CmdInputDef> &CmdUserUpdateHandler::inputs(){
	return m_vInputs;
};

QString CmdUserUpdateHandler::description(){
    return "Update user info";
}

QStringList CmdUserUpdateHandler::errors(){
	QStringList	list;
	return list;
}

void CmdUserUpdateHandler::handle(ModelRequest *pRequest){
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

    QSqlDatabase db = *(pRequest->server()->database());
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
    RunTasks::AddPublicEvents(pRequest->server(), "users", "User #" + QString::number(nUserID) + "  " + sNick
                              + " updated info");

    data["id"] = nUserID;
    data["nick"] = sNick.toHtmlEscaped();
    data["university"] = sUniversity.toHtmlEscaped();
    data["about"] = sAbout.toHtmlEscaped();
    jsonResponse["data"] = data;

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
