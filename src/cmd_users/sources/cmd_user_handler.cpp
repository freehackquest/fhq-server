#include <cmd_user_handler.h>
#include <log.h>
#include <QJsonArray>

CmdUserHandler::CmdUserHandler(){
    TAG = "CmdUserHandler";

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
	m_vInputs.push_back(CmdInputDef("userid").optional().integer_().description("Id of user"));
}

// ---------------------------------------------------------------------

std::string CmdUserHandler::cmd(){
	return "user";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdUserHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdUserHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdUserHandler::description(){
	return "Return user info";
}

// ---------------------------------------------------------------------

void CmdUserHandler::handle(ModelRequest *pRequest){
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
    QSqlDatabase db = *(pRequest->server()->database());

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
