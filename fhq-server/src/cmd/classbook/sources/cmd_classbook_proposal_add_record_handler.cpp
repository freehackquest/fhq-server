#include <cmd_classbook_proposal_add_record_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>
#include <QUuid>
#include <QCryptographicHash>
#include <employ_database.h>

CmdClassbookProposalAddRecordHandler::CmdClassbookProposalAddRecordHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("Classbookid for an article"));
    m_vInputs.push_back(CmdInputDef("lang").required().string_().description("Language"));
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("Article name"));
    m_vInputs.push_back(CmdInputDef("content").required().string_().description("The content of the article"));
}

// ---------------------------------------------------------------------

std::string CmdClassbookProposalAddRecordHandler::cmd(){
    return "classbook_proposal_add_record";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdClassbookProposalAddRecordHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdClassbookProposalAddRecordHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdClassbookProposalAddRecordHandler::description(){
    return "Propose an update of article";
}

// ---------------------------------------------------------------------

void CmdClassbookProposalAddRecordHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QJsonObject data;

    QSqlQuery query(db);
    int classbookid = jsonRequest["classbookid"].toInt();
    QString lang = jsonRequest["lang"].toString().trimmed();
    QString name = jsonRequest["name"].toString().trimmed();
    QString content = jsonRequest["content"].toString().trimmed();

    //obtain a current version of classbook name and content
    if(lang=="en"){
        query.prepare("SELECT name, content FROM classbook WHERE id = :classbookid");
        query.bindValue(":classbookid", classbookid);
    }
    else {
        query.prepare("SELECT name, content FROM classbook_localization WHERE lang = :lang");
        query.bindValue(":lang", lang);
    }
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pRequest->sendMessageError(cmd(), Error(404, "This article or localization doesn't exist"));
        return;
    }
    QSqlRecord record = query.record();
    QString name_before = record.value("name").toString();
    QString content_before = record.value("content").toString();

    //Set md5_content hash
    QString md5_content = QString(QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5).toHex());

    //generate uuid
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");

    query.prepare("INSERT INTO classbook_proposal("
                  "classbookid,"
                  "uuid,"
                  "lang,"
                  "name,"
                  "content,"
                  "name_before,"
                  "content_before,"
                  "md5_content,"
                  "created"
                  ") "
                  "VALUES("
                  ":classbookid,"
                  ":uuid,"
                  ":lang,"
                  ":name,"
                  ":content,"
                  ":name_before,"
                  ":content_before,"
                  ":md5_content,"
                  "NOW()"
                  ")");
    query.bindValue(":classbookid", classbookid);
    query.bindValue(":uuid", uuid);
    query.bindValue(":lang", lang);
    query.bindValue(":name", name);
    query.bindValue(":content", content);
    query.bindValue(":name_before", name_before);
    query.bindValue(":content_before", content_before);
    query.bindValue(":md5_content", md5_content);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    int rowid = query.lastInsertId().toInt();
    data["classbookid"] = classbookid;
    data["classbook_proposal_id"] = QJsonValue(rowid);
    data["lang"] = lang;
    data["name"] = name;
    data["content"] = content;
    data["name_before"] = name_before;
    data["content_before"] = content_before;
    data["md5_content"] = md5_content;

    jsonResponse["data"] = data;

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

