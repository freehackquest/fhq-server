#include <cmd_classbook_localization_delete_record_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>

CmdClassbookLocalizationDeleteRecordHandler::CmdClassbookLocalizationDeleteRecordHandler(){
    m_vInputs.push_back(CmdInputDef("classbook_localizationid").required().integer_().description("Localization id"));
}

QString CmdClassbookLocalizationDeleteRecordHandler::cmd(){
    return "classbook_localization_delete_record";
}

bool CmdClassbookLocalizationDeleteRecordHandler::accessUnauthorized(){
    return false;
}

bool CmdClassbookLocalizationDeleteRecordHandler::accessUser(){
    return false;
}

bool CmdClassbookLocalizationDeleteRecordHandler::accessTester(){
    return false;
}

bool CmdClassbookLocalizationDeleteRecordHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookLocalizationDeleteRecordHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookLocalizationDeleteRecordHandler::description(){
    return "Delete an article localization";
}

QStringList CmdClassbookLocalizationDeleteRecordHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookLocalizationDeleteRecordHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());

    QSqlQuery query(db);
    QString classbook_localizationid;
    if(obj.contains("classbook_localizationid")){
        query.prepare("SELECT classbook_localizationid FROM classbook_localization WHERE classbook_localizationid=:classbook_localizationid");
        query.bindValue(":classbook_localizationid", obj["classbook_localizationid"].toInt());
        query.exec();
        if(query.next()){
            classbook_localizationid = obj["classbook_localizationid"].toInt();
        } else {
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(403, "This localization doesn't exist"));
            return;
        }
    }
    query.prepare("DELETE FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
    }

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    pWebSocketServer->sendMessage(pClient, jsonData);
}

