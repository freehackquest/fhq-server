#include <cmd_classbook_info_handler.h>
#include <QJsonArray>
#include <QSqlError>

CmdClassbookInfoHandler::CmdClassbookInfoHandler(){
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("id for classbook article"));
    m_vInputs.push_back(CmdInputDef("lang").optional().string_().description("Language"));
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
    return "Return name and content classbook record for id";
}

QStringList CmdClassbookInfoHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookInfoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());

    int classbookid = obj["classbookid"].toInt();

    QJsonObject info;

    QSqlQuery query(db);
    query.prepare("SELECT name, parentid, content FROM classbook WHERE id=:classbookid");
    query.bindValue(":classbookid", classbookid);
    query.exec();
    if (query.next()) {
        QSqlRecord record = query.record();
        info["classbookid"] = classbookid;
        info["parentid"] = record.value("parentid").toString();
        info["name"] = record.value("name").toString();
        info["content"] = record.value("content").toString();
    }else{
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotFound("article"));
        return;
    }

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    jsonData["data"] = info;
    pWebSocketServer->sendMessage(pClient, jsonData);
}
