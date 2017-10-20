#include <cmd_classbook_get_info_handler.h>
#include <QJsonArray>
#include <QSqlError>

CmdClassbookGetInfoHandler::CmdClassbookGetInfoHandler(){
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

    QJsonObject info;

    QSqlQuery query(db);
    query.prepare("SELECT name, content FROM classbook WHERE id=:classbookid");
    query.bindValue(":classbookid", classbookid);
    query.exec();
    while (query.next()) {
        QSqlRecord record = query.record();
        info["id"] = classbookid;
        info["name"] = record.value("name").toString();
        info["content"] = record.value("content").toString();
    }

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["result"] = QJsonValue("DONE");
    jsonData["m"] = QJsonValue(m);
    jsonData["info"] = info;
    pWebSocketServer->sendMessage(pClient, jsonData);
}
