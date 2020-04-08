#include <cmd_handlers_leaks.h>
#include <iostream>
#include <employ_database.h>
#include <employ_leaks.h>
#include <QJsonArray>

/*********************************************
 * This handler will return the list of leaks
**********************************************/

CmdHandlerLeaksList::CmdHandlerLeaksList()
    : CmdHandlerBase("leaks_list", "Method returns list of leaks") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("page", "Number of page"); // TODO validator
    requireIntegerParam("onpage", "How much rows in one page"); // TODO validator
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksList::handle(ModelRequest *pRequest) {

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

    std::string sName = pRequest->getInputString("name", "");
    WsjcppCore::trim(sName);
    if (sName != "") {
        vFilters.push_back("(l.name = :name)");
        filter_values[":name"] = QString::fromStdString(sName);
    }
    

    bool bAdmin = pRequest->isAdmin();
    std::string sSearch = pRequest->getInputString("search", "");
    WsjcppCore::trim(sSearch);

    if (sSearch != "" && bAdmin) {
        vFilters.push_back("(l.message LIKE :search)");
        filter_values[":search"] = "%" + QString::fromStdString(sSearch) + "%";
        jsonResponse["search"] = sSearch;
    }
    std::string sWhere = "";
    for (int i = 0; i > vFilters.size(); i++) {
        if (sWhere.length() > 0) {
            sWhere += " AND ";
        }
        sWhere += vFilters[i];
    }

    if (sWhere.length() > 0) {
        sWhere = "WHERE " + sWhere;
    }

    QSqlDatabase db = *(pDatabase->database());

    //count leaks
    {
        QSqlQuery query(db);
        query.prepare("SELECT count(*) as cnt FROM leaks l"
            " " + QString::fromStdString(sWhere)
        );
        foreach (QString key, filter_values.keys()) {
            query.bindValue(key, filter_values.value(key));
        }
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            jsonResponse["count"] = record.value("cnt").toInt();
        }
    }

    //data
    nlohmann::json jsonLeaksList = nlohmann::json::array();
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM leaks l"
            " " + QString::fromStdString(sWhere) +
            " ORDER BY id "
            " LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage)
        );
        foreach (QString key, filter_values.keys()) {
            query.bindValue(key, filter_values.value(key));
        }
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        while (query.next()) {
            QSqlRecord record = query.record();
            nlohmann::json jsonLeak;
            jsonLeak["id"] = record.value("id").toInt();
            jsonLeak["uuid"] = record.value("uuid").toString().toHtmlEscaped().toStdString();
            jsonLeak["gameid"] = record.value("gameid").toInt();
            jsonLeak["name"] = record.value("name").toString().toHtmlEscaped().toStdString();
            jsonLeak["score"] = record.value("score").toInt();
            jsonLeak["created"] = record.value("created").toString().toHtmlEscaped().toStdString();
            jsonLeak["updated"] = record.value("updated").toString().toHtmlEscaped().toStdString();
            jsonLeak["sold"] = record.value("sold").toInt();

            jsonLeaksList.push_back(jsonLeak);
        }
    }

    jsonResponse["data"] = jsonLeaksList;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * This handler will add one new leak
**********************************************/

CmdHandlerLeaksAdd::CmdHandlerLeaksAdd()
    : CmdHandlerBase("leaks_add", "Method adds a leak") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireStringParam("uuid", "UUID of the leak")
        .addValidator(new WsjcppValidatorUUID()); // may be optional ?

    requireStringParam("game_uuid", "UUID of the game")
        .addValidator(new WsjcppValidatorUUID());

    requireStringParam("name", "Visible part of the content");
    requireStringParam("content", "Content of the leak");
    requireIntegerParam("score", "Price of the leak"); // TODO validator
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksAdd::handle(ModelRequest *pRequest) {
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    ModelLeak *pModelLeak = new ModelLeak();
    pModelLeak->fillFrom(jsonRequest);

    if (pModelLeak->score() <= 0) {
        // todo this check move to cmd input def
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Score must be more then 0"));
        return;
    }

    EmployLeaks *pEmployLeaks = findWsjcppEmploy<EmployLeaks>();
    std::string sError = "";
    int nResult = pEmployLeaks->addLeak(pModelLeak, sError);

    if (nResult == EmployResult::DATABASE_ERROR) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, sError));
        delete pModelLeak;
        return;
    }

    if (nResult == EmployResult::ALREADY_EXISTS) {
        pRequest->sendMessageError(cmd(), WsjcppError(403, "Leak already exists with this uuid"));
        delete pModelLeak;
        return;
    }

    if (nResult == EmployResult::GAME_NOT_FOUND) {
        pRequest->sendMessageError(cmd(), WsjcppError(404, "Game does not exists with this uuid"));
        delete pModelLeak;
        return;
    }

    if (nResult == EmployResult::OK) {
        nlohmann::json jsonResponse;
        jsonResponse["data"] = pModelLeak->toJson();
        pRequest->sendMessageSuccess(cmd(), jsonResponse);
        return;
    } else {
        pRequest->sendMessageError(cmd(), WsjcppError(500, "Server error"));
        delete pModelLeak;
    }
}

