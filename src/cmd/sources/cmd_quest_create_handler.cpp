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

void CmdCreateQuestHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    IMemoryCache *pMemoryCache = pRequest->server()->findMemoryCache("serverinfo");
    if(pMemoryCache == NULL){
        pRequest->sendMessageError(cmd(), Errors::InternalServerError());
        return;
    }
    MemoryCacheServerInfo *pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);

    QSqlDatabase db = *(pRequest->server()->database());

    QString sUUID = jsonRequest["uuid"].toString().trimmed();
	{
		QSqlQuery query(db);
		query.prepare("SELECT * FROM quest WHERE uuid = :uuid");
		query.bindValue(":uuid", sUUID);
		query.exec();
		if (query.next()) {
            pRequest->sendMessageError(cmd(), Error(403, "Quest with uuid {" + sUUID + "} already exists"));
			return;
		}
	}
	
    int nGameID = jsonRequest["gameid"].toInt();
	{
		QSqlQuery query(db);
		query.prepare("SELECT * FROM games WHERE id = :id");
		query.bindValue(":id", nGameID);
		query.exec();
		if (!query.next()) {
            pRequest->sendMessageError(cmd(), Error(404, "Game not found"));
			return;
		}
	}

    QString sName = jsonRequest["name"].toString().trimmed();
	/*if(sName.length() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Name could not be empty"));
		return;
	}*/
	
    QString sText = jsonRequest["text"].toString().trimmed();
    int nScore = jsonRequest["score"].toInt();
    QString sSubject = jsonRequest["subject"].toString().trimmed();
    QString sAnswer = jsonRequest["answer"].toString().trimmed();
    QString sAuthor = jsonRequest["author"].toString().trimmed();
    QString sAnswerFormat = jsonRequest["answer_format"].toString().trimmed();
    QString sState = jsonRequest["state"].toString().trimmed();
    QString sCopyright = jsonRequest["copyright"].toString().trimmed();
    QString sDescriptionState = jsonRequest["description_state"].toString().trimmed();

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
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
		return;
	}
    pMemoryCacheServerInfo->incrementQuests();

	
	int rowid = query.lastInsertId().toInt();
    jsonResponse["questid"] = QJsonValue(rowid);

    RunTasks::AddPublicEvents(pRequest->server(), "quests", "New quest #" + QString::number(rowid) + " " + sName + " (subject: " + sSubject + ")");
    RunTasks::UpdateMaxScoreGame(pRequest->server(),nGameID);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
