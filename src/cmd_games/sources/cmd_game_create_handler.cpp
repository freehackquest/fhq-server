#include <cmd_game_create_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameCreateHandler::CmdGameCreateHandler(){
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
    m_vInputs.push_back(CmdInputDef("name").string_().required().description("Name of the Game"));
    m_vInputs.push_back(CmdInputDef("description").string_().required().description("Description of the Game"));
    m_vInputs.push_back(CmdInputDef("state").string_().required().description("State of the game"));
    m_vInputs.push_back(CmdInputDef("form").string_().required().description("Form of the game"));
    m_vInputs.push_back(CmdInputDef("type").string_().required().description("Type of the game"));
    m_vInputs.push_back(CmdInputDef("date_start").string_().required().description("Date start"));
    m_vInputs.push_back(CmdInputDef("date_stop").string_().required().description("Date stop"));
    m_vInputs.push_back(CmdInputDef("date_restart").string_().required().description("Date restart"));
    m_vInputs.push_back(CmdInputDef("organizators").string_().required().description("Organizators"));
}

std::string CmdGameCreateHandler::cmd(){
    return "game_create";
}

bool CmdGameCreateHandler::accessUnauthorized(){
	return false;
}

bool CmdGameCreateHandler::accessUser(){
	return false;
}

bool CmdGameCreateHandler::accessTester(){
	return false;
}

bool CmdGameCreateHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdGameCreateHandler::inputs(){
	return m_vInputs;
};

QString CmdGameCreateHandler::description(){
    return "Create the game";
}

QStringList CmdGameCreateHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGameCreateHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());

    QString sUUID = jsonRequest["uuid"].toString().trimmed();
	{
		QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE uuid = :uuid");
		query.bindValue(":uuid", sUUID);
		query.exec();
		if (query.next()) {
            pRequest->sendMessageError(cmd(), Error(403, "Game with uuid {" + sUUID + "} already exists"));
			return;
		}
	}

    QString sName = jsonRequest["name"].toString().trimmed();
    if(sName.length() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Name could not be empty"));
		return;
    }
	
    QString sDescription = jsonRequest["description"].toString().trimmed();
    QString sState = jsonRequest["state"].toString().trimmed();
    QString sForm = jsonRequest["form"].toString().trimmed();
    QString sType = jsonRequest["type"].toString().trimmed();
    QString sDateStart = jsonRequest["date_start"].toString().trimmed();
    QString sDateStop = jsonRequest["date_stop"].toString().trimmed();
    QString sDateRestart = jsonRequest["date_restart"].toString().trimmed();
    QString sOrganizators = jsonRequest["organizators"].toString().trimmed();

	QSqlQuery query(db);
	query.prepare(
        "INSERT INTO games("
		"		uuid,"
        "		title,"
        "		type_game,"
        "		date_create,"
        "		date_change,"
        "		date_start,"
        "		date_stop,"
        "		date_restart,"
        "		description,"
        "		organizators,"
        "		state,"
        "		form,"
        "		rules,"
        "		maxscore"
		"	)"
		"	VALUES("
		"		:uuid,"
        "		:title,"
        "		:type_game,"
        "		NOW(),"
        "		NOW(),"
        "		:date_start,"
        "		:date_stop,"
        "		:date_restart,"
        "		:description,"
        "		:organizators,"
        "		:state,"
        "		:form,"
        "		:rules,"
        "		:maxscore"
		"	)");
	query.bindValue(":uuid", sUUID);
    query.bindValue(":title", sName);
    query.bindValue(":type_game", sType);
    query.bindValue(":date_start", sDateStart);
    query.bindValue(":date_stop", sDateStop);
    query.bindValue(":date_restart", sDateRestart);
    query.bindValue(":description", sDescription);
    query.bindValue(":organizators", sOrganizators);
	query.bindValue(":state", sState);
    query.bindValue(":form", sForm);
    query.bindValue(":rules", "");
    query.bindValue(":maxscore", 0);

	if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
		return;
	}
	
	int rowid = query.lastInsertId().toInt();
    jsonResponse["questid"] = QJsonValue(rowid);

    RunTasks::AddPublicEvents(pRequest->server(), "games", "New game #" + QString::number(rowid) + " " + sName);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