/*********************************************
 * This handler will update a leak
**********************************************/

CmdHandlerLeaksUpdate::CmdHandlerLeaksUpdate()
    : CmdHandlerBase("leaks_update", "Method updates a leak") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireIntegerParam("id", "Leak id");
    optionalStringParam("name", "Visible part of the content");
    optionalStringParam("content", "Content of the leak");
    optionalIntegerParam("score", "Price of the leak");
    
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksUpdate::handle(ModelRequest *pRequest) {

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int id = pRequest->getInputInteger("id", -1);

    QSqlQuery query(db);
    {
        query.prepare("SELECT id FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (!query.next()) {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "leak with this id"));
            return;
        }
    }

    if (pRequest->hasInputParam("name")) {
        std::string sName = pRequest->getInputString("name", "");
        WsjcppCore::trim(sName);
        query.prepare("UPDATE leaks SET name=:name WHERE id = :id");
        query.bindValue(":name", QString::fromStdString(sName));
        query.bindValue(":id", id);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    if (pRequest->hasInputParam("content")) {
        std::string sContent = pRequest->getInputString("content", "");
        WsjcppCore::trim(sContent);
        query.prepare("UPDATE leaks SET content=:content WHERE id = :id");
        query.bindValue(":content", QString::fromStdString(sContent));
        query.bindValue(":id", id);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }
    
    if (pRequest->hasInputParam("score")) {
        int nScore = pRequest->getInputInteger("score", 0);
        query.prepare("UPDATE leaks SET score=:score WHERE id = :id");
        query.bindValue(":score", nScore);
        query.bindValue(":id", id);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * This handler will delete a leak
**********************************************/

CmdHandlerLeaksDelete::CmdHandlerLeaksDelete()
    : CmdHandlerBase("leaks_delete", "Method deletes a leak") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("id", "Leak id");
    //TODO admin password
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksDelete::handle(ModelRequest *pRequest) {

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);

    int id = pRequest->getInputInteger("id", -1);
    {
        query.prepare("SELECT id FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (!query.next()) {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Not found leak with this id"));
            return;
        }
    }

    //deleting from users_leaks
    {
        query.prepare("DELETE FROM users_leaks WHERE leakid = :id");
        query.bindValue(":id", id);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    //deleting from leaks_files
    {
        query.prepare("DELETE FROM leaks_files WHERE leakid = :id");
        query.bindValue(":id", id);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    //deleting from leaks
    {
        query.prepare("DELETE FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * This handler allows you to buy a leak
**********************************************/

CmdHandlerLeaksBuy::CmdHandlerLeaksBuy()
    : CmdHandlerBase("leaks_buy", "Method buys a leak") {

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(false);

    // validation and description input fields
    requireIntegerParam("id", "Leak id");
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksBuy::handle(ModelRequest *pRequest) {

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);

    int id = pRequest->getInputInteger("id", 0);
    {
        query.prepare("SELECT id FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (!query.next()) {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Not found leak with this id"));
            return;
        }
    }

    WsjcppUserSession *pUserSession = pRequest->getUserSession();
    int nUserID = 0;
    if (pUserSession != nullptr) {
        nUserID = pUserSession->userid();
    }

    int nScore = 0;
    {
        query.prepare("SELECT score FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (!query.next()) {
            pRequest->sendMessageError(cmd(), WsjcppError(404, "Not found leak with this id"));
            return;
        } else {
            QSqlRecord record = query.record();
            nScore = record.value("score").toInt();
        }
    }

    //update the user rating
    {
        query.prepare("UPDATE users SET rating = rating - :score WHERE id = :id");
        query.bindValue(":score", nScore);
        query.bindValue(":id", nUserID);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    //attaching a leak to the user
    query.prepare("INSERT INTO users_leaks("
                  "leakid,"
                  "userid,"
                  "grade,"
                  "dt"
                  ")"
                  "VALUES("
                  ":leakid,"
                  ":userid,"
                  ":grade,"
                  "NOW()"
                  ")");
    query.bindValue(":leakid", id);
    query.bindValue(":userid", nUserID);
    query.bindValue(":grade", -1);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    //increment of the number of leaks sold
    {
        query.prepare("UPDATE leaks SET sold = sold + 1 WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
