#include <cmd_createpublicevent_handler.h>
#include <QJsonArray>

CmdCreatePublicEventHandler::CmdCreatePublicEventHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

	QStringList eventTypes;
	// TODO load from database
	eventTypes << "info";
	eventTypes << "users";
	eventTypes << "games";
	eventTypes << "quests";
	eventTypes << "warning";
	
    // validation and description input fields
	m_vInputs.push_back(CmdInputDef("type").enum_(eventTypes).required());
	m_vInputs.push_back(CmdInputDef("message").string_().required());
}

// ---------------------------------------------------------------------

std::string CmdCreatePublicEventHandler::cmd(){
	return "createpublicevent";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdCreatePublicEventHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdCreatePublicEventHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdCreatePublicEventHandler::description(){
	return "Create the public event";
}

// ---------------------------------------------------------------------

void CmdCreatePublicEventHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QString type = jsonRequest["type"].toString().trimmed();
    QString message = jsonRequest["message"].toString().trimmed();
	

    QSqlDatabase db = *(pRequest->server()->database());
	QSqlQuery query(db);
	query.prepare("INSERT INTO public_events(type,message,dt) VALUES(:type,:message,NOW())");
	query.bindValue(":type", type);
	query.bindValue(":message", message);
    if(!query.exec()){
        // TODO database error
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
