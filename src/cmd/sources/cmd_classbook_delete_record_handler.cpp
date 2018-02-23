#include <cmd_classbook_delete_record_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <QUuid>

CmdClassbookDeleteRecordHandler::CmdClassbookDeleteRecordHandler(){
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("id for classbook article"));
}

std::string CmdClassbookDeleteRecordHandler::cmd(){
    return "classbook_delete_record";
}

bool CmdClassbookDeleteRecordHandler::accessUnauthorized(){
    return false;
}

bool CmdClassbookDeleteRecordHandler::accessUser(){
    return false;
}

bool CmdClassbookDeleteRecordHandler::accessTester(){
    return false;
}

bool CmdClassbookDeleteRecordHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookDeleteRecordHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookDeleteRecordHandler::description(){
    return "Delete a article with a given classbookid";
}

QStringList CmdClassbookDeleteRecordHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookDeleteRecordHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());

    int classbookid = obj["classbookid"].toInt();

    //DELETE Record IF haven't childs
    QSqlQuery query(db);
    if(classbookid !=0){
        query.prepare("SELECT id FROM classbook WHERE parentid=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.exec();
        if (query.next()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(403, "Could not delete, because childs exists. Please remove childs first."));
            return;
        }
        //Delete record in classbook
        query.prepare("DELETE FROM classbook WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.exec();
        if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::DatabaseError(query.lastError().text()));
            return;
        }

        //Delete record's localization
        query.prepare("DELETE FROM classbook_localization WHERE classbookid=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.exec();
        if(!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::DatabaseError(query.lastError().text()));
            return;
        }
    }

    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(QString(cmd().c_str()));
    jsonResponse["m"] = QJsonValue(m);
    jsonResponse["result"] = QJsonValue("DONE");
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}

