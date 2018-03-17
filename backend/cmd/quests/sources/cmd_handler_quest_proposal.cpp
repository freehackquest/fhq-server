#include <cmd_handler_quest_proposal.h>
#include <QJsonArray>
#include <SmtpMime>
// #include <memory_cache_serverinfo.h>
#include <employ_settings.h>

CmdHandlerQuestProposal::CmdHandlerQuestProposal(){
    TAG = "CmdQuestProposalHandler";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
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

// ---------------------------------------------------------------------

std::string CmdHandlerQuestProposal::cmd(){
        return "quest_proposal";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerQuestProposal::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerQuestProposal::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdHandlerQuestProposal::description(){
        return "Add quest proposal ";
}

// ---------------------------------------------------------------------

void CmdHandlerQuestProposal::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QSqlDatabase db = *(pRequest->server()->database());

    IUserToken *pUserToken = pRequest->userToken();
    int nUserID = 0;
    QString sUserEmail = "";
    if(pUserToken != NULL) {
        nUserID = pUserToken->userid();
        sUserEmail = pUserToken->email();
    }

    // check the user already sended quest

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
    if(sName.length() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Name could not be empty"));
        return;
    }

    QString sText = jsonRequest["text"].toString().trimmed();

    int nScore = jsonRequest["score"].toInt();
    QString sSubject = jsonRequest["subject"].toString().trimmed();
    QString sAnswer = jsonRequest["answer"].toString().trimmed();

    if(sAnswer.length() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Answer could not be empty"));
        return;
    }

    QString sAuthor = jsonRequest["author"].toString().trimmed();

    if(sAuthor.length() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Author could not be empty"));
        return;
    }

    QString sAnswerFormat = jsonRequest["answer_format"].toString().trimmed();

    if(sAnswerFormat.length() == 0){
        pRequest->sendMessageError(cmd(), Error(400, "Answer Format could not be empty"));
        return;
    }

    QString sCopyright = jsonRequest["copyright"].toString().trimmed();

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
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    // pMemoryCacheServerInfo->incrementQuests();


    int nQuestProposalId = query.lastInsertId().toInt();
    jsonResponse["questid"] = QJsonValue(nQuestProposalId);

    // TODO move to tasks
    QString sMailHost = pSettings->getSettString("mail_host");
    int nMailPort = pSettings->getSettInteger("mail_port");
    QString sMailPassword = pSettings->getSettPassword("mail_password");
    QString sMailFrom = pSettings->getSettString("mail_from");
    QString sMailToAdmin = pSettings->getSettString("mail_system_message_admin_email");

    SmtpClient smtp(sMailHost, nMailPort, SmtpClient::SslConnection);
    smtp.setUser(sMailFrom);
    smtp.setPassword(sMailPassword);

    MimeMessage message;

    EmailAddress sender(sMailFrom, "FreeHackQuest");
    message.setSender(&sender);

    EmailAddress to(sMailToAdmin, "");
    message.addRecipient(&to);
    // message.addRecipient(&sUserEmail);

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
        pRequest->sendMessageError(cmd(), Error(500, "[MAIL] Failed to connect to host!"));
        return;
    }

    if (!smtp.login()) {
        pRequest->sendMessageError(cmd(), Error(500, "[MAIL] Failed to login!"));
        return;
    }

    if (!smtp.sendMail(message)) {
        pRequest->sendMessageError(cmd(), Error(500, "[MAIL] Failed to send mail!"));
        return;
    }
    smtp.quit();


    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
