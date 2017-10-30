#include <cmd_classbook_get_list_handler.h>
#include <QJsonArray>
#include <QSqlError>

CmdClassbookGetListHandler::CmdClassbookGetListHandler(){
    m_vInputs.push_back(CmdInputDef("parentid").required().integer_().description("parentid for classbook article"));
    m_vInputs.push_back(CmdInputDef("order").optional().integer_().description("order for classbook article"));
}

QString CmdClassbookGetListHandler::cmd(){
    return "classbook_get_list";
}

bool CmdClassbookGetListHandler::accessUnauthorized(){
    return true;
}

bool CmdClassbookGetListHandler::accessUser(){
    return true;
}

bool CmdClassbookGetListHandler::accessTester(){
    return true;
}

bool CmdClassbookGetListHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookGetListHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookGetListHandler::description(){
    return "Return list of parentid, id, names for classbook article";
}

QStringList CmdClassbookGetListHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookGetListHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());

    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(cmd());

    int parentid = obj["parentid"].toInt();

    jsonResponse["parentid"] = parentid;

    QJsonArray data;
    {
        QSqlQuery query(db);
        query.prepare("SELECT id, name FROM classbook WHERE parentid =:parentid ORDER BY ordered");
        query.bindValue(":parentid", parentid);
        query.exec();
        while (query.next()) {
            QSqlRecord record = query.record();
            QJsonObject item;
            item["parentid"] = parentid;
            item["classbookid"] = record.value("id").toInt();
            item["name"] = record.value("name").toString();
            data.push_back(item);
        }
    }

    jsonResponse["m"] = QJsonValue(m);
    jsonResponse["result"] = QJsonValue("DONE");
    jsonResponse["data"] = data;
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}

