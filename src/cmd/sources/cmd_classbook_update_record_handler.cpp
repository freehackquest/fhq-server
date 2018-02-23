#include <cmd_classbook_update_record_handler.h>
#include <QJsonArray>
#include <QSqlError>

CmdClassbookUpdateRecordHandler::CmdClassbookUpdateRecordHandler(){
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("id for classbook article"));
    m_vInputs.push_back(CmdInputDef("name").optional().string_().description("name for classbook article"));
    m_vInputs.push_back(CmdInputDef("content").optional().string_().description("content for classbook article"));
    m_vInputs.push_back(CmdInputDef("ordered").optional().integer_().description("ordered for classbook article"));
    m_vInputs.push_back(CmdInputDef("parentid").optional().integer_().description("parentid for classbook article"));
}

std::string CmdClassbookUpdateRecordHandler::cmd(){
    return "classbook_update_record";
}

bool CmdClassbookUpdateRecordHandler::accessUnauthorized(){
    return false;
}

bool CmdClassbookUpdateRecordHandler::accessUser(){
    return false;
}

bool CmdClassbookUpdateRecordHandler::accessTester(){
    return false;
}

bool CmdClassbookUpdateRecordHandler::accessAdmin(){
    return true;
}

const QVector<CmdInputDef> &CmdClassbookUpdateRecordHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookUpdateRecordHandler::description(){
    return "Update a article with a given classbookid";
}

QStringList CmdClassbookUpdateRecordHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookUpdateRecordHandler::handle(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, QString m, QJsonObject obj){

    QSqlDatabase db = *(pWebSocketServer->database());

    int classbookid = obj["classbookid"].toInt();
    //IF classbookid = 0, THEN reject request
    if(classbookid == 0){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(403, "Not today. It's root article id"));
        return;
    }

    //Find article with a given classbookid
    QSqlQuery query(db);
    query.prepare("SELECT name FROM classbook WHERE id=:classbookid");
    query.bindValue(":classbookid", classbookid);
    query.exec();
    if (!query.next()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Not found a article with a given classbookid"));
        return;
    }

    //CHECK Do we have anything to change?
    if (!(obj.contains("name") || obj.contains("content") || obj.contains("ordered") || obj.contains("parentid"))){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(403, "Not found a charges. Not enough parameters"));
        return;
    }

    int parentid;
    //FIND article with id = parentid AND UPDATE parentid IF exist
    if(obj.contains("parentid")){
        parentid = obj.value("parentid").toInt();

        if (parentid != 0){
            //CHECK existence of the article
            query.prepare("SELECT name FROM classbook WHERE id=:parentid");
            query.bindValue(":parentid", parentid);
            query.exec();
            if(!query.next()){
                pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(404, "Not found a article with a given parentid"));
                return;
            }
        }

        query.prepare("UPDATE classbook SET parentid=:parentid WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":parentid", parentid);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
    }

    //UPDATE name for article
    QString name;
    if(obj.contains("name")){
        name = obj.value("name").toString().trimmed().toHtmlEscaped();
        query.prepare("UPDATE classbook SET name=:name WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":name", name);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
    }

    //UPDATE content for article
    QString content;
    if(obj.contains("content")){
        content = obj.value("content").toString().trimmed().toHtmlEscaped();
        QString md5_content = QString(QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5).toHex());
        query.prepare("UPDATE classbook SET content=:content, md5_content=:md5_content WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":content", content);
        query.bindValue(":md5_content", md5_content);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
    }

    //UPDATE ordered for article
    int ordered;
    if(obj.contains("ordered")){
        ordered = obj.value("ordered").toInt();
        query.prepare("UPDATE classbook SET ordered=:ordered WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":ordered", ordered);
        if (!query.exec()){
            pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
            return;
        }
    }

    //UPDATE updated column
    query.prepare("UPDATE classbook SET updated = NOW() WHERE id=:classbookid");
    query.bindValue(":classbookid", classbookid);
    if (!query.exec()){
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Error(500, query.lastError().text()));
        return;
    }

    //GET article info
    QJsonObject info;
    query.prepare("SELECT id, name, content, md5_content, ordered, parentid FROM classbook WHERE id=:classbookid");
    query.bindValue(":classbookid", classbookid);
    query.exec();
    if (query.next()) {
        QSqlRecord record = query.record();
        info["classbookid"] = classbookid;
        info["parentid"] = record.value("parentid").toInt();
        info["name"] = record.value("name").toString();
        info["content"] = record.value("content").toString();
        info["md5_content"] = record.value("md5_content").toString();
        info["ordered"] = record.value("ordered").toInt();
    } else {
        pWebSocketServer->sendMessageError(pClient, cmd(), m, Errors::NotFound("article"));
        return;
    }

    QJsonObject jsonResponse;
    jsonResponse["cmd"] = QJsonValue(QString(cmd().c_str()));
    jsonResponse["m"] = QJsonValue(m);
    jsonResponse["result"] = QJsonValue("DONE");
    jsonResponse["data"] = QJsonValue(info);
    pWebSocketServer->sendMessage(pClient, jsonResponse);
}

