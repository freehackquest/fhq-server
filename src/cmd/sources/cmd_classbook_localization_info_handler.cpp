#include <cmd_classbook_localization_info_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>

CmdClassbookLocalizationInfoHandler::CmdClassbookLocalizationInfoHandler(){
    m_vInputs.push_back(CmdInputDef("classbook_localizationid").required().integer_().description("Localization id"));
}

std::string CmdClassbookLocalizationInfoHandler::cmd(){
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

const std::vector<CmdInputDef> &CmdClassbookLocalizationInfoHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookLocalizationInfoHandler::description(){
    return "Find and display localization for an article by classbookid";
}

QStringList CmdClassbookLocalizationInfoHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookLocalizationInfoHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());

    QJsonObject data;

    QSqlQuery query(db);
    int classbook_localizationid = jsonRequest["classbook_localizationid"].toInt();
    query.prepare("SELECT id FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pRequest->sendMessageError(cmd(), Error(404, "This localization doesn't exist"));
        return;
    }

    query.prepare("SELECT classbookid, lang, name, content FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    query.next();
    QSqlRecord record = query.record();
    data["classbookid"] = record.value("classbookid").toInt();
    data["classbook_localizationid"] = classbook_localizationid;
    data["lang"] = record.value("lang").toString();
    data["name"] = record.value("name").toString();
    data["content"] = record.value("content").toString();


    jsonResponse["data"] = data;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
