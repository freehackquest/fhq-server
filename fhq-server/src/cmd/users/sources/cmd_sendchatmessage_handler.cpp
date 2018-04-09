#include <cmd_sendchatmessage_handler.h>
#include <employ_database.h>

CmdSendChatMessageHandler::CmdSendChatMessageHandler(){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);
}

// ---------------------------------------------------------------------

std::string CmdSendChatMessageHandler::cmd(){
	return "sendchatmessage";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdSendChatMessageHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdSendChatMessageHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdSendChatMessageHandler::description(){
	return "Method will be keep message and it sent to another users";
}

// ---------------------------------------------------------------------

void CmdSendChatMessageHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    IUserToken *pUserToken = pRequest->userToken();
	QString username = "";
	if(pUserToken != NULL){
		username = pUserToken->nick();
	}else{
		username = "Guest";
	}

    pRequest->sendMessageSuccess(cmd(), jsonResponse);

    QSqlDatabase db = *(pDatabase->database());
	QSqlQuery query(db);
	query.prepare("INSERT INTO chatmessages(user, message, dt) VALUES(:user,:message, NOW())");
	query.bindValue(":user", username);
    query.bindValue(":message", jsonRequest["message"].toString());
	query.exec();
	
	QJsonObject jsonData2;
	jsonData2["cmd"] = QJsonValue("chat");
    jsonData2["type"] = jsonRequest["type"];
	jsonData2["user"] = username;
    jsonData2["message"] = jsonRequest["message"];
	jsonData2["dt"] = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss");

    pRequest->server()->sendToAll(jsonData2);
}
