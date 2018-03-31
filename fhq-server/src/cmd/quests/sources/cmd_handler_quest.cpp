#include <cmd_handler_quest.h>
#include <runtasks.h>
#include <log.h>
#include <employ_settings.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdHandlerQuest::CmdHandlerQuest(){
    TAG = "CmdHandlerQuest";

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
	m_vInputs.push_back(CmdInputDef("questid").integer_().required().description("Quest ID"));
}

// ---------------------------------------------------------------------

std::string CmdHandlerQuest::cmd(){
	return "quest";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerQuest::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerQuest::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdHandlerQuest::description(){
	return "Update the quest info";
}

// ---------------------------------------------------------------------

void CmdHandlerQuest::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());

    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QString sBaseGamesURL = pSettings->getSettString("server_folder_games_url");

    IUserToken *pUserToken = pRequest->userToken();
	bool bAdmin = false;
	int nUserID = 0;
	if(pUserToken != NULL) {
		bAdmin = pUserToken->isAdmin();
		nUserID = pUserToken->userid();
	}

    int nQuestID = jsonRequest["questid"].toInt();

	{
		QSqlQuery query(db);
		query.prepare("SELECT "
			"	q.idquest, "
			"	q.gameid, "
			"	q.name, "
			"	q.text, "
			"	q.answer_format, "
			"	q.score, "
			"	q.subject, "
			"	q.copyright, "
			"	q.state, "
			"	q.author, "
			"	q.count_user_solved, "
			"	q.answer, "
			"	q.description_state, "
			" 	users_quests.dt_passed as dt_passed2"
			" FROM "
			"	quest q "
			" LEFT JOIN "
			"	users_quests ON users_quests.questid = q.idquest AND users_quests.userid = :userid"
			" WHERE "
			"	q.idquest = :questid"
		);
		query.bindValue(":userid", nUserID);
		query.bindValue(":questid", nQuestID);
		if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
			return;
		}
		
		if (query.next()) {
			QSqlRecord record = query.record();
			QJsonObject quest;
			quest["id"] = record.value("idquest").toInt();
			int nGameID = record.value("gameid").toInt();
			QString sState = record.value("state").toString();
			QString sPassed = record.value("dt_passed2").toString();
			
			if(sState == "open" || bAdmin){
				quest["gameid"] = nGameID;
				quest["name"] = record.value("name").toString();
				quest["text"] = record.value("text").toString();
				quest["answer_format"] = record.value("answer_format").toString();
				quest["score"] = record.value("score").toInt();
				quest["subject"] = record.value("subject").toString();
				quest["copyright"] = record.value("copyright").toString();
				quest["state"] = record.value("state").toString();
				quest["author"] = record.value("author").toString();
				quest["count_user_solved"] = record.value("count_user_solved").toString();
			}
			
			// user completed quest
			quest["completed"] = !record.isNull("dt_passed2");
			quest["dt_passed"] = sPassed;
			
			if(bAdmin){
				quest["answer"] = record.value("answer").toString();
				quest["description_state"] = record.value("description_state").toString();
			}
            jsonResponse["quest"] = quest;
			
			// game info
			{
				QJsonObject game;
				QSqlQuery query_game(db);
				query_game.prepare("SELECT * FROM games WHERE id = :id");
				query_game.bindValue(":id", nGameID);
				if(!query_game.exec()){
                    pRequest->sendMessageError(cmd(), Error(500, query_game.lastError().text()));
					return;
				}
				if (query_game.next()) {
					QSqlRecord record_game = query_game.record();
                    int nGameID = record_game.value("id").toInt();
                    game["id"] = nGameID;
					game["title"] = record_game.value("title").toString();
                    game["logo"] = sBaseGamesURL + QString::number(nGameID) + ".png";
				}else{
                    pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
					return;
				}
                jsonResponse["game"] = game;
			}
			

			// files
			{
				QJsonArray files;
				QSqlQuery query_files(db);
				query_files.prepare("SELECT * FROM quests_files WHERE questid = :questid");
				query_files.bindValue(":questid", nQuestID);
				if(!query_files.exec()){
                    pRequest->sendMessageError(cmd(), Error(500, query_files.lastError().text()));
					return;
				}
				while (query_files.next()) {
					QSqlRecord record_game = query_files.record();
					QJsonObject fileinfo;
					fileinfo["id"] = record_game.value("id").toInt();
					fileinfo["uuid"] = record_game.value("uuid").toInt();
					fileinfo["filename"] = record_game.value("filename").toString();
					fileinfo["size"] = record_game.value("size").toString();
					fileinfo["dt"] = record_game.value("dt").toString();
					fileinfo["filepath"] = record_game.value("filepath").toString();
					files.append(fileinfo);
				}
                jsonResponse["files"] = files;
			}
	
			// hints
			{
				QJsonArray hints;
				QSqlQuery query_hints(db);
				query_hints.prepare("SELECT * FROM quests_hints WHERE questid = :questid");
				query_hints.bindValue(":questid", nQuestID);
				if(!query_hints.exec()){
                    pRequest->sendMessageError(cmd(), Error(500, query_hints.lastError().text()));
					return;
				}
				while (query_hints.next()) {
					QSqlRecord record_game = query_hints.record();
					QJsonObject hint;
					hint["id"] = record_game.value("id").toInt();
					hint["text"] = record_game.value("text").toString();
					hints.append(hint);
				}
                jsonResponse["hints"] = hints;
			}
	
		}else{
            pRequest->sendMessageError(cmd(), Error(404, "Quest not found"));
			return;
		}
	}

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
