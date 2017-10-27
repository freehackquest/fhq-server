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
    return true;
}

bool CmdClassbookLocalizationDeleteRecordHandler::accessUser(){
    return true;
}

bool CmdClassbookLocalizationDeleteRecordHandler::accessTester(){
    return true;
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

    int classbook_localizationid = obj["classbook_localizationid"].toInt();

    QSqlDatabase db = *(pWebSocketServer->database());

    QSqlQuery query(db);
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

