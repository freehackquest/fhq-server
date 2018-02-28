#include <cmd_users_handler.h>
#include <QJsonArray>

CmdUsersHandler::CmdUsersHandler(){
	m_vInputs.push_back(CmdInputDef("filter_text").string_().optional().description("Filter by user email or nick"));
	m_vInputs.push_back(CmdInputDef("filter_role").string_().optional().description("Filter by user role"));
    m_vInputs.push_back(CmdInputDef("onpage").integer_().optional().description("On page"));
    m_vInputs.push_back(CmdInputDef("page").integer_().optional().description("page"));
}

std::string CmdUsersHandler::cmd(){
	return "users";
}

bool CmdUsersHandler::accessUnauthorized(){
	return false;
}

bool CmdUsersHandler::accessUser(){
	return false;
}

bool CmdUsersHandler::accessTester(){
	return false;
}

bool CmdUsersHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdUsersHandler::inputs(){
	return m_vInputs;
};

QString CmdUsersHandler::description(){
	return "Method return list of users";
}

QStringList CmdUsersHandler::errors(){
	QStringList	list;
	return list;
}

void CmdUsersHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);

	if(pUserToken == NULL){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotAuthorizedRequest());
		return;
	}


	QStringList filters;
	QMap<QString,QString> filter_values;

    int nPage = 0;
    int nOnPage = 5;
    int nCount = 0;

	if(obj.contains("filter_text")){
		QString text = obj["filter_text"].toString().trimmed();
		if(text != ""){
			filters << "(email LIKE :email OR nick LIKE :nick)";
			filter_values[":email"] = "%" + text + "%";
			filter_values[":nick"] = "%" + text + "%";
		}
	}
	if(obj.contains("filter_role")){
		QString role = obj["filter_role"].toString().trimmed();
		if(role != ""){
			filters << "role = :role";
			filter_values[":role"] = role;
		}
	}

    if(obj.contains("page")){
        nPage = obj["page"].toInt();
    }

    if(obj.contains("onpage")){
        nOnPage = obj["onpage"].toInt();
    }

	QJsonArray users;
	QSqlDatabase db = *(pWebSocketServer->database());
	QString where = filters.join(" AND "); 
	if(where.length() > 0){
		where = "WHERE " + where;
	}

    // calculate count
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM users " + where);
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        if(query.next()) {
            QSqlRecord record = query.record();
            nCount = record.value("cnt").toInt();
        }
    }

    // users
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM users " + where + " ORDER BY dt_last_login DESC LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage));
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        query.exec();
        while (query.next()) {
            QSqlRecord record = query.record();
            int userid = record.value("id").toInt();
            QString sUuid = record.value("uuid").toString();
            QString sEmail = record.value("email").toString();
            QString sNick = record.value("nick").toString().toHtmlEscaped();
            QString sUniversity = record.value("university").toString().toHtmlEscaped();
            int nRating = record.value("rating").toInt();
            QString sCreated = record.value("dt_created").toString().toHtmlEscaped();
            QString sDTLastLogin = record.value("dt_last_login").toString().toHtmlEscaped();
            QString sLastIP = record.value("last_ip").toString().toHtmlEscaped();
            QString sCountry = record.value("country").toString().toHtmlEscaped();
            QString sRegion = record.value("region").toString().toHtmlEscaped();
            QString sCity = record.value("city").toString().toHtmlEscaped();
            QString sRole = record.value("role").toString().toHtmlEscaped();
            QJsonObject user;
            user["id"] = userid;
            user["uuid"] = sUuid;
            user["nick"] = sNick;
            user["email"] = sEmail;
            user["created"] = sCreated;
            user["dt_last_login"] = sDTLastLogin;
            user["last_ip"] = sLastIP;
            user["university"] = sUniversity;
            user["rating"] = nRating;
            user["country"] = sCountry;
            user["region"] = sRegion;
            user["city"] = sCity;
            user["role"] = sRole;
            users.push_back(user);
        }
    }

	QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	jsonData["data"] = users;
    jsonData["onpage"] = nOnPage;
    jsonData["page"] = nPage;
    jsonData["count"] = nCount;
	pWebSocketServer->sendMessage(pClient, jsonData);
}
