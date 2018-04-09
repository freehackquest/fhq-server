#include <cmd_classbook_delete_record_handler.h>
#include <employ_database.h>
#include <QJsonArray>
#include <QSqlError>
#include <QUuid>

CmdClassbookDeleteRecordHandler::CmdClassbookDeleteRecordHandler(){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("classbookid").required().integer_().description("id for classbook article"));
}

// ---------------------------------------------------------------------

std::string CmdClassbookDeleteRecordHandler::cmd(){
    return "classbook_delete_record";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdClassbookDeleteRecordHandler::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdClassbookDeleteRecordHandler::inputs(){
    return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdClassbookDeleteRecordHandler::description(){
    return "Delete a article with a given classbookid";
}

// ---------------------------------------------------------------------

void CmdClassbookDeleteRecordHandler::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int classbookid = jsonRequest["classbookid"].toInt();

    //DELETE Record IF haven't childs
    QSqlQuery query(db);
    if(classbookid !=0){
        query.prepare("SELECT id FROM classbook WHERE parentid=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.exec();
        if (query.next()){
            pRequest->sendMessageError(cmd(), Error(403, "Could not delete, because childs exists. Please remove childs first."));
            return;
        }
        //Delete record in classbook
        query.prepare("DELETE FROM classbook WHERE id=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.exec();
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Errors::DatabaseError(query.lastError().text()));
            return;
        }

        //Delete record's localization
        query.prepare("DELETE FROM classbook_localization WHERE classbookid=:classbookid");
        query.bindValue(":classbookid", classbookid);
        query.exec();
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Errors::DatabaseError(query.lastError().text()));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

