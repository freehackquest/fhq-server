#include <cmd_classbook_add_record_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <QUuid>
#include <QCryptographicHash>

CmdClassbookAddRecordHandler::CmdClassbookAddRecordHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("parentid").required().integer_().description("pareintid for classbook article"));
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("name of article"));
    m_vInputs.push_back(CmdInputDef("content").required().string_().description("content of article"));
    m_vInputs.push_back(CmdInputDef("uuid").optional().uuid_().description("uuid of article"));
    m_vInputs.push_back(CmdInputDef("ordered").optional().integer_().description("order of article"));
}

// ---------------------------------------------------------------------

std::string CmdClassbookAddRecordHandler::cmd(){
    return "classbook_add_record";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdClassbookAddRecordHandler::access(){
    return m_modelCommandAccess;
}


// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdClassbookAddRecordHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdClassbookAddRecordHandler::description(){
    return "Adds a new article with the specified name, content, and id.";
}

// ---------------------------------------------------------------------

void CmdClassbookAddRecordHandler::handle(ModelRequest *pRequest){

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());

    int parentid = jsonRequest["parentid"].toInt();

    //Check parentid in database
    QSqlQuery query(db);
    if(parentid !=0){
        query.prepare("SELECT name FROM classbook WHERE id=:parentid");
        query.bindValue(":parentid", parentid);
        query.exec();
        if (!query.next()){
            pRequest->sendMessageError(cmd(), Errors::NotFound("article with this id"));
            return;
        }
    }

    QString name = jsonRequest["name"].toString().trimmed().toHtmlEscaped();
    QString content = jsonRequest["content"].toString().trimmed().toHtmlEscaped();

    //Set uuid from request if available, else generate uuid
    QString uuid;
    if(jsonRequest.contains("uuid")){
        query.prepare("SELECT uuid FROM classbook WHERE uuid=:uuid");
        query.bindValue(":uuid", jsonRequest["uuid"].toString());
        query.exec();
        if (!query.next()){
            uuid = jsonRequest["uuid"].toString().replace("{", "").replace("}", "");
        } else {
            pRequest->sendMessageError(cmd(), Error(403, "Uuid already exist"));
            return;
        }
    } else {
        uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");
    }

    //Set md5_content hash
    QString md5_content = QString(QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5).toHex());

    //Find parentuuid from database
    QString parentuuid = "00000000-0000-0000-0000-000000000000";
    if(parentid != 0){
        query.prepare("SELECT uuid FROM classbook WHERE id=:parentid");
        query.bindValue(":parentid", parentid);
        query.exec();
        query.next();
        QSqlRecord record = query.record();
        parentuuid = record.value("uuid").toString();
    }

    //Set ordered of article: increment max of child's ordered
    int ordered;
    if (jsonRequest.contains("ordered")){
        ordered = jsonRequest["ordered"].toInt();
    } else {
        query.prepare("SELECT MAX(ordered) AS max FROM classbook WHERE parentid=:parentid");
        query.bindValue(":parentid", parentid);
        query.exec();
        QSqlRecord record = query.record();
        if (!record.value("max").isNull())
            ordered = record.value("max").toInt() + 1;
        else {
            if (parentid != 0){
                query.prepare("SELECT ordered FROM classbook WHERE id=:parentid");
                query.bindValue(":parentid", parentid);
                query.exec();
                if (query.next()){
                    QSqlRecord record = query.record();
                    ordered = record.value("ordered").toInt() + 1;
                } else {
                    pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
                    return;
                }
            } else {
                ordered = 1;
            }

        }
    }

    //Insert article into classbook
    query.prepare("INSERT INTO classbook("
                  "parentid,"
                  "ordered,"
                  "uuid,"
                  "parentuuid,"
                  "name,"
                  "content,"
                  "md5_content,"
                  "created,"
                  "updated"
                  ")"
                  "VALUES("
                  ":parentid,"
                  ":ordered,"
                  ":uuid,"
                  ":parentuuid,"
                  ":name,"
                  ":content,"
                  ":md5_content,"
                  "NOW(),"
                  "NOW()"
                  ")");
    query.bindValue(":parentid", parentid);
    query.bindValue(":ordered", ordered);
    query.bindValue(":uuid", uuid);
    query.bindValue(":parentuuid", parentuuid);
    query.bindValue(":name", name);
    query.bindValue(":content", content);
    query.bindValue(":md5_content", md5_content);
    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    QJsonObject data;
    int rowid = query.lastInsertId().toInt();
    data["classbookid"] = QJsonValue(rowid);
    data["parentid"] = parentid;
    data["name"] = name;
    data["content"] = content;
    data["md5_content"] = md5_content;

    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

