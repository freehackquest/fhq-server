#include <cmd_quest_pass_handler.h>
#include <runtasks.h>
#include <log.h>
#include <utils.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdQuestPassHandler::CmdQuestPassHandler(){
	m_vInputs.push_back(CmdInputDef("questid").integer_().required().description("Quest ID"));
    m_vInputs.push_back(CmdInputDef("answer").string_().required().description("Answer"));
    TAG = "CmdQuestPassHandler";
}

QString CmdQuestPassHandler::cmd(){
    return "quest_pass";
}

bool CmdQuestPassHandler::accessUnauthorized(){
    return false;
}

bool CmdQuestPassHandler::accessUser(){
	return true;
}

bool CmdQuestPassHandler::accessTester(){
	return true;
}

bool CmdQuestPassHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdQuestPassHandler::inputs(){
	return m_vInputs;
};

QString CmdQuestPassHandler::description(){
	return "Update the quest info";
}

QStringList CmdQuestPassHandler::errors(){
	QStringList	list;
	return list;
}

void CmdQuestPassHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){
	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	QSqlDatabase db = *(pWebSocketServer->database());

	IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
	int nUserID = 0;
    QString sNick = "";
	if(pUserToken != NULL) {
		nUserID = pUserToken->userid();
        sNick = pUserToken->nick();
	}

	int nQuestID = obj["questid"].toInt();
    QString sUserAnswer = obj["answer"].toString().trimmed();

    QString sState = "";
    QString sQuestAnswer = "";
    QString sQuestName = "";
    int nGameID = 0;
	{
		QSqlQuery query(db);
        query.prepare("SELECT * FROM quest WHERE idquest = :questid");
        query.bindValue(":questid", nQuestID);
        if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
		
		if (query.next()) {
			QSqlRecord record = query.record();
            sState = record.value("state").toString();
            sQuestAnswer = record.value("answer").toString().trimmed();
            sQuestName = record.value("name").toString().trimmed();
            nGameID = record.value("gameid").toInt();
		}else{
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Quest not found"));
			return;
		}
	}

    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM games WHERE id = :gameid AND (NOW() < date_stop OR NOW() > date_restart)");
        query.bindValue(":gameid", nGameID);
        if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }

        if (!query.next()) {
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(403, "Game ended. Please wait date of restart."));
            return;
        }
    }

    // check passed quest
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) as cnt FROM users_quests WHERE questid = :questid AND userid = :userid");
        query.bindValue(":questid", nQuestID);
        query.bindValue(":userid", nUserID);
        if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            if(record.value("cnt").toInt() > 0){
                pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Quest already passed"));
                return;
            }
        }
    }

    // check answer
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) as cnt FROM users_quests_answers WHERE user_answer = :user_asnwer AND questid = :questid AND userid = :userid");
        query.bindValue(":user_answer", sUserAnswer);
        query.bindValue(":questid", nQuestID);
        query.bindValue(":userid", nUserID);
        if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            if(record.value("cnt").toInt() > 0){
                pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Your already try this answer."));
                return;
            }
        }
    }

    bool bPassed = sQuestAnswer.toUpper() == sUserAnswer.toUpper();
    QString sPassed = bPassed ? "Yes" : "No";

    // insert to user tries
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO users_quests_answers(userid, questid, user_answer, quest_answer, passed, levenshtein, dt) "
                      "VALUES(:userid, :questid, :user_answer, :quest_answer, :passed, :levenshtein, NOW())");
        query.bindValue(":userid", nUserID);
        query.bindValue(":questid", nQuestID);
        query.bindValue(":user_answer", sUserAnswer);
        query.bindValue(":quest_answer", sQuestAnswer);
        query.bindValue(":passed", sPassed);
        int nLevenshtein = UtilsLevenshtein::distance(sUserAnswer.toUpper(), sQuestAnswer.toUpper());
        query.bindValue(":levenshtein", nLevenshtein);

        if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
    }

    if(!bPassed){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(403, "Answer incorrect"));
        return;
    }

    // insert to user passed quests
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO users_quests(userid, questid, dt_passed) "
                      "VALUES(:userid, :questid, NOW())");
        query.bindValue(":userid", nUserID);
        query.bindValue(":questid", nQuestID);
        if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
    }


    RunTasks::AddPublicEvents(pWebSocketServer, "quests", "User #" + QString::number(nUserID) + "  " + sNick
                              + " passed quest #" + QString::number(nQuestID) + " " + sQuestName);

    RunTasks::UpdateUserRating(pWebSocketServer, nUserID);
    RunTasks::UpdateQuestSolved(pWebSocketServer, nQuestID);

	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	pWebSocketServer->sendMessage(pClient, jsonData);
}
