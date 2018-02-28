#include <cmd_quest_proposal_handler.h>
#include <QJsonArray>
#include <SmtpMime>
#include <memory_cache_serverinfo.h>
#include <memory_cache_serversettings.h>

CmdQuestProposalHandler::CmdQuestProposalHandler(){
    TAG = "CmdQuestProposalHandler";
    m_vInputs.push_back(CmdInputDef("gameid").integer_().required().description("Which game included this quest"));
    m_vInputs.push_back(CmdInputDef("name").string_().required().description("Name of the quest"));
    m_vInputs.push_back(CmdInputDef("text").string_().required().description("Description of the quest"));
    m_vInputs.push_back(CmdInputDef("score").integer_().minval(1).maxval(1000).required().description("How much append to user score after solve quest by them"));

    QStringList questTypes; // TODO fill from database or use employees
    questTypes << "trivia";
    m_vInputs.push_back(CmdInputDef("author").string_().required().description("Author of the quest"));
    m_vInputs.push_back(CmdInputDef("subject").string_().required().description("Subject must be one from types (look types)"));
    m_vInputs.push_back(CmdInputDef("answer").string_().required().description("Answer for the quest"));
    m_vInputs.push_back(CmdInputDef("answer_format").string_().required().description("Answer format for the quest"));
}

std::string CmdQuestProposalHandler::cmd(){
        return "quest_proposal";
}

bool CmdQuestProposalHandler::accessUnauthorized(){
        return false;
}

bool CmdQuestProposalHandler::accessUser(){
	return true;
}

bool CmdQuestProposalHandler::accessTester(){
	return true;
}

bool CmdQuestProposalHandler::accessAdmin(){
	return true;
}

const QVector<CmdInputDef> &CmdQuestProposalHandler::inputs(){
	return m_vInputs;
};

QString CmdQuestProposalHandler::description(){
        return "Add quest proposal ";
}

QStringList CmdQuestProposalHandler::errors(){
	QStringList	list;
	return list;
}

void CmdQuestProposalHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));

    IMemoryCache *pMemoryCache = pWebSocketServer->findMemoryCache("serverinfo");
    if(pMemoryCache == NULL){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
        return;
    }
    MemoryCacheServerInfo *pMemoryCacheServerInfo = dynamic_cast<MemoryCacheServerInfo*>(pMemoryCache);

    IMemoryCache *pMemoryCache2 = pWebSocketServer->findMemoryCache("serversettings");
    if(pMemoryCache2 == NULL){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::InternalServerError());
        return;
    }
    MemoryCacheServerSettings *pMemoryCacheServerSettings = dynamic_cast<MemoryCacheServerSettings*>(pMemoryCache2);

    QSqlDatabase db = *(pWebSocketServer->database());

    IUserToken *pUserToken = pWebSocketServer->getUserToken(pClient);
    int nUserID = 0;
    QString sUserEmail = "";
    if(pUserToken != NULL) {
        nUserID = pUserToken->userid();
        sUserEmail = pUserToken->email();
    }

    // check the user already sended quest

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
    if(sName.length() == 0){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(400, "Name could not be empty"));
        return;
    }

    QString sText = obj["text"].toString().trimmed();

    int nScore = obj["score"].toInt();
    QString sSubject = obj["subject"].toString().trimmed();
    QString sAnswer = obj["answer"].toString().trimmed();

    if(sAnswer.length() == 0){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(400, "Answer could not be empty"));
        return;
    }

    QString sAuthor = obj["author"].toString().trimmed();

    if(sAuthor.length() == 0){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(400, "Author could not be empty"));
        return;
    }

    QString sAnswerFormat = obj["answer_format"].toString().trimmed();

    if(sAnswerFormat.length() == 0){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(400, "Answer Format could not be empty"));
        return;
    }

    QString sCopyright = obj["copyright"].toString().trimmed();

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO quests_proposal("
        "		userid,"
        "		gameid,"
        "		name,"
        "		text,"
        "		answer,"
        "		answer_format,"
        "		score,"
        "		author,"
        "		subject,"
        "		copyright,"
        "		created,"
        "		updated,"
        "		confirmed"
        "	)"
        "	VALUES("
        "		:userid,"
        "		:gameid,"
        "		:name,"
        "		:text,"
        "		:answer,"
        "		:answer_format,"
        "		:score,"
        "		:author,"
        "		:subject,"
        "		:copyright,"
        "		NOW(),"
        "		NOW(),"
        "		:confirmed"
        "	)");
    query.bindValue(":userid", nUserID);
    query.bindValue(":gameid", nGameID);
    query.bindValue(":name", sName);
    query.bindValue(":text", sText);
    query.bindValue(":answer", sAnswer);
    query.bindValue(":answer_format", sAnswerFormat);
    query.bindValue(":score", nScore);
    query.bindValue(":author", sAuthor);
    query.bindValue(":subject", sSubject);
    query.bindValue(":copyright", sCopyright);
    query.bindValue(":confirmed", 0);

    if (!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }
    // pMemoryCacheServerInfo->incrementQuests();


    int nQuestProposalId = query.lastInsertId().toInt();
    jsonData["questid"] = QJsonValue(nQuestProposalId);

    // TODO move to tasks
    QString sMailHost = pMemoryCacheServerSettings->getSettString("mail_host");
    int nMailPort = pMemoryCacheServerSettings->getSettInteger("mail_port");
    QString sMailPassword = pMemoryCacheServerSettings->getSettPassword("mail_password");
    QString sMailFrom = pMemoryCacheServerSettings->getSettString("mail_from");
    QString sMailToAdmin = pMemoryCacheServerSettings->getSettString("mail_system_message_admin_email");

    SmtpClient smtp(sMailHost, nMailPort, SmtpClient::SslConnection);
    smtp.setUser(sMailFrom);
    smtp.setPassword(sMailPassword);

    MimeMessage message;

    EmailAddress sender(sMailFrom, "FreeHackQuest");
    message.setSender(&sender);

    EmailAddress to(sMailToAdmin, "");
    message.addRecipient(&to);

    message.setSubject("Quest Proposal (FreeHackQuest)");
    MimeText text;
    text.setText("Quest Proposal \n"
                 "UserID: " + QString::number(nUserID) + "\n"
                 "From: " + sUserEmail + "\n"
                 "QuestProposalId: #" + QString::number(nQuestProposalId) + "\n"
              );

    message.addPart(&text);

    // Now we can send the mail
    if (!smtp.connectToHost()) {
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, "[MAIL] Failed to connect to host!"));
        return;
    }

    if (!smtp.login()) {
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, "[MAIL] Failed to login!"));
        return;
    }

    if (!smtp.sendMail(message)) {
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, "[MAIL] Failed to send mail!"));
        return;
    }
    smtp.quit();


    jsonData["result"] = QJsonValue("DONE");
    jsonData["m"] = QJsonValue(m);
    pWebSocketServer->sendMessage(pClient, jsonData);
}
