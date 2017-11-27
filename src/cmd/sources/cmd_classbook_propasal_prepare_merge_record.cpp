#include <cmd_classbook_propasal_prepare_merge_record.h>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>

CmdClassbookProposalPrepareMergeRecordHandler::CmdClassbookProposalPrepareMergeRecordHandler(){
    m_vInputs.push_back(CmdInputDef("classbook_proposal_id").required().integer_().description("Proposal id"));
}

QString CmdClassbookProposalPrepareMergeRecordHandler::cmd(){
    return "classbook_propasal_prepare_merge_record";
}

bool CmdClassbookProposalPrepareMergeRecordHandler::accessUnauthorized(){
    return false;
}

bool CmdClassbookProposalPrepareMergeRecordHandler::accessUser(){
    return false;
}

bool CmdClassbookProposalPrepareMergeRecordHandler::accessTester(){
    return false;
}

bool CmdClassbookProposalPrepareMergeRecordHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookProposalPrepareMergeRecordHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookProposalPrepareMergeRecordHandler::description(){
    return "Prepare to merge updating requests";
}

QStringList CmdClassbookProposalPrepareMergeRecordHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookProposalPrepareMergeRecordHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());

    QJsonObject data;

    QSqlQuery query(db);
    int classbook_proposal_id = obj["classbook_proposal_id"].toInt();
    query.prepare("SELECT id FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", obj["classbook_proposal_id"].toInt());
    if(!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "This proposal doesn't exist"));
        return;
    }

    query.prepare("SELECT content FROM classbook WHERE id IN (SELECT classbookid FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", classbook_proposal_id);
    if (!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }
    QSqlRecord record = query.record();
    data["content"] = record.value("content").toString();

    //add merge and update output

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    jsonData["data"] = data;
    pWebSocketServer->sendMessage(pClient, jsonData);
}


