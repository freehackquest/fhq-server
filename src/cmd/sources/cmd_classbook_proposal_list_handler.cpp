#include <cmd_classbook_proposal_list_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <QMap>
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

    QMap<QString, QJsonValue> mapFilter;
    if(obj.contains("classbookid")){
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
        mapFilter.insert("classbookid", obj["classbookid"].toInt());
    }
    if(obj.contains("lang")){
        mapFilter.insert("lang", obj["lang"].toString().trimmed());
    }

    QString sQuery = "SELECT id, name FROM classbook_proposal";
    if(mapFilter.size() > 0) sQuery += " WHERE ";
    bool bFirst = true;
    foreach(QString key, mapFilter.keys()){
        if(!bFirst) sQuery += " AND ";
        bFirst = false;
        sQuery +=  key + " = :" + key;
    }
    query.prepare(sQuery);
    foreach(QString key, mapFilter.keys()){
        QMap<QString, QJsonValue>::const_iterator v = mapFilter.lowerBound(key);
        query.bindValue(":" + key, v.value());
    }
    if (!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        item["id"] = record.value("id").toInt();
        item["classbookid"] = obj["classbookid"].toInt();
        item["lang"] = obj["lang"].toString().trimmed();
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
