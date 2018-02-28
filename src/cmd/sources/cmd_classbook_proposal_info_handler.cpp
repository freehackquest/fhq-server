#include <cmd_classbook_proposal_info_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>

CmdClassbookProposalInfoHandler::CmdClassbookProposalInfoHandler(){
    m_vInputs.push_back(CmdInputDef("classbook_proposal_id").required().integer_().description("Proposal id"));
}

std::string CmdClassbookProposalInfoHandler::cmd(){
    return "classbook_proposal_info";
}

bool CmdClassbookProposalInfoHandler::accessUnauthorized(){
    return false;
}

bool CmdClassbookProposalInfoHandler::accessUser(){
    return true;
}

bool CmdClassbookProposalInfoHandler::accessTester(){
    return true;
}

bool CmdClassbookProposalInfoHandler::accessAdmin(){
    return true;
}

const std::vector<CmdInputDef> &CmdClassbookProposalInfoHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookProposalInfoHandler::description(){
    return "Find and display all proposal data by id";
}

QStringList CmdClassbookProposalInfoHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookProposalInfoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

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

    query.prepare("SELECT classbookid, lang, name, content FROM classbook_proposal WHERE id = :classbook_proposal_id");
    query.bindValue(":classbook_proposal_id", classbook_proposal_id);
    if (!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }
    query.next();
    QSqlRecord record = query.record();
    data["classbookid"] = record.value("classbookid").toInt();
    data["id"] = classbook_proposal_id;
    data["lang"] = record.value("lang").toString();
    data["name"] = record.value("name").toString();
    data["content"] = record.value("content").toString();

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    jsonData["data"] = data;
    pWebSocketServer->sendMessage(pClient, jsonData);
}

