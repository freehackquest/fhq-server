#include <cmd_classbook_list_handler.h>
#include <QJsonArray>
#include <QSqlError>

CmdClassbookListHandler::CmdClassbookListHandler(){
    m_vInputs.push_back(CmdInputDef("parentid").required().integer_().description("parentid for classbook article"));
    m_vInputs.push_back(CmdInputDef("ordered").optional().integer_().description("order for classbook article"));
}

QString CmdClassbookListHandler::cmd(){
    return "classbook_get_list";
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
    return "Return list of parentid, id, names for classbook article";
}

QStringList CmdClassbookListHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookListHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

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

