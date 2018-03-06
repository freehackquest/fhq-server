#include <cmd_classbook_proposal_delete_record_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>

CmdClassbookProposalDeleteRecordHandler::CmdClassbookProposalDeleteRecordHandler(){
    m_vInputs.push_back(CmdInputDef("classbook_proposal_id").required().integer_().description("Proposal id"));
}

std::string CmdClassbookProposalDeleteRecordHandler::cmd(){
    return "classbook_proposal_delete_record";
}

bool CmdClassbookProposalDeleteRecordHandler::accessUnauthorized(){
    return false;
}

bool CmdClassbookProposalDeleteRecordHandler::accessUser(){
    return true;
}

bool CmdClassbookProposalDeleteRecordHandler::accessTester(){
    return true;
}

bool CmdClassbookProposalDeleteRecordHandler::accessAdmin(){
    return true;
}

const std::vector<CmdInputDef> &CmdClassbookProposalDeleteRecordHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookProposalDeleteRecordHandler::description(){
    return "Delete a proposal of updating an article";
}

QStringList CmdClassbookProposalDeleteRecordHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookProposalDeleteRecordHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());

    QSqlQuery query(db);
    int classbook_proposal_id = jsonRequest["classbook_proposal_id"].toInt();
    query.prepare("SELECT id FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", jsonRequest["classbook_proposal_id"].toInt());
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pRequest->sendMessageError(cmd(), Error(404, "This proposal doesn't exist"));
        return;
    }
    query.prepare("DELETE FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", classbook_proposal_id);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


