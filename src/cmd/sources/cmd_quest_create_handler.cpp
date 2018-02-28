#include <cmd_quest_create_handler.h>
#include <runtasks.h>
#include <memory_cache_serverinfo.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdCreateQuestHandler::CmdCreateQuestHandler(){
	m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the quest"));
	m_vInputs.push_back(CmdInputDef("gameid").integer_().required().description("Which game included this quest"));
	m_vInputs.push_back(CmdInputDef("name").string_().required().description("Name of the quest"));
	m_vInputs.push_back(CmdInputDef("text").string_().required().description("Description of the quest"));
	m_vInputs.push_back(CmdInputDef("score").integer_().minval(1).maxval(1000).required().description("How much append to user score after solve quest by them"));
	
	// TODO from database init
	QStringList questTypes;
	questTypes << "trivia";
    m_vInputs.push_back(CmdInputDef("author").string_().required().description("Author of the quest"));
	m_vInputs.push_back(CmdInputDef("subject").string_().required().description("Subject must be one from types (look types)"));
	m_vInputs.push_back(CmdInputDef("answer").string_().required().description("Answer for the quest"));
	m_vInputs.push_back(CmdInputDef("answer_format").string_().required().description("Answer format for the quest"));
	m_vInputs.push_back(CmdInputDef("state").string_().required().description("State of the quest, can be: open, broken, closed"));
	m_vInputs.push_back(CmdInputDef("description_state").string_().required().description("You can add some descriptions for quest state"));
}

std::string CmdCreateQuestHandler::cmd(){
	return "createquest";
}

bool CmdCreateQuestHandler::accessUnauthorized(){
	return false;
}

bool CmdCreateQuestHandler::accessUser(){
	return false;
}

bool CmdCreateQuestHandler::accessTester(){
	return false;
}

bool CmdCreateQuestHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdCreateQuestHandler::inputs(){
	return m_vInputs;
};

QString CmdCreateQuestHandler::description(){
	return "Create the quest";
}

QStringList CmdCreateQuestHandler::errors(){
	QStringList	list;
	return list;
}

void CmdCreateQuestHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

	QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));

    IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serverinfo");
    if(pMemoryCache == NULL){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
        return;
    }
    MemoryCacheServerInfo *pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);

	QSqlDatabase db = *(pWebSocketServer->database());

	QString sUUID = obj["uuid"].toString().trimmed();
	{
		QSqlQuery query(db);
		query.prepare("SELECT * FROM quest WHERE uuid = :uuid");
		query.bindValue(":uuid", sUUID);
		query.exec();
		if (query.next()) {
			pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(403, "Quest with uuid {" + sUUID + "} already exists"));
			return;
		}
	}
	
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

	QString sName = obj["name"].toString().trimmed();
	/*if(sName.length() == 0){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(400, "Name could not be empty"));
		return;
	}*/
	
	QString sText = obj["text"].toString().trimmed();
	int nScore = obj["score"].toInt();
	QString sSubject = obj["subject"].toString().trimmed();
	QString sAnswer = obj["answer"].toString().trimmed();
	QString sAuthor = obj["author"].toString().trimmed();
	QString sAnswerFormat = obj["answer_format"].toString().trimmed();
	QString sState = obj["state"].toString().trimmed();
	QString sCopyright = obj["copyright"].toString().trimmed();
	QString sDescriptionState = obj["description_state"].toString().trimmed();

	QSqlQuery query(db);
	query.prepare(
		"INSERT INTO quest("
		"		uuid,"
		"		gameid,"
		"		name,"
		"		text,"
		"		answer,"
		"		answer_upper_md5,"
		"		answer_format,"
		"		score,"
		"		author,"
		"		subject,"
		"		state,"
		"		description_state,"
		"		copyright,"
		"		count_user_solved,"
		"		date_change,"
		"		date_create"
		"	)"
		"	VALUES("
		"		:uuid,"
		"		:gameid,"
		"		:name,"
		"		:text,"
		"		:answer,"
		"		:answer_upper_md5,"
		"		:answer_format,"
		"		:score,"
		"		:author,"
		"		:subject,"
		"		:state,"
		"		:description_state,"
		"		:copyright,"
		"		:count_user_solved,"
		"		NOW(),"
		"		NOW()"
		"	)");
	query.bindValue(":uuid", sUUID);
	query.bindValue(":gameid", nGameID);
	query.bindValue(":name", sName);
	query.bindValue(":text", sText);
	query.bindValue(":answer", sAnswer);
	QString sAnswerUpperMd5 = QString(QCryptographicHash::hash(sAnswer.toUpper().toUtf8(),QCryptographicHash::Md5).toHex());
	query.bindValue(":answer_upper_md5", sAnswerUpperMd5);
	query.bindValue(":answer_format", sAnswerFormat);
	query.bindValue(":score", nScore);
	query.bindValue(":author", sAuthor);
	query.bindValue(":subject", sSubject);
	query.bindValue(":state", sState);
	query.bindValue(":description_state", sDescriptionState);
	query.bindValue(":copyright", sCopyright);
	query.bindValue(":count_user_solved", 0);

	if (!query.exec()){
		pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
		return;
	}
    pMemoryCacheServerInfo->incrementQuests();

	
	int rowid = query.lastInsertId().toInt();
	jsonData["questid"] = QJsonValue(rowid);

	RunTasks::AddPublicEvents(pWebSocketServer, "quests", "New quest #" + QString::number(rowid) + " " + sName + " (subject: " + sSubject + ")");
	RunTasks::UpdateMaxScoreGame(pWebSocketServer,nGameID);
	
	jsonData["result"] = QJsonValue("DONE");
	jsonData["m"] = QJsonValue(m);
	pWebSocketServer->sendMessage(pClient, jsonData);
}
