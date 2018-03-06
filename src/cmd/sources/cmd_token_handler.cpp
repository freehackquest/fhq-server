#include <cmd_token_handler.h>
#include <runtasks.h>
#include <log.h>
#include <model_usertoken.h>

CmdTokenHandler::CmdTokenHandler(){
    TAG = "CmdTokenHandler";

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(false);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("token").string_().optional().description("Auth token"));
}

// ---------------------------------------------------------------------

std::string CmdTokenHandler::cmd(){
    return "token";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdTokenHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdTokenHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdTokenHandler::description(){
    return "Method for login by token";
}

// ---------------------------------------------------------------------

void CmdTokenHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

	QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
	
    if(!jsonRequest.contains("token")){
        pRequest->sendMessageError(cmd(), Errors::NotFound("requred parameter token"));
		return;
	}
    QString token = jsonRequest["token"].toString();
    QSqlDatabase db = *(pRequest->server()->database());

	QSqlQuery query(db);
	query.prepare("SELECT * FROM users_tokens WHERE token = :token");
	query.bindValue(":token", token);
	if(!query.exec()){
		Log::err(TAG, query.lastError().text());
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
		return;
	}
	if (query.next()) {
		QSqlRecord record = query.record();
        int userid = record.value("userid").toInt();
		QString status = record.value("status").toString();
		QString data = record.value("data").toString();
		QString start_date = record.value("start_date").toString();
		QString end_date = record.value("end_date").toString();
        QString lastip = pRequest->client()->peerAddress().toString();
        pRequest->server()->setUserToken(pRequest->client(), new ModelUserToken(data));
        Log::info(TAG, "userid: " + QString::number(userid));
        // TODO redesign this
        RunTasks::UpdateUserLocation(pRequest->server(), userid, lastip);
	}else{
		Log::err(TAG, "Invalid token " + token);
		// ["error"]
        pRequest->sendMessageError(cmd(), Errors::InvalidToken());
		return;
	}

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
