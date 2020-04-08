#include <cmd_handlers_events.h>
#include <runtasks.h>
#include <fallen.h>
#include <iostream>
#include <employ_database.h>
#include <employ_server_info.h>
#include <QtCore>
#include <validators.h>

// *****************************************
// * Create public events
// *****************************************

CmdHandlerEventAdd::CmdHandlerEventAdd()
    : CmdHandlerBase("createpublicevent", "Create the public event") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireStringParam("type", "Type of event")
        .addValidator(new ValidatorEventType());
    requireStringParam("message", "Message");
}

// ---------------------------------------------------------------------

void CmdHandlerEventAdd::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;
    std::string sType = pRequest->getInputString("type", "");
    std::string sMessage = pRequest->getInputString("message", "");

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("INSERT INTO public_events(type,message,dt) VALUES(:type,:message,NOW())");
    query.bindValue(":type", QString::fromStdString(sType));
    query.bindValue(":message", QString::fromStdString(sMessage));
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *****************************************
// * Delete public event
// *****************************************


CmdHandlerEventDelete::CmdHandlerEventDelete()
    : CmdHandlerBase("deletepublicevent", "Delete public event") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireIntegerParam("eventid", "Event ID");
}

// ---------------------------------------------------------------------

void CmdHandlerEventDelete::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;

    int nEventId = pRequest->getInputInteger("eventid", -1);
    jsonResponse["eventid"] = nEventId;

    nlohmann::json jsonEvent;

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT * FROM public_events WHERE id = :eventid");
    query.bindValue(":eventid", nEventId);
    query.exec();
    if (!query.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "Event not found"));
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
    : CmdHandlerBase("getpublicevent", "Return public event info by id") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("eventid", "Event id");
}

// ---------------------------------------------------------------------

void CmdHandlerEventInfo::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;
    int nEventId = pRequest->getInputInteger("eventid", -1);

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
    } else {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "Event not found"));
        return;
    }

    jsonResponse["data"] = jsonEvent;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// *****************************************
// * public events list
// *****************************************


CmdHandlerEventsList::CmdHandlerEventsList()
    : CmdHandlerBase("publiceventslist", "Return list of public events") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    requireIntegerParam("page", "Number of page"); // TODO validator
    requireIntegerParam("onpage", "How much rows in one page"); // TODO validator
    optionalStringParam("type", "Filter by type events"); // TODO validator
    optionalStringParam("search", "Filter by message event"); // TODO validator
    
}

// ---------------------------------------------------------------------

void CmdHandlerEventsList::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;

    int nPage = pRequest->getInputInteger("page", 0);
    jsonResponse["page"] = nPage;

    int nOnPage = pRequest->getInputInteger("onpage", 10);
    if (nOnPage > 50) {
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Parameter 'onpage' could not be more then 50"));
        return;
    }
    jsonResponse["onpage"] = nOnPage;

    std::vector<std::string> vFilters;
    QMap<QString,QString> filter_values;
    
    std::string sType = pRequest->getInputString("type", "");
    WsjcppCore::trim(sType);
    if (sType != "") {
        vFilters.push_back("(e.type = :type)");
        filter_values[":type"] = QString::fromStdString(sType);
    }

    std::string sSearch = pRequest->getInputString("search", "");
    WsjcppCore::trim(sSearch);
    if (sSearch != "") {
        vFilters.push_back("(e.message LIKE :search)");
        filter_values[":search"] = "%" + QString::fromStdString(sSearch) + "%";
        jsonResponse["search"] = sSearch;
    }

    // TODO redesign join
    std::string sWhere = "";
    for (int i = 0; i < vFilters.size(); i++) {
        if (sWhere.length() > 0) {
            sWhere += " AND ";
        }
        sWhere += vFilters[i];
    }

    if (sWhere.length() > 0) {
        sWhere = "WHERE " + sWhere;
    }

    // count quests
    QSqlDatabase db = *(pDatabase->database());

    {
        QSqlQuery query(db);
        query.prepare("SELECT count(*) as cnt FROM public_events e "
            " " + QString::fromStdString(sWhere)
        );
        foreach (QString key, filter_values.keys()) {
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
            " " + QString::fromStdString(sWhere) +
            " ORDER BY dt DESC "
            " LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage)
        );
        foreach (QString key, filter_values.keys()) {
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
