#include <cmd_api_handler.h>
#include <QtCore>

CmdApiHandler::CmdApiHandler(){
    TAG = "CmdApiHandler";

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);
}

// ---------------------------------------------------------------------

std::string CmdApiHandler::cmd(){
	return "api";
}

// ---------------------------------------------------------------------

std::string CmdApiHandler::description(){
    return "This method Will be return list of all handlers";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdApiHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdApiHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

void CmdApiHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonResponse;

	QJsonObject data;
    pRequest->server()->exportApi(data);

    jsonResponse["data"] = data;
    jsonResponse["version"] = QCoreApplication::applicationVersion();
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
