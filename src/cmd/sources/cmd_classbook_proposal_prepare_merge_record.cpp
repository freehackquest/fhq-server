#include <cmd_classbook_proposal_prepare_merge_record.h>
#include <utils_merge_text.h>
#include <vector>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>

CmdClassbookProposalPrepareMergeRecordHandler::CmdClassbookProposalPrepareMergeRecordHandler(){
    m_vInputs.push_back(CmdInputDef("classbook_proposal_id").required().integer_().description("Proposal id"));
}

std::string CmdClassbookProposalPrepareMergeRecordHandler::cmd(){
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
    QSqlRecord record = query.record();
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
    QString curtxt = record.value("content").toString();

    query.prepare("SELECT content, content_before FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", classbook_proposal_id);
    if (!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }
    QString txt1 = record.value("content").toString();
    QString txt2 = record.value("content_before").toString();
    std::vector<row *> arr1, arr2;
    UtilsMergeText::merge(curtxt, txt1, txt2, arr1, arr2);

    //TO DO final merge, lang checkout, update output (with data)

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    jsonData["data"] = data;
    pWebSocketServer->sendMessage(pClient, jsonData);
}


