#include <cmd_classbook_localization_update_record_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>
#include <QCryptographicHash>

CmdClassbookLocalizationUpdateRecordHandler::CmdClassbookLocalizationUpdateRecordHandler(){
    m_vInputs.push_back(CmdInputDef("classbook_localizationid").required().integer_().description("Localization id"));
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("Article name"));
    m_vInputs.push_back(CmdInputDef("content").required().string_().description("The content of the article"));
}

std::string CmdClassbookLocalizationUpdateRecordHandler::cmd(){
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
    int classbook_localizationid = obj["classbook_localizationid"].toInt();
    query.prepare("SELECT id FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    if(!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "This localization doesn't exist"));
        return;
    }

    QString name = obj["name"].toString().trimmed();
    QString content = obj["content"].toString().trimmed();

    //Set md5_content hash
    QString md5_content = QString(QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5).toHex());

    query.prepare("UPDATE classbook_localization SET name = :name, content = :content, md5_content = :md5_content, updated = NOW() "
                  "WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    query.bindValue(":name", name);
    query.bindValue(":content", content);
    query.bindValue(":md5_content", md5_content);
    if(!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }
    query.prepare("SELECT classbookid, lang FROM classbook_localization WHERE id=:id");
    query.bindValue(":id", classbook_localizationid);
    query.exec();
    query.next();
    QSqlRecord record = query.record();
    data["classbookid"] = record.value("classbookid").toInt();
    data["classbook_localizationid"] = classbook_localizationid;
    data["lang"] = record.value("lang").toString();
    data["name"] = name;
    data["content"] = content;
    data["md5_content"] = md5_content;

    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));
    jsonData["m"] = QJsonValue(m);
    jsonData["result"] = QJsonValue("DONE");
    jsonData["data"] = data;
    pWebSocketServer->sendMessage(pClient, jsonData);
}
