#include <cmd_classbook_get_info_handler.h>
#include <QJsonArray>
#include <QSqlError>

CmdClassbookGetInfoHandler::CmdClassbookGetInfoHandler(){
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("id for classbook article"));
    m_vInputs.push_back(CmdInputDef("lang").optional().string_().description("Language"));
}

QString CmdClassbookGetInfoHandler::cmd(){
    return "classbook_get_info";
}

bool CmdClassbookGetInfoHandler::accessUnauthorized(){
    return true;
}

bool CmdClassbookGetInfoHandler::accessUser(){
    return true;
}

bool CmdClassbookGetInfoHandler::accessTester(){
    return true;
}

bool CmdClassbookGetInfoHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookGetInfoHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookGetInfoHandler::description(){
    return "Return name and content classbook record for id";
}

QStringList CmdClassbookGetInfoHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookGetInfoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());

    int classbookid = obj["classbookid"].toInt();

    QJsonObject jsonData;

    {
        QSqlQuery query(db);
        query.prepare("SELECT name, parentid, content FROM classbook WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            jsonData["classbookid"] = classbookid;
            jsonData["parentid"] = record.value("parentid").toString();
            jsonData["name"] = record.value("name").toString();
            jsonData["content"] = record.value("content").toString();
        }else{
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "not found classbook article with this id"));
            return;
        }
    }

    // load langs
    // TODO load to cache
    {
        QJsonObject jsonLangs;
        QSqlQuery query(db);
        query.prepare("SELECT id, lang FROM classbook_localization WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
        while(query.next()) {
            QSqlRecord record = query.record();
            QString lang = record.value("lang").toString();
            jsonLangs[lang] = record.value("id").toInt();
        }
        jsonData["langs"] = jsonLangs;
    }

    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(cmd());
    jsonResponse["m"] = QJsonValue(m);
    jsonResponse["result"] = QJsonValue("DONE");
    jsonResponse["data"] = jsonData;
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}
