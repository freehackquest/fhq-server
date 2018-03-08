#include <cmd_game_delete_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameDeleteHandler::CmdGameDeleteHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
    m_vInputs.push_back(CmdInputDef("admin_password").string_().required().description("Admin Password"));
}

// ---------------------------------------------------------------------

std::string CmdGameDeleteHandler::cmd(){
    return "game_delete";
}
// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGameDeleteHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGameDeleteHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdGameDeleteHandler::description(){
    return "Remove game and all quests";
}

// ---------------------------------------------------------------------

void CmdGameDeleteHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QString sUuid = jsonRequest["uuid"].toString().trimmed();
    QString sAdminPassword = jsonRequest["admin_password"].toString();
	
    IUserToken *pUserToken = pRequest->userToken();
    int nUserID = pUserToken->userid();

    QSqlDatabase db = *(pRequest->server()->database());

    // check admin password
	{
		QSqlQuery query(db);
		query.prepare("SELECT * FROM users WHERE id = :userid");
		query.bindValue(":userid", nUserID);
		if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
			return;
		}

		QString sPass = "";
		QString sEmail = "";

		if (query.next()) {
			QSqlRecord record = query.record();
			sEmail = record.value("email").toString();
			sPass = record.value("pass").toString();
		}else{
            pRequest->sendMessageError(cmd(), Errors::NotFound("user"));
			return;
		}

        QString sAdminPasswordHash = sEmail.toUpper() + sAdminPassword;
        sAdminPasswordHash = QString("%1").arg(QString(QCryptographicHash::hash(sAdminPasswordHash.toUtf8(),QCryptographicHash::Sha1).toHex()));

        if(sAdminPasswordHash != sPass){
            pRequest->sendMessageError(cmd(), Error(401, "Wrong password"));
			return;
		}
	}

    int nGameID = 0;

    // check existing game
	{
		QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE uuid = :uuid");
        query.bindValue(":uuid", sUuid);
		
		if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
			return;
		}

		if(!query.next()) {
            pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
			return;
        }else{
            QSqlRecord record = query.record();
            nGameID = record.value("id").toInt();
        }
	}

	// delete from users_games
	{
		QSqlQuery query_del(db);
		query_del.prepare("DELETE FROM users_games WHERE gameid = :gameid");
		query_del.bindValue(":gameid", nGameID);
		if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
			return;
		}
	}
	
	// delete from users_quests_answers
	{
		QSqlQuery query_del(db);
		query_del.prepare("DELETE FROM users_quests_answers WHERE questid IN (SELECT idquest FROM quest q WHERE q.gameid = :gameid)");
		query_del.bindValue(":gameid", nGameID);
		if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
			return;
		}
	}
	
	// delete from users_quests
	{
		QSqlQuery query_del(db);
		query_del.prepare("DELETE FROM users_quests WHERE questid IN (SELECT idquest FROM quest q WHERE q.gameid = :gameid)");
		query_del.bindValue(":gameid", nGameID);
		if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
			return;
		}
	}
	
	// delete from quests
	{
		QSqlQuery query_del(db);
		query_del.prepare("DELETE FROM quest WHERE gameid = :gameid");
		query_del.bindValue(":gameid", nGameID);
		if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
			return;
		}
	}

	// delete from games
	{
		QSqlQuery query_del(db);
		query_del.prepare("DELETE FROM games WHERE id = :gameid");
		query_del.bindValue(":gameid", nGameID);
		if(!query_del.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query_del.lastError().text()));
			return;
		}
	}

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
