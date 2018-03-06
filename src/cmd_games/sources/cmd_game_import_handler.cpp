#include <cmd_game_import_handler.h>
#include <runtasks.h>

#include <QJsonArray>
#include <QCryptographicHash>

CmdGameImportHandler::CmdGameImportHandler(){
    m_vInputs.push_back(CmdInputDef("uuid").uuid_().required().description("Global Identificator of the Game"));
}

std::string CmdGameImportHandler::cmd(){
    return "game_import";
}

bool CmdGameImportHandler::accessUnauthorized(){
	return false;
}

bool CmdGameImportHandler::accessUser(){
	return false;
}

bool CmdGameImportHandler::accessTester(){
	return false;
}

bool CmdGameImportHandler::accessAdmin(){
	return true;
}

const std::vector<CmdInputDef> &CmdGameImportHandler::inputs(){
	return m_vInputs;
};

QString CmdGameImportHandler::description(){
    return "Import game";
}

QStringList CmdGameImportHandler::errors(){
	QStringList	list;
	return list;
}

void CmdGameImportHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    pRequest->sendMessageError(cmd(), Errors::NotImplementedYet());
    return;

    // TODO
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
