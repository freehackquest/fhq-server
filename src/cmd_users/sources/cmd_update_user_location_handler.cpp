#include <cmd_update_user_location_handler.h>
#include <runtasks.h>
#include <QJsonArray>

CmdUpdateUserLocationHandler::CmdUpdateUserLocationHandler(){
	m_vInputs.push_back(CmdInputDef("userid").required().integer_().description("User ID"));
}

std::string CmdUpdateUserLocationHandler::cmd(){
	return "updateuserlocation";
}

bool CmdUpdateUserLocationHandler::accessUnauthorized(){
	return false;
}

bool CmdUpdateUserLocationHandler::accessUser(){
	return false;
}

bool CmdUpdateUserLocationHandler::accessTester(){
	return false;
}

bool CmdUpdateUserLocationHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdUpdateUserLocationHandler::inputs(){
	return m_vInputs;
};

QString CmdUpdateUserLocationHandler::description(){
	return "This method will be try update user location by lastip";
}

QStringList CmdUpdateUserLocationHandler::errors(){
	QStringList	list;
	return list;
}

void CmdUpdateUserLocationHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

	// bool bConvert = false;

    int userid = jsonRequest["userid"].toInt();
	
	// TODO redesign
	if(userid == 0){
        pRequest->sendMessageError(cmd(), Errors::QuestIDMustBeNotZero());
		return;
	}

    QSqlDatabase db = *(pRequest->server()->database());
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
