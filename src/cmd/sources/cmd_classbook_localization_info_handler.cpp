#include <cmd_classbook_localization_info_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>

CmdClassbookLocalizationInfoHandler::CmdClassbookLocalizationInfoHandler(){
    m_vInputs.push_back(CmdInputDef("classbook_localizationid").required().integer_().description("Localization id"));
}

QString CmdClassbookLocalizationInfoHandler::cmd(){
    return "classbook_localization_info";
}

bool CmdClassbookLocalizationInfoHandler::accessUnauthorized(){
    return false;
}

bool CmdClassbookLocalizationInfoHandler::accessUser(){
    return false;
}

bool CmdClassbookLocalizationInfoHandler::accessTester(){
    return false;
}

bool CmdClassbookLocalizationInfoHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookLocalizationInfoHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookLocalizationInfoHandler::description(){
    return "Find and display localization for an article by classbookid";
}

QStringList CmdClassbookLocalizationInfoHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookLocalizationInfoHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());

    QJsonObject data;

    QSqlQuery query(db);
    QString classbook_localizationid;
    query.prepare("SELECT classbook_localizationid FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", obj["classbook_localizationid"].toInt());
    if(!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }
    if(query.next()){
        classbook_localizationid = obj["classbook_localizationid"].toInt();
    } else {
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "This localization doesn't exist"));
        return;
    }

    query.prepare("SELECT id, lang FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    if (!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }
    QSqlRecord record = query.record();
    data["classbookid"] = record.value("classbookid").toInt();
    data["classbook_localizationid"] = classbook_localizationid;
    data["lang"] = record.value("lang").toString();
    data["name"] = record.value("name").toString();
    data["content"] = record.value("content").toString();

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    jsonData["data"] = data;
    pWebSocketServer->sendMessage(pClient, jsonData);
}
