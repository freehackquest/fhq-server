#include <cmd_handlers_events.h>
#include <runtasks.h>
#include <utils_logger.h>
#include <iostream>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <QtCore>
#include <validators.h>

// *****************************************
// * Create public events
// *****************************************

CmdHandlerEventAdd::CmdHandlerEventAdd()
    : CmdHandlerBase("createpublicevent", "Create the public event"){

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireStringParam("type", "Type of event")
        .addValidator(new ValidatorEventType());
    requireStringParam("message", "Message");
}

// ---------------------------------------------------------------------

void CmdHandlerEventAdd::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    QString type = jsonRequest["type"].toString().trimmed();
    QString message = jsonRequest["message"].toString().trimmed();


    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("INSERT INTO public_events(type,message,dt) VALUES(:type,:message,NOW())");
    query.bindValue(":type", type);
    query.bindValue(":message", message);
    if(!query.exec()){
        // TODO database error
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *****************************************
// * Delete public event
// *****************************************


CmdHandlerEventDelete::CmdHandlerEventDelete()
    : CmdHandlerBase("deletepublicevent", "Delete public event"){

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireIntegerParam("hintid", "hint id");
}

// ---------------------------------------------------------------------

void CmdHandlerEventDelete::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;
    

    int nEventId = jsonRequest["eventid"].toInt();
    jsonResponse["eventid"] = nEventId;

    nlohmann::json jsonEvent;

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM public_events WHERE id = :eventid");
    query.bindValue(":eventid", nEventId);
    query.exec();
    if (!query.next()) {
        pRequest->sendMessageError(cmd(), Error(404, "Event not found"));
        return;
    }

    QSqlQuery query2(db);
    query2.prepare("DELETE FROM public_events WHERE id = :eventid");
    query2.bindValue(":eventid", nEventId);
    query2.exec();
    
    jsonResponse["data"] = jsonEvent;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *****************************************
// * get public event
// *****************************************

CmdHandlerEventInfo::CmdHandlerEventInfo()
    : CmdHandlerBase("getpublicevent", "Return public event info by id"){

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("eventid", "Event id");
}

// ---------------------------------------------------------------------

void CmdHandlerEventInfo::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    int nEventId = jsonRequest["eventid"].toInt();
    jsonResponse["eventid"] = nEventId;

    nlohmann::json jsonEvent;

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM public_events e WHERE id = :eventid");
    query.bindValue(":eventid", nEventId);
    query.exec();
    if (query.next()) {
        QSqlRecord record = query.record();
        jsonEvent["dt"] = record.value("dt").toString().toStdString();
        jsonEvent["id"] = record.value("id").toInt();
        jsonEvent["type"] = record.value("type").toString().toHtmlEscaped().toStdString(); // TODO htmlspecialchars
        jsonEvent["message"] = record.value("message").toString().toHtmlEscaped().toStdString(); // TODO htmlspecialchars
    }else{
        pRequest->sendMessageError(cmd(), Error(404, "Event not found"));
        return;
    }

    jsonResponse["data"] = jsonEvent;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *****************************************
// * public events list
// *****************************************


CmdHandlerEventsList::CmdHandlerEventsList()
    : CmdHandlerBase("publiceventslist", "Return list of public events"){

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    requireIntegerParam("page", "Number of page"); // TODO validator
    requireIntegerParam("onpage", "How much rows in one page"); // TODO validator
}

// ---------------------------------------------------------------------

void CmdHandlerEventsList::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;


    int nPage = jsonRequest["page"].toInt();
    jsonResponse["page"] = nPage;

    int nOnPage = jsonRequest["onpage"].toInt();
    if(nOnPage > 50){
        pRequest->sendMessageError(cmd(), Error(400, "Parameter 'onpage' could not be more then 50"));
        return;
    }
    jsonResponse["onpage"] = nOnPage;

    QStringList filters;
    QMap<QString,QString> filter_values;
    
    if(jsonRequest.contains("type")){
        QString type = jsonRequest["type"].toString().trimmed();
        if(type != ""){
            filters << "(e.type = :type)";
            filter_values[":type"] = type;
        }
    }

    if(jsonRequest.contains("search")){
        QString search = jsonRequest["search"].toString().trimmed();
        if(search != ""){
            filters << "(e.message LIKE :search)";
            filter_values[":search"] = "%" + search + "%";
        }
        jsonResponse["search"] = search.toStdString();
    }

    QString where = filters.join(" AND ");
    if(where.length() > 0){
        where = "WHERE " + where;
    }

    // count quests
    QSqlDatabase db = *(pDatabase->database());

    {
        QSqlQuery query(db);
        query.prepare("SELECT count(*) as cnt FROM public_events e "
            " " + where
        );
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        query.exec();
        if (query.next()) {
            QSqlRecord record = query.record();
            jsonResponse["count"] = record.value("cnt").toInt();
        }
    }

    // data
    nlohmann::json jsonPublicEventsList = nlohmann::json::array();

    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM public_events e"
            " " + where +
            " ORDER BY dt DESC "
            " LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage)
        );
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        query.exec();
        while (query.next()) {
            QSqlRecord record = query.record();
            nlohmann::json jsonEvent;
            jsonEvent["dt"] = record.value("dt").toString().toStdString();
            jsonEvent["id"] = record.value("id").toInt();
            jsonEvent["type"] = record.value("type").toString().toHtmlEscaped().toStdString(); // TODO htmlspecialchars
            jsonEvent["message"] = record.value("message").toString().toHtmlEscaped().toStdString(); // TODO htmlspecialchars

            jsonPublicEventsList.push_back(jsonEvent);
        }
    }

    jsonResponse["data"] = jsonPublicEventsList;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
