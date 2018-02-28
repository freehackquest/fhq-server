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

void CmdClassbookProposalDeleteRecordHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());

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
    query.prepare("DELETE FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", classbook_proposal_id);
    if(!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    pWebSocketServer->sendMessage(pClient, jsonData);
}


