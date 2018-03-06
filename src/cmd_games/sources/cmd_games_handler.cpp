#include <cmd_games_handler.h>
#include <QJsonArray>
#include <employ_settings.h>

CmdGamesHandler::CmdGamesHandler(){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
	// m_vInputs.push_back(CmdInputDef("filter_text").string_().optional().description("Filter by user email or nick"));
	// m_vInputs.push_back(CmdInputDef("filter_role").string_().optional().description("Filter by user role"));
}

// ---------------------------------------------------------------------

std::string CmdGamesHandler::cmd(){
	return "games";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGamesHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGamesHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdGamesHandler::description(){
	return "Method returned list of games";
}

// ---------------------------------------------------------------------

void CmdGamesHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;


    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QString base_url = pSettings->getSettString("server_folder_games_url");

	QJsonArray games;
    QSqlDatabase db = *(pRequest->server()->database());

	QSqlQuery query(db);
	query.prepare("SELECT * FROM games ORDER BY games.date_start");

    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    while (query.next()) {
		QSqlRecord record = query.record();
		QJsonObject game;
        int nGameID = record.value("id").toInt();
        game["id"] = nGameID;
		game["uuid"] = record.value("uuid").toString();
		game["title"] = record.value("title").toString();
		game["type_game"] = record.value("type_game").toString();
		game["date_start"] = record.value("date_start").toString();
		game["date_stop"] = record.value("date_stop").toString();
		game["date_restart"] = record.value("date_restart").toString();
		game["description"] = record.value("description").toString();
		game["state"] = record.value("state").toString();
		game["form"] = record.value("form").toString();
        game["logo"] = base_url + QString::number(nGameID) + ".png";
		game["organizators"] = record.value("organizators").toString();
		game["maxscore"] = record.value("maxscore").toInt();
		games.push_back(game);
	}

    jsonResponse["data"] = games;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
