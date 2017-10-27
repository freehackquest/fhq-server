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
    return false;
}

bool CmdClassbookLocalizationUpdateRecordHandler::accessUser(){
    return false;
}

bool CmdClassbookLocalizationUpdateRecordHandler::accessTester(){
    return false;
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

    QSqlDatabase db = *(pWebSocketServer->database());

    QJsonObject data;

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

    QString name = obj["name"].toString().trimmed();
    QString content = obj["content"].toString().trimmed();

    query.prepare("UPDATE classbook_localization SET name = :name, content = :content, updated = NOW() "
                  "WHERE classbook_localizationid = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    query.bindValue(":name", name);
    query.bindValue(":content", content);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
    }
    QSqlRecord record = query.record();
    data["classbookid"] = record.value("classbookid").toInt();
    data["classbook_localizationid"] = classbook_localizationid;
    data["lang"] = record.value("lang").toString();
    data["name"] = name;
    data["content"] = content;

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(cmd());
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    jsonData["data"] = data;
    pWebSocketServer->sendMessage(pClient, jsonData);
}
