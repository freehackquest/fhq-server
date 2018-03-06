#include <cmd_classbook_localization_delete_record_handler.h>
#include <QJsonArray>
#include <QSqlError>
#include <log.h>

CmdClassbookLocalizationDeleteRecordHandler::CmdClassbookLocalizationDeleteRecordHandler(){
    m_vInputs.push_back(CmdInputDef("classbook_localizationid").required().integer_().description("Localization id"));
}

std::string CmdClassbookLocalizationDeleteRecordHandler::cmd(){
    return "classbook_localization_delete_record";
}

bool CmdClassbookLocalizationDeleteRecordHandler::accessUnauthorized(){
    return false;
}

bool CmdClassbookLocalizationDeleteRecordHandler::accessUser(){
    return false;
}

bool CmdClassbookLocalizationDeleteRecordHandler::accessTester(){
    return false;
}

bool CmdClassbookLocalizationDeleteRecordHandler::accessAdmin(){
    return true;
}

const std::vector<CmdInputDef> &CmdClassbookLocalizationDeleteRecordHandler::inputs(){
    return m_vInputs;
};

QString CmdClassbookLocalizationDeleteRecordHandler::description(){
    return "Delete an article localization";
}

QStringList CmdClassbookLocalizationDeleteRecordHandler::errors(){
    QStringList	list;
    return list;
}

void CmdClassbookLocalizationDeleteRecordHandler::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pRequest->server()->database());

    QSqlQuery query(db);
    int classbook_localizationid = jsonRequest["classbook_localizationid"].toInt();
    query.prepare("SELECT id FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", jsonRequest["classbook_localizationid"].toInt());
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }
    if(!query.next()){
        pRequest->sendMessageError(cmd(), Error(404, "This localization doesn't exist"));
        return;
    }
    query.prepare("DELETE FROM classbook_localization WHERE id = :classbook_localizationid");
    query.bindValue(":classbook_localizationid", classbook_localizationid);
    if(!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

