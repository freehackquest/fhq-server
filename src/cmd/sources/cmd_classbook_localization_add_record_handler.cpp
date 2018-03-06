#include <cmd_classbook_localization_add_record_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>
#include <QUuid>
#include <QCryptographicHash>

CmdClassbookLocalizationAddRecordHandler::CmdClassbookLocalizationAddRecordHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("Classbookid for article localization"));
    m_vInputs.push_back(CmdInputDef("lang").required().string_().description("Language"));
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("Article name"));
    m_vInputs.push_back(CmdInputDef("content").required().string_().description("The content of the article"));
}

// ---------------------------------------------------------------------

std::string CmdClassbookLocalizationAddRecordHandler::cmd(){
    return "classbook_localization_add_record";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdClassbookLocalizationAddRecordHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdClassbookLocalizationAddRecordHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdClassbookLocalizationAddRecordHandler::description(){
    return "Add a new article localization for the English version";
}

// ---------------------------------------------------------------------

void CmdClassbookLocalizationAddRecordHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    int classbookid = jsonRequest["classbookid"].toInt();

    QSqlDatabase db = *(pRequest->server()->database());

    QJsonObject data;

    QSqlQuery query(db);
    QString lang = jsonRequest["lang"].toString().trimmed();
    query.prepare("SELECT lang FROM classbook_localization WHERE lang = :lang AND classbookid=:classbookid");
    query.bindValue(":lang", jsonRequest["lang"].toString().trimmed());
    query.bindValue(":classbookid", classbookid);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if(query.next()){
        pRequest->sendMessageError(cmd(), Error(400, "This lang already exist"));
        return;
    }

    QString name = jsonRequest["name"].toString().trimmed();
    QString content = jsonRequest["content"].toString().trimmed();

    //Set md5_content hash
    QString md5_content = QString(QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5).toHex());

    //generate uuid
    QString uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "");

    query.prepare("INSERT INTO classbook_localization("
                  "classbookid,"
                  "uuid,"
                  "lang,"
                  "name,"
                  "content,"
                  "md5_content,"
                  "created,"
                  "updated"
                  ") "
                  "VALUES("
                  ":classbookid,"
                  ":uuid,"
                  ":lang,"
                  ":name,"
                  ":content,"
                  ":md5_content,"
                  "NOW(),"
                  "NOW()"
                  ")");
    query.bindValue(":classbookid", classbookid);
    query.bindValue(":uuid", uuid);
    query.bindValue(":lang", lang);
    query.bindValue(":name", name);
    query.bindValue(":content", content);
    query.bindValue(":md5_content", md5_content);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    int rowid = query.lastInsertId().toInt();
    data["classbookid"] = classbookid;
    data["classbook_localizationid"] = QJsonValue(rowid);
    data["lang"] = lang;
    data["name"] = name;
    data["content"] = content;
    data["md5_content"] = md5_content;

    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
