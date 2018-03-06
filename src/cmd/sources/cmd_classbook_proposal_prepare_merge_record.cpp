#include <cmd_classbook_proposal_prepare_merge_record.h>
#include <utils_merge_text.h>
#include <vector>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>

CmdClassbookProposalPrepareMergeRecordHandler::CmdClassbookProposalPrepareMergeRecordHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbook_proposal_id").required().integer_().description("Proposal id"));
}

// ---------------------------------------------------------------------

std::string CmdClassbookProposalPrepareMergeRecordHandler::cmd(){
    return "classbook_propasal_prepare_merge_record";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdClassbookProposalPrepareMergeRecordHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdClassbookProposalPrepareMergeRecordHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdClassbookProposalPrepareMergeRecordHandler::description(){
    return "Prepare to merge updating requests";
}

// ---------------------------------------------------------------------

void CmdClassbookProposalPrepareMergeRecordHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());

    QJsonObject data;

    QSqlQuery query(db);
    QSqlRecord record = query.record();
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

    query.prepare("SELECT content FROM classbook WHERE id IN (SELECT classbookid FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", classbook_proposal_id);
    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    QString curtxt = record.value("content").toString();

    query.prepare("SELECT content, content_before FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", classbook_proposal_id);
    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    QString txt1 = record.value("content").toString();
    QString txt2 = record.value("content_before").toString();
    std::vector<row *> arr1, arr2;
    UtilsMergeText::merge(curtxt, txt1, txt2, arr1, arr2);

    //TO DO final merge, lang checkout, update output (with data)

    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


