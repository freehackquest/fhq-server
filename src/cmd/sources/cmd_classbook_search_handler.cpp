#include <cmd_classbook_search_handler.h>
#include <QJsonArray>
#include <QSqlError>

CmdClassbookSearchHandler::CmdClassbookSearchHandler(){
    m_vInputs.push_back(CmdInputDef("parentid").required().integer_().description("parentid for classbook articles"));
    m_vInputs.push_back(CmdInputDef("lang").optional().string_().description("lang for classbook articles"));
    m_vInputs.push_back(CmdInputDef("search").required().string_().description("Search string by LIKE"));
}

QString CmdClassbookSearchHandler::cmd(){
    return "classbook_search";
}

bool CmdClassbookSearchHandler::accessUnauthorized(){
    return true;
}

bool CmdClassbookSearchHandler::accessUser(){
    return true;
}

bool CmdClassbookSearchHandler::accessTester(){
    return true;
}

bool CmdClassbookSearchHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookSearchHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookSearchHandler::description(){
    return "Return list of classbook articles with parentid, id, names, childs, proposals for search string";
}

QStringList CmdClassbookSearchHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookSearchHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());
    QSqlQuery query(db);

    int parentid = obj["parentid"].toInt();
    //CHECK exist parentid in DB
    query.prepare("SELECT name FROM classbook WHERE parentid =:parentid");
    query.bindValue(":parentid", parentid);
    query.exec();
    if (!query.next() && parentid != 0){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Not found the article with a given parentid"));
        return;
    }

    QString search = obj["search"].toString();

    //SET lang
    QString lang;
    if (obj.contains("lang")){
        lang = obj.value("lang").toString().trimmed();
        QList<QString> allow_lang = {"en", "ru","de"};
        if(!allow_lang.contains(lang)){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Language is'not support"));
            return;
        }
    } else {
        lang = "en";
    }

    QJsonArray data;
    if (lang != "en"){
        query.prepare("SELECT id, name FROM classbook WHERE name LIKE :search OR content LIKE :search ORDER BY ordered;");
        query.bindValue(":search", "'%" + search + "%'");
    } else {
        query.prepare("SELECT classbookid, name FROM classbook_localization "
                          "WHERE lang=:lang AND (name LIKE '%:search%' OR content LIKE '%:search%')");
        query.bindValue(":search", search);
        query.bindValue(":lang", lang);
    }
    //DOESN'T WORK
    query.exec();
    while (query.next()) {
        QSqlRecord record = query.record();
        QJsonObject item;
        int classbookid;
        item["parentid"] = parentid;
        classbookid = record.value("id").toInt();
        item["classbookid"] = classbookid;
        if (lang != "en"){
            query.prepare("SELECT classbookid, name FROM classbook_localization "
                              "WHERE lang=:lang AND (name LIKE '%:search%' OR content LIKE '%:search%')");
            query.bindValue(":search", search);
            query.bindValue(":lang", lang);
        }
        item["name"] = record.value("name").toString();

        //COUNT childs for an article
        QSqlQuery query_childs(db);
        query_childs.prepare("SELECT COUNT(id) AS childs FROM classbook WHERE parentid =:classbookid");
        query_childs.bindValue(":classbookid", classbookid);
        query_childs.exec();
        int childs = 0;
        if (query_childs.next()){
            QSqlRecord record_childs = query_childs.record();
            childs = record_childs.value("childs").toInt();
        }
        item["childs"] = QJsonValue(childs);

        //COUNT proposals for an article
        QSqlQuery query_proposals(db);
        query_proposals.prepare("SELECT COUNT(id) AS proposals FROM classbook_proposal "
                                "WHERE classbookid =:classbookid AND lang=:lang");
        query_proposals.bindValue(":classbookid", classbookid);
        query_proposals.bindValue(":lang", lang);
        query_proposals.exec();
        int proposals = 0;
        if (query_proposals.next()){
            QSqlRecord record_proposals = query_proposals.record();
            proposals = record_proposals.value("proposals").toInt();
        }
        item["proposals"] = QJsonValue(proposals);

        data.push_back(item);
    }

    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(cmd());
    jsonResponse["m"] = QJsonValue(m);
    jsonResponse["result"] = QJsonValue("DONE");
    jsonResponse["data"] = data;
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}


