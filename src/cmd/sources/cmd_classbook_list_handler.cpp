#include <cmd_classbook_list_handler.h>
#include <QJsonArray>
#include <QSqlError>

CmdClassbookListHandler::CmdClassbookListHandler(){
    m_vInputs.push_back(CmdInputDef("parentid").required().integer_().description("parentid for classbook articles"));
    m_vInputs.push_back(CmdInputDef("lang").optional().string_().description("lang for classbook articles"));
    m_vInputs.push_back(CmdInputDef("search").optional().string_().description("Search string for classbook articles"));
}

QString CmdClassbookListHandler::cmd(){
    return "classbook_list";
}

bool CmdClassbookListHandler::accessUnauthorized(){
    return true;
}

bool CmdClassbookListHandler::accessUser(){
    return true;
}

bool CmdClassbookListHandler::accessTester(){
    return true;
}

bool CmdClassbookListHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookListHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookListHandler::description(){
    return "Return list of classbook articles with parentid, id, names, childs, proposals for a given parentid";
}

QStringList CmdClassbookListHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookListHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());
    QSqlQuery query(db);

    int parentid = obj["parentid"].toInt();
    QJsonArray data;

    if (obj.contains("search") && !obj.value("search").toString().isEmpty()){
        QString search = obj.value("search").toString();

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

        QSqlQuery query1(db);
        if (lang == "en"){
            query1.prepare("SELECT id, name FROM classbook WHERE name LIKE :search OR content LIKE :search ORDER BY ordered;");
            query1.bindValue(":search", "%" + search + "%");
        } else {
            query1.prepare("SELECT id, name FROM classbook_localization WHERE name LIKE :search OR content LIKE :search");
            query1.bindValue(":search", "%" + search + "%");
        }

        if (!query1.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query1.lastError().text()));
            return;
        }
        while (query1.next()) {
            QSqlRecord record = query1.record();
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

    } else {
    //CHECK exist parentid in DB
    query.prepare("SELECT name FROM classbook WHERE id =:parentid");
    query.bindValue(":parentid", parentid);
    query.exec();
    if (!query.next() && parentid != 0){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Not found the article with a given parentid"));
        return;
    }


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

    query.prepare("SELECT id, name FROM classbook WHERE parentid =:parentid ORDER BY ordered");
    query.bindValue(":parentid", parentid);
    query.exec();
    while (query.next()) {
        QSqlRecord record = query.record();
        QJsonObject item;
        int classbookid;
        item["parentid"] = parentid;
        classbookid = record.value("id").toInt();
        item["classbookid"] = classbookid;

        //GET name with the lang
        if (lang == "en"){
            item["name"] = record.value("name").toString();
        } else {
            QSqlQuery query_lang(db);
            query_lang.prepare("SELECT name FROM classbook_localization WHERE classbookid=:classbookid AND lang=:lang");
            query_lang.bindValue(":classbookid", classbookid);
            query_lang.bindValue(":lang", lang);
            query_lang.exec();
            if (query_lang.next()){
                QSqlRecord record_lang = query_lang.record();
                item["name"] = record_lang.value("name").toString();
            } else {
                item["name"] = record.value("name").toString();
            }
        }

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
    }}

    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(cmd());
    jsonResponse["m"] = QJsonValue(m);
    jsonResponse["result"] = QJsonValue("DONE");
    jsonResponse["data"] = data;
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}

