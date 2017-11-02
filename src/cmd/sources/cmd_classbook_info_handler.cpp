#include <cmd_classbook_info_handler.h>
#include <QJsonArray>
#include <QSqlError>

CmdClassbookInfoHandler::CmdClassbookInfoHandler(){
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("id for an classbook article"));
    m_vInputs.push_back(CmdInputDef("lang").optional().string_().description("Set lang for an article"));
    m_vInputs.push_back(CmdInputDef("proposal").optional().string_().description("Proposal"));
    m_vInputs.push_back(CmdInputDef("search").optional().string_().description("Search articles by LIKE"));
}

QString CmdClassbookInfoHandler::cmd(){
    return "classbook_get_info";
}

bool CmdClassbookInfoHandler::accessUnauthorized(){
    return true;
}

bool CmdClassbookInfoHandler::accessUser(){
    return true;
}

bool CmdClassbookInfoHandler::accessTester(){
    return true;
}

bool CmdClassbookInfoHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookInfoHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookInfoHandler::description(){
    return "Return name and content, langs, path classbook article with a given id";
}

QStringList CmdClassbookInfoHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookInfoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());

    int classbookid = obj["classbookid"].toInt();

    QSqlQuery query(db);
    QJsonObject info;
    //GET parentid and uuid for an article
    query.prepare("SELECT parentid, uuid FROM classbook WHERE id=:classbookid");
    query.bindValue(":classbookid", classbookid);
    query.exec();
    if (query.next()) {
        QSqlRecord record = query.record();
        info["classbookid"] = classbookid;
        info["parentid"] = record.value("parentid").toInt();
        info["uuid"] = record.value("uuid").toString();
    } else {
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotFound("an article"));
        return;
    }
    QString lang = "en";
    if(obj.contains("lang")){
        //GET localization for an article with a given lang
        lang = obj.value("lang").toString().trimmed();
        query.prepare("SELECT name, content FROM classbook_localization WHERE classbookid=:classbookid AND lang=:lang");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":lang", lang);
        query.exec();
        if (query.next()) {
            QSqlRecord record = query.record();
            info["lang"] = QJsonValue(lang);
            info["name"] = record.value("name").toString();
            info["content"] = record.value("content").toString();
        } else {
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotFound("Localization for an article"));
            return;
        }
    } else {
        //GET default localization for an article
        query.prepare("SELECT name, content FROM classbook WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.exec();
        if (query.next()) {
            QSqlRecord record = query.record();
            info["lang"] = QJsonValue(lang);
            info["name"] = record.value("name").toString();
            info["content"] = record.value("content").toString();
        } else {
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotFound("an article"));
            return;
        }
    }

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    jsonData["data"] = info;
    pWebSocketServer->sendMessage(pClient, jsonData);
}
