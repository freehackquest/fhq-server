#include <cmd_classbook_proposal_add_record_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>

CmdClassbookProposalAddRecordHandler::CmdClassbookProposalAddRecordHandler(){
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("Classbookid for an article"));
    m_vInputs.push_back(CmdInputDef("lang").required().string_().description("Language"));
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("Article name"));
    m_vInputs.push_back(CmdInputDef("content").required().string_().description("The content of the article"));
}

QString CmdClassbookProposalAddRecordHandler::cmd(){
    return "classbook_proposal_add_record";
}

bool CmdClassbookProposalAddRecordHandler::accessUnauthorized(){
    return false;
}

bool CmdClassbookProposalAddRecordHandler::accessUser(){
    return true;
}

bool CmdClassbookProposalAddRecordHandler::accessTester(){
    return true;
}

bool CmdClassbookProposalAddRecordHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookProposalAddRecordHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookProposalAddRecordHandler::description(){
    return "Propose an update of article";
}

QStringList CmdClassbookProposalAddRecordHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookProposalAddRecordHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());

    QJsonObject data;

    QSqlQuery query(db);
    int classbookid = obj["classbookid"].toInt();
    query.prepare("SELECT id FROM classbook WHERE id = :classbookid");
    query.bindValue(":classbookid", obj["classbookid"].toString().trimmed());
    if(!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "This article doesn't exist"));
        return;
    }

    QString lang = obj["lang"].toString().trimmed();
    QString name = obj["name"].toString().trimmed();
    QString content = obj["content"].toString().trimmed();

    query.prepare("INSERT INTO classbook_proposal("
                  "classbookid,"
                  "lang,"
                  "name,"
                  "content,"
                  "created"
                  ") "
                  "VALUES("
                  ":classbookid,"
                  ":lang,"
                  ":name,"
                  ":content,"
                  "NOW()"
                  ")");
    query.bindValue(":classbookid", classbookid);
    query.bindValue(":lang", lang);
    query.bindValue(":name", name);
    query.bindValue(":content", content);
    if(!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }
    data["classbookid"] = classbookid;
    data["classbook_proposal_id"] = QJsonValue(query.lastInsertId().toInt());
    data["lang"] = lang;
    data["name"] = name;
    data["content"] = content;

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    jsonData["data"] = data;
    pWebSocketServer->sendMessage(pClient, jsonData);
}

