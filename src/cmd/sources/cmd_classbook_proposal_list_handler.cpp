#include <cmd_classbook_proposal_list_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>

CmdClassbookProposalListHandler::CmdClassbookProposalListHandler(){
    m_vInputs.push_back(CmdInputDef("classbookid").optional().integer_().description("Classbookid for an article"));
    m_vInputs.push_back(CmdInputDef("lang").optional().string_().description("Language"));
}

QString CmdClassbookProposalListHandler::cmd(){
    return "classbook_proposal_list";
}

bool CmdClassbookProposalListHandler::accessUnauthorized(){
    return false;
}

bool CmdClassbookProposalListHandler::accessUser(){
    return true;
}

bool CmdClassbookProposalListHandler::accessTester(){
    return true;
}

bool CmdClassbookProposalListHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookProposalListHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookProposalListHandler::description(){
    return "Display list of proposals by classbookid";
}

QStringList CmdClassbookProposalListHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookProposalListHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());
    QSqlQuery query(db);

    QJsonObject item;
    QJsonArray data;

    int n;
    if(obj.contains("classbookid")){
        if(obj.contains("lang")) n = 4;
        else n = 3;
    } else {
        if(obj.contains("lang")) n = 2;
        else n = 1;
    }

    int classbookid = obj["classbookid"].toInt();
    QString lang = obj["lang"].toString().trimmed();

    switch(n){
            case 4:
                query.prepare("SELECT id FROM classbook WHERE id = :classbookid");
                query.bindValue(":classbookid", obj["classbookid"].toInt());
                if(!query.exec()){
                    pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
                    return;
                }
                if(!query.next()){
                    pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "This article doesn't exist"));
                    return;
                }
                query.prepare("SELECT lang FROM classbook_proposal WHERE lang = :lang");
                query.bindValue(":lang", obj["lang"].toInt());
                if(!query.exec()){
                    pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
                    return;
                }
                if(!query.next()){
                    pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "This language doesn't exist"));
                    return;
                }
                query.prepare("SELECT id, name FROM classbook_proposal WHERE classbookid = :classbookid AND lang = :lang");
                query.bindValue(":classbookid", classbookid);
                query.bindValue(":lang", lang);
                if (!query.exec()){
                    pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
                    return;
                }
                break;
            case 3:
                query.prepare("SELECT id FROM classbook WHERE id = :classbookid");
                query.bindValue(":classbookid", obj["classbookid"].toInt());
                if(!query.exec()){
                    pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
                    return;
                }
                if(!query.next()){
                    pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "This article doesn't exist"));
                    return;
                }
                query.prepare("SELECT id, name FROM classbook_proposal WHERE classbookid = :classbookid");
                query.bindValue(":classbookid", classbookid);
                if (!query.exec()){
                    pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
                    return;
                }
                break;
            case 2:
                query.prepare("SELECT lang FROM classbook_proposal WHERE lang = :lang");
                query.bindValue(":lang", obj["lang"].toInt());
                if(!query.exec()){
                    pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
                    return;
                }
                if(!query.next()){
                    pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "This language doesn't exist"));
                    return;
                }
                query.prepare("SELECT id, name FROM classbook_proposal WHERE lang = :lang");
                query.bindValue(":lang", lang);
                if (!query.exec()){
                    pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
                    return;
                }
                break;
            case 1:
                query.prepare("SELECT id, name FROM classbook_proposal");
                if (!query.exec()){
                    pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
                    return;
                }
                break;
        }

    while (query.next()) {
        QSqlRecord record = query.record();
        item["id"] = record.value("id").toInt();
        item["classbookid"] = classbookid;
        item["lang"] = lang;
        item["name"] = record.value("name").toString();
        data.push_back(item);
    }

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    jsonData["data"] = data;
    pWebSocketServer->sendMessage(pClient, jsonData);
}
