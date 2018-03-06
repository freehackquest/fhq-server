#include <cmd_game_import_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameImportHandler::CmdGameImportHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
}

// ---------------------------------------------------------------------

std::string CmdGameImportHandler::cmd(){
    return "game_import";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdGameImportHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdGameImportHandler::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdGameImportHandler::description(){
    return "Import game";
}

// ---------------------------------------------------------------------

void CmdGameImportHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    pRequest->sendMessageError(cmd(), Errors::NotImplementedYet());
    return;

    // TODO
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
