#include <cmd_quest_update_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdQuestUpdateHandler::CmdQuestUpdateHandler(){
	m_vInputs.push_back(CmdInputDef("questid").integer_().required().description("Quest ID"));
	m_vInputs.push_back(CmdInputDef("name").string_().optional().description("Name of the quest"));
	
	m_vInputs.push_back(CmdInputDef("gameid").integer_().optional().description("Which game included this quest"));
	
	m_vInputs.push_back(CmdInputDef("text").string_().optional().description("Description of the quest"));
	m_vInputs.push_back(CmdInputDef("score").integer_().minval(1).maxval(1000).optional().description("How much append to user score after solve quest by them"));
	
	// TODO from database init
	QStringList questTypes;
	questTypes << "trivia";
	
	m_vInputs.push_back(CmdInputDef("subject").string_().optional().description("Subject must be one from types (look types)"));
	m_vInputs.push_back(CmdInputDef("answer").string_().optional().description("Answer for the quest"));
	m_vInputs.push_back(CmdInputDef("answer_format").string_().optional().description("Answer format for the quest"));
	m_vInputs.push_back(CmdInputDef("state").string_().optional().description("State of the quest, can be: open, broken, closed"));
	m_vInputs.push_back(CmdInputDef("description_state").string_().optional().description("You can add some descriptions for quest state"));
}

QString CmdQuestUpdateHandler::cmd(){
    return "quest_update";
}

bool CmdQuestUpdateHandler::accessUnauthorized(){
	return false;
}

bool CmdQuestUpdateHandler::accessUser(){
	return false;
}

bool CmdQuestUpdateHandler::accessTester(){
	return false;
}

bool CmdQuestUpdateHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdQuestUpdateHandler::inputs(){
	return m_vInputs;
};

QString CmdQuestUpdateHandler::description(){
	return "Update the quest info";
}

QStringList CmdQuestUpdateHandler::errors(){
	QStringList	list;
	return list;
}

void CmdQuestUpdateHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

	QJsonObject jsonData;
	jsonData["cmd"] = QJsonValue(cmd());
	QSqlDatabase db = *(pWebSocketServer->database());

	int nQuestID = obj["questid"].toInt();
	QString sNamePrev = "";
	QString sTextPrev = "";
	int nScorePrev = 0;
	int nGameIDPrev = 0;
	QString sSubjectPrev = "";
	QString sAnswerPrev = "";
	QString sAuthorPrev = "";
	QString sAnswerFormatPrev = "";
	QString sStatePrev = "";
	QString sCopyrightPrev = "";
	QString sDescriptionStatePrev = "";
	
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
			sNamePrev = record.value("name").toString();
			sTextPrev = record.value("text").toString();
			nScorePrev = record.value("score").toInt();
			nGameIDPrev = record.value("gameid").toInt();
			sSubjectPrev = record.value("subject").toString();
			sAnswerPrev = record.value("answer").toString();
			sAuthorPrev = record.value("author").toString();
			sAnswerFormatPrev = record.value("answer_format").toString();
			sStatePrev = record.value("state").toString();
			sCopyrightPrev = record.value("copyright").toString();
			sDescriptionStatePrev = record.value("description_state").toString();
		}else{
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Quest not found"));
			return;
		}
	}

	// Update name
	if(obj.contains("name")){
		QString sName = obj["name"].toString().trimmed();
		if(sName != sNamePrev){
			QSqlQuery query(db);
			query.prepare("UPDATE quest SET name = :name WHERE idquest = :questid");
			query.bindValue(":name", sName);
			query.bindValue(":questid", nQuestID);
			if (!query.exec()){
				pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
				return;
			}
			sNamePrev = sName;
			RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated name of quest #" + QString::number(nQuestID) + " from [" + sNamePrev + "] to [" + sName + "]");
		}
	}

	// Update gameid
	if(obj.contains("gameid")){
		int nGameID = obj["gameid"].toInt();
		{
			QSqlQuery query(db);
			query.prepare("SELECT * FROM games WHERE id = :id");
			query.bindValue(":id", nGameID);
			query.exec();
			if (!query.next()) {
				pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Game not found"));
				return;
			}
		}
		
		if(nGameID != nGameIDPrev){
			QSqlQuery query(db);
			query.prepare("UPDATE quest SET gameid = :gameid WHERE idquest = :questid");
			query.bindValue(":gameid", nGameID);
			query.bindValue(":questid", nQuestID);
			if (!query.exec()){
				pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
				return;
			}
			RunTasks::UpdateMaxScoreGame(pWebSocketServer,nGameID);
			RunTasks::UpdateMaxScoreGame(pWebSocketServer,nGameIDPrev);
			RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Moved quest #" + QString::number(nQuestID) + " from " + QString::number(nGameIDPrev) + " to " + QString::number(nGameID));
			nGameIDPrev = nGameID;
		}
	}
	
	// Update subject
	if(obj.contains("subject")){
		QString sSubject = obj["subject"].toString().trimmed();
		if(sSubject != sSubjectPrev){
			QSqlQuery query(db);
			query.prepare("UPDATE quest SET subject = :subject WHERE idquest = :questid");
			query.bindValue(":subject", sSubject);
			query.bindValue(":questid", nQuestID);
			if (!query.exec()){
				pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
				return;
			}
			RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated subject of quest #" + QString::number(nQuestID) + " " + sNamePrev);
			// TODO update skills of users in future
		}
	}
	
	// Update text
	if(obj.contains("text")){
		QString sText = obj["text"].toString().trimmed();
		if(sText != sTextPrev){
			QSqlQuery query(db);
			query.prepare("UPDATE quest SET text = :text WHERE idquest = :questid");
			query.bindValue(":text", sText);
			query.bindValue(":questid", nQuestID);
			if (!query.exec()){
				pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
				return;
			}
			RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated text of quest #" + QString::number(nQuestID) + " " + sNamePrev);
		}
	}
	
	// Update score
	if(obj.contains("score")){
		int nScore = obj["score"].toInt();
		if(nScore != nScorePrev){
			QSqlQuery query(db);
			query.prepare("UPDATE quest SET score = :score WHERE idquest = :questid");
			query.bindValue(":score", nScore);
			query.bindValue(":questid", nQuestID);
			if (!query.exec()){
				pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
				return;
			}
			RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated score of quest #" + QString::number(nQuestID) + " " + sNamePrev + " from " + QString::number(nScorePrev) + " to " + QString::number(nScore));
			RunTasks::UpdateMaxScoreGame(pWebSocketServer,nGameIDPrev);
			// TODO update users reting/score
		}
	}
	
	// Update answer
	if(obj.contains("answer")){
		QString sAnswer = obj["answer"].toString();
		if(sAnswer != sAnswerPrev){
			QSqlQuery query(db);
			query.prepare("UPDATE quest SET answer = :answer WHERE idquest = :questid");
			query.bindValue(":answer", sAnswer);
			query.bindValue(":questid", nQuestID);
			if (!query.exec()){
				pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
				return;
			}
			RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated answer of quest #" + QString::number(nQuestID) + " " + sNamePrev);
		}
	}
	
	// Update author
	if(obj.contains("author")){
		QString sAuthor = obj["author"].toString();
		if(sAuthor != sAuthorPrev){
			QSqlQuery query(db);
			query.prepare("UPDATE quest SET author = :author WHERE idquest = :questid");
			query.bindValue(":author", sAuthor);
			query.bindValue(":questid", nQuestID);
			if (!query.exec()){
				pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
				return;
			}
			RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated author of quest #" + QString::number(nQuestID) + " " + sNamePrev);
		}
	}
	
	// Update answer format
	if(obj.contains("answer_format")){
		QString sAnswerFormat = obj["answer_format"].toString();
		if(sAnswerFormat != sAnswerFormatPrev){
			QSqlQuery query(db);
			query.prepare("UPDATE quest SET answer_format = :answer_format WHERE idquest = :questid");
			query.bindValue(":answer_format", sAnswerFormat);
			query.bindValue(":questid", nQuestID);
			if (!query.exec()){
				pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
				return;
			}
			RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated answer format of quest #" + QString::number(nQuestID) + " " + sNamePrev + " from [" + sAnswerFormatPrev + "] to [" + sAnswerFormat + "]");
		}
	}
	
	// Update state
	if(obj.contains("state")){
		QString sState = obj["state"].toString();
		if(sState != sStatePrev){
			QSqlQuery query(db);
			query.prepare("UPDATE quest SET state = :state WHERE idquest = :questid");
			query.bindValue(":state", sState);
			query.bindValue(":questid", nQuestID);
			if (!query.exec()){
				pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
				return;
			}
			RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated state of quest #" + QString::number(nQuestID) + " " + sNamePrev + " from [" + sStatePrev + "] to [" + sState + "]");
		}
	}
	
	// Update copyright
	if(obj.contains("copyright")){
		QString sCopyright = obj["copyright"].toString();
		if(sCopyright != sCopyrightPrev){
			QSqlQuery query(db);
			query.prepare("UPDATE quest SET copyright = :copyright WHERE idquest = :questid");
			query.bindValue(":copyright", sCopyright);
			query.bindValue(":questid", nQuestID);
			if (!query.exec()){
				pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
				return;
			}
			RunTasks::AddPublicEvents(pWebSocketServer, "quests", "Updated copyright of quest #" + QString::number(nQuestID) + " " + sNamePrev + " from [" + sCopyrightPrev + "] to [" + sCopyright + "]");
		}
	}
	
	// Update copyright
	if(obj.contains("description_state")){
		QString sDescriptionState = obj["description_state"].toString();
		if(sDescriptionState != sDescriptionStatePrev){
			QSqlQuery query(db);
			query.prepare("UPDATE quest SET description_state = :description_state WHERE idquest = :questid");
			query.bindValue(":description_state", sDescriptionState);
			query.bindValue(":questid", nQuestID);
			if (!query.exec()){
				pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
				return;
			}
			// nothing to inform
		}
	}
	
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	pWebSocketServer->sendMessage(pClient, jsonData);
}
