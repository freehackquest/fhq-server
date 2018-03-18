#include <cmd_lxd_info.h>

CmdLXDInfoHandler::CmdLXDInfoHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("name").string_().required().description("Container name"));
    m_vInputs.push_back(CmdInputDef("get").string_().required().description("Requested information"));
}

// ---------------------------------------------------------------------

std::string CmdLXDInfoHandler::cmd(){
    return "lxd_info";
}
// ---------------------------------------------------------------------

const ModelCommandAccess & CmdLXDInfoHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdLXDInfoHandler::inputs(){
    return m_vInputs;
}

std::string CmdLXDInfoHandler::description(){
    return "Get information about the orhestra, containers.";
}

// ---------------------------------------------------------------------

void CmdLXDInfoHandler::handle(ModelRequest *pRequest){
    using namespace std;
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());

    string name = jsonRequest["name"].toString().trimmed().toStdString();
    string error, response;

    //TO DO
    //Action switch
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


