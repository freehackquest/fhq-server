#include <cmd_classbook_get_list_handler.h>
#include <QJsonArray>
#include <QSqlError>

CmdClassbookGetListHandler::CmdClassbookGetListHandler(){
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
    return "Return list of names from classbook";
}

QStringList CmdClassbookGetListHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookGetListHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());

    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(cmd());

    int parentid = 0;
    if(obj.contains("parentid")){
        parentid = obj["parentid"].toInt();
    }
    jsonResponse["parentid"] = parentid;

    QString contain;
    if(obj.contains("contain")){
        contain = obj["contain"].toString().trimmed();
    }

    QString where = "WHERE parentid =:parentid";
    if(contain.length() != 0){
        where = "WHERE content LIKE '%:contain%' OR name LIKE '%:contain%'";
    }

    // data
    QJsonArray names;
    {
        QSqlQuery query(db);
        query.prepare("SELECT name FROM classbook "+where);
        query.bindValue(":parentid", parentid);
        query.bindValue(":contain", contain);
        query.exec();
        while (query.next()) {
            QSqlRecord record = query.record();
            QString name;
            name = record.value("name").toString();
            names.push_back(name);
        }
    }

    jsonResponse["result"] = QJsonValue("DONE");
    jsonResponse["m"] = QJsonValue(m);
    jsonResponse["names"] = names;
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}

