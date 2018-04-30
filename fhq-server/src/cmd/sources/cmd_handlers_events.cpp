#include <cmd_handlers_events.h>
#include <log.h>
#include <runtasks.h>
#include <log.h>
#include <iostream>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <QtCore>
#include <SmtpMime>

// *****************************************
// * Create public events
// *****************************************

CmdHandlerEventAdd::CmdHandlerEventAdd()
    : CmdHandlerBase("createpublicevent", "Create the public event"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    QStringList eventTypes;
    // TODO load from database
    eventTypes << "info";
    eventTypes << "users";
    eventTypes << "games";
    eventTypes << "quests";
    eventTypes << "warning";

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("type").enum_(eventTypes).required());
    m_vInputs.push_back(CmdInputDef("message").string_().required());
}

// ---------------------------------------------------------------------

void CmdHandlerEventAdd::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

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

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("hintid").required().integer_().description("hint id"));

}

// ---------------------------------------------------------------------

void CmdHandlerEventDelete::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonData;
    jsonData["cmd"] = QJsonValue(QString(cmd().c_str()));

    int nEventId = jsonRequest["eventid"].toInt();
    jsonData["eventid"] = nEventId;

    QJsonObject event;

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM public_events WHERE id = :eventid");
    query.bindValue(":eventid", nEventId);
    query.exec();
    if (!query.next()) {
        pRequest->sendMessageError(cmd(), Errors::EventNotFound());
        return;
    }

    QSqlQuery query2(db);
    query2.prepare("DELETE FROM public_events WHERE id = :eventid");
    query2.bindValue(":eventid", nEventId);
    query2.exec();

    jsonData["result"] = QJsonValue("DONE");
    jsonData["data"] = event;
    pRequest->sendMessageSuccess(cmd(), jsonData);
}

// *****************************************
// * get public event
// *****************************************

CmdHandlerEventInfo::CmdHandlerEventInfo()
    : CmdHandlerBase("getpublicevent", "Return public event info by id"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("eventid").required().integer_().description("Event id"));
}

// ---------------------------------------------------------------------

void CmdHandlerEventInfo::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    int nEventId = jsonRequest["eventid"].toInt();
    jsonResponse["eventid"] = nEventId;

    QJsonObject event;

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM public_events e WHERE id = :eventid");
    query.bindValue(":eventid", nEventId);
    query.exec();
    if (query.next()) {
        QSqlRecord record = query.record();
        event["dt"] = record.value("dt").toString();
        event["id"] = record.value("id").toInt();
        event["type"] = record.value("type").toString().toHtmlEscaped(); // TODO htmlspecialchars
        event["message"] = record.value("message").toString().toHtmlEscaped(); // TODO htmlspecialchars
    }else{
        pRequest->sendMessageError(cmd(), Errors::EventNotFound());
        return;
    }

    jsonResponse["data"] = event;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *****************************************
// * public events list
// *****************************************


CmdHandlerEventsList::CmdHandlerEventsList()
    : CmdHandlerBase("publiceventslist", "Return list of public events"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("page").required().integer_().description("Number of page"));
    m_vInputs.push_back(CmdInputDef("onpage").required().integer_().description("How much rows in one page"));
}

// ---------------------------------------------------------------------

void CmdHandlerEventsList::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;


    int nPage = jsonRequest["page"].toInt();
    jsonResponse["page"] = nPage;

    int nOnPage = jsonRequest["onpage"].toInt();
    if(nOnPage > 50){
        pRequest->sendMessageError(cmd(), Errors::OnPageCouldNotBeMoreThen50());
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
        jsonResponse["search"] = search;
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
    QJsonArray publiceventslist;
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
            QJsonObject event;
            event["dt"] = record.value("dt").toString();
            event["id"] = record.value("id").toInt();
            event["type"] = record.value("type").toString().toHtmlEscaped(); // TODO htmlspecialchars
            event["message"] = record.value("message").toString().toHtmlEscaped(); // TODO htmlspecialchars

            publiceventslist.push_back(event);
        }
    }

    jsonResponse["data"] = publiceventslist;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
