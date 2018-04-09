#include <cmd_update_user_location_handler.h>
#include <runtasks.h>
#include <QJsonArray>
#include <employ_database.h>

CmdUpdateUserLocationHandler::CmdUpdateUserLocationHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
	m_vInputs.push_back(CmdInputDef("userid").required().integer_().description("User ID"));
}

// ---------------------------------------------------------------------

std::string CmdUpdateUserLocationHandler::cmd(){
	return "updateuserlocation";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdUpdateUserLocationHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdUpdateUserLocationHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdUpdateUserLocationHandler::description(){
	return "This method will be try update user location by lastip";
}

// ---------------------------------------------------------------------

void CmdUpdateUserLocationHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

	// bool bConvert = false;

    int userid = jsonRequest["userid"].toInt();
	
	// TODO redesign
	if(userid == 0){
        pRequest->sendMessageError(cmd(), Errors::QuestIDMustBeNotZero());
		return;
	}

    QSqlDatabase db = *(pDatabase->database());
	QString lastip = "";
	{
		QSqlQuery query(db);
		query.prepare("SELECT * FROM users WHERE id = :userid");
		query.bindValue(":userid", userid);
		query.exec();
		if (query.next()) {
			QSqlRecord record = query.record();
			lastip = record.value("last_ip").toString();
		}
	}
	
	if(lastip == "" || lastip == NULL){
		QSqlQuery query(db);
		query.prepare("SELECT * FROM users_ips WHERE userid = :userid ORDER BY id DESC");
		query.bindValue(":userid", userid);
		query.exec();
		if (query.next()) {
			QSqlRecord record = query.record();
			lastip = record.value("ip").toString();
		}
	}

    RunTasks::UpdateUserLocation(pRequest->server(), userid, lastip);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
