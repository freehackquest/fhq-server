#include <cmd_classbook_localization_update_record_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>

CmdClassbookLocalizationUpdateRecordHandler::CmdClassbookLocalizationUpdateRecordHandler(){
    m_vInputs.push_back(CmdInputDef("classbook_localizationid").required().integer_().description("Localization id"));
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("Article name"));
    m_vInputs.push_back(CmdInputDef("content").required().string_().description("The content of the article"));
}

QString CmdClassbookLocalizationUpdateRecordHandler::cmd(){
    return "classbook_localization_update_record";
}

bool CmdClassbookLocalizationUpdateRecordHandler::accessUnauthorized(){
    return true;
}

bool CmdClassbookLocalizationUpdateRecordHandler::accessUser(){
    return true;
}

bool CmdClassbookLocalizationUpdateRecordHandler::accessTester(){
    return true;
}

bool CmdClassbookLocalizationUpdateRecordHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookLocalizationUpdateRecordHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookLocalizationUpdateRecordHandler::description(){
    return "Update table with localization by classbookid";
}

QStringList CmdClassbookLocalizationUpdateRecordHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookLocalizationUpdateRecordHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    int classbook_localizationid = obj["classbook_localizationid"].toInt();
    QString name = obj["name"].toString().trimmed();
    QString content = obj["content"].toString().trimmed();

    QSqlDatabase db = *(pWebSocketServer->database());

    QJsonObject data;

    QSqlQuery query(db);
    query.prepare("UPDATE classbook_localization SET name = :name, content = :content, updated = NOW() "
                  "WHERE classbook_localizationid = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    query.bindValue(":name", name);
    query.bindValue(":content", content);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
    }
    while (query.next()) {
        QSqlRecord record = query.record();
        data["classbookid"] = record.value("classbookid").toInt();
        data["classbook_localizationid"] = classbook_localizationid;
        data["lang"] = record.value("lang").toString();
        data["name"] = name;
        data["content"] = content;
    }

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    jsonData["data"] = data;
    pWebSocketServer->sendMessage(pClient, jsonData);
}
