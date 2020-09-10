#include <cmd_handlers_events.h>
#include <runtasks.h>
#include <iostream>
#include <employ_database.h>
#include <employ_server_info.h>
#include <QtCore>
#include <validators.h>
#include <QSqlQuery> // TODO deprecated
#include <QSqlRecord> // TODO deprecated
#include <QString> // TODO deprecated
#include <QVariant> // TODO deprecated

// *****************************************
// * Create public events
// *****************************************
REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerEventAdd)

CmdHandlerEventAdd::CmdHandlerEventAdd()
    : WsjcppJsonRpc20HandlerBase("createpublicevent", "Create the public event") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireStringParam("type", "Type of event")
        .addValidator(new ValidatorEventType());
    requireStringParam("message", "Message");
}

// ---------------------------------------------------------------------

void CmdHandlerEventAdd::handle(WsjcppJsonRpc20Request *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;
    std::string sType = pRequest->getInputString("type", "");
    std::string sMessage = pRequest->getInputString("message", "");

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("INSERT INTO public_events(type,message,meta,dt) VALUES(:type,:message,:meta,NOW())");
    query.bindValue(":type", QString::fromStdString(sType));
    query.bindValue(":message", QString::fromStdString(sMessage));
    query.bindValue(":meta", "{}");
    if (!query.exec()) {
        pRequest->fail(WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
        return;
    }

    pRequest->done(jsonResponse);
}

// *****************************************
// * Delete public event
// *****************************************
REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerEventDelete)

CmdHandlerEventDelete::CmdHandlerEventDelete()
    : WsjcppJsonRpc20HandlerBase("deletepublicevent", "Delete public event") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireIntegerParam("eventid", "Event ID");
}

// ---------------------------------------------------------------------

void CmdHandlerEventDelete::handle(WsjcppJsonRpc20Request *pRequest) {
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
        pRequest->fail(WsjcppJsonRpc20Error(404, "Event not found"));
        return;
    }

    QSqlQuery query2(db);
    query2.prepare("DELETE FROM public_events WHERE id = :eventid");
    query2.bindValue(":eventid", nEventId);
    query2.exec();
    
    jsonResponse["data"] = jsonEvent;
    pRequest->done(jsonResponse);
}

// *****************************************
// * get public event
// *****************************************

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerEventInfo)

CmdHandlerEventInfo::CmdHandlerEventInfo()
    : WsjcppJsonRpc20HandlerBase("getpublicevent", "Return public event info by id") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("eventid", "Event id");
}

// ---------------------------------------------------------------------

void CmdHandlerEventInfo::handle(WsjcppJsonRpc20Request *pRequest) {
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
        pRequest->fail(WsjcppJsonRpc20Error(404, "Event not found"));
        return;
    }

    jsonResponse["data"] = jsonEvent;
    pRequest->done(jsonResponse);
}

// *****************************************
// * public events list
// *****************************************
REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdHandlerEventsList)

CmdHandlerEventsList::CmdHandlerEventsList()
    : WsjcppJsonRpc20HandlerBase("publiceventslist", "Return list of public events") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    requireIntegerParam("page", "Number of page"); // TODO validator
    requireIntegerParam("onpage", "How much rows in one page"); // TODO validator
    optionalStringParam("type", "Filter by type events"); // TODO validator
    optionalStringParam("search", "Filter by message event"); // TODO validator
    
}

// ---------------------------------------------------------------------

void CmdHandlerEventsList::handle(WsjcppJsonRpc20Request *pRequest) {
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;

    int nPage = pRequest->getInputInteger("page", 0);
    jsonResponse["page"] = nPage;

    int nOnPage = pRequest->getInputInteger("onpage", 10);
    if (nOnPage > 50) {
        pRequest->fail(WsjcppJsonRpc20Error(400, "Parameter 'onpage' could not be more then 50"));
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
            std::string sMeta = record.value("meta").toString().toStdString();
            if (nlohmann::json::accept(sMeta)) {
                jsonEvent["meta"] = nlohmann::json::parse(sMeta);
            } else {
                jsonEvent["meta"] = "";
            }

            jsonPublicEventsList.push_back(jsonEvent);
        }
    }

    jsonResponse["data"] = jsonPublicEventsList;
    pRequest->done(jsonResponse);
}
