#include <cmd_classbook_update_record_handler.h>
#include <QJsonArray>
#include <QSqlError>

CmdClassbookUpdateRecordHandler::CmdClassbookUpdateRecordHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("id for classbook article"));
    m_vInputs.push_back(CmdInputDef("name").optional().string_().description("name for classbook article"));
    m_vInputs.push_back(CmdInputDef("content").optional().string_().description("content for classbook article"));
    m_vInputs.push_back(CmdInputDef("ordered").optional().integer_().description("ordered for classbook article"));
    m_vInputs.push_back(CmdInputDef("parentid").optional().integer_().description("parentid for classbook article"));
}

// ---------------------------------------------------------------------

std::string CmdClassbookUpdateRecordHandler::cmd(){
    return "classbook_update_record";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdClassbookUpdateRecordHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdClassbookUpdateRecordHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdClassbookUpdateRecordHandler::description(){
    return "Update a article with a given classbookid";
}

// ---------------------------------------------------------------------

void CmdClassbookUpdateRecordHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());

    int classbookid = jsonRequest["classbookid"].toInt();
    //IF classbookid = 0, THEN reject request
    if(classbookid == 0){
        pRequest->sendMessageError(cmd(), Error(403, "Not today. It's root article id"));
        return;
    }

    //Find article with a given classbookid
    QSqlQuery query(db);
    query.prepare("SELECT name FROM classbook WHERE id=:classbookid");
    query.bindValue(":classbookid", classbookid);
    query.exec();
    if (!query.next()){
        pRequest->sendMessageError(cmd(), Error(404, "Not found a article with a given classbookid"));
        return;
    }

    //CHECK Do we have anything to change?
    if (!(jsonRequest.contains("name") || jsonRequest.contains("content") || jsonRequest.contains("ordered") || jsonRequest.contains("parentid"))){
        pRequest->sendMessageError(cmd(), Error(403, "Not found a charges. Not enough parameters"));
        return;
    }

    int parentid;
    //FIND article with id = parentid AND UPDATE parentid IF exist
    if(jsonRequest.contains("parentid")){
        parentid = jsonRequest.value("parentid").toInt();

        if (parentid != 0){
            //CHECK existence of the article
            query.prepare("SELECT name FROM classbook WHERE id=:parentid");
            query.bindValue(":parentid", parentid);
            query.exec();
            if(!query.next()){
                pRequest->sendMessageError(cmd(), Error(404, "Not found a article with a given parentid"));
                return;
            }
        }

        query.prepare("UPDATE classbook SET parentid=:parentid WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":parentid", parentid);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    //UPDATE name for article
    QString name;
    if(jsonRequest.contains("name")){
        name = jsonRequest.value("name").toString().trimmed().toHtmlEscaped();
        query.prepare("UPDATE classbook SET name=:name WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":name", name);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    //UPDATE content for article
    QString content;
    if(jsonRequest.contains("content")){
        content = jsonRequest.value("content").toString().trimmed().toHtmlEscaped();
        QString md5_content = QString(QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5).toHex());
        query.prepare("UPDATE classbook SET content=:content, md5_content=:md5_content WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":content", content);
        query.bindValue(":md5_content", md5_content);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    //UPDATE ordered for article
    int ordered;
    if(jsonRequest.contains("ordered")){
        ordered = jsonRequest.value("ordered").toInt();
        query.prepare("UPDATE classbook SET ordered=:ordered WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.bindValue(":ordered", ordered);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    //UPDATE updated column
    query.prepare("UPDATE classbook SET updated = NOW() WHERE id=:classbookid");
    query.bindValue(":classbookid", classbookid);
    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
        pRequest->sendMessageError(cmd(), Errors::NotFound("article"));
        return;
    }

    jsonResponse["data"] = QJsonValue(info);
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

