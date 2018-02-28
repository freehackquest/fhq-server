#include <cmd_classbook_proposal_list_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <QMap>
#include <log.h>

CmdClassbookProposalListHandler::CmdClassbookProposalListHandler(){
    m_vInputs.push_back(CmdInputDef("classbookid").optional().integer_().description("Classbookid for an article"));
    m_vInputs.push_back(CmdInputDef("lang").optional().string_().description("Language"));
}

std::string CmdClassbookProposalListHandler::cmd(){
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

const std::vector<CmdInputDef> &CmdClassbookProposalListHandler::inputs(){
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

    QString sQuery;
    QMap<QString, QJsonValue> mapFilter;

    //checkout and validation of classbookid
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

    //checkout of lang and generation of query's bone
    if(obj.contains("lang")){
        mapFilter.insert("lang", obj["lang"].toString().trimmed());
        sQuery = "SELECT id, name FROM classbook_proposal";
    }
    else sQuery = "SELECT id, name, lang FROM classbook_proposal";

    //generation of the rest of the query
    if(mapFilter.size() > 0) sQuery += " WHERE ";
    bool bFirst = true;
    foreach(QString key, mapFilter.keys()){
        if(!bFirst) sQuery += " AND ";
        bFirst = false;
        sQuery +=  key + " = :" + key;
    }
    query.prepare(sQuery);

    //binding of values
    foreach(QString key, mapFilter.keys()){
        QMap<QString, QJsonValue>::const_iterator v = mapFilter.lowerBound(key);
        if(key=="classbookid")
            query.bindValue(":" + key, v.value().toInt());
        else
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
        if(obj.contains("lang")){
            item["lang"] = obj["lang"].toString().trimmed();
        }
        else item["lang"] = record.value("lang").toString().trimmed();
        item["name"] = record.value("name").toString();
        data.push_back(item);
    }

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    jsonData["data"] = data;
    pWebSocketServer->sendMessage(pClient, jsonData);
}
