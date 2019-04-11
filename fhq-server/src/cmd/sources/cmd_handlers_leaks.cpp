#include <cmd_handlers_leaks.h>
#include <iostream>
#include <employ_database.h>
#include <employ_leaks.h>
#include <QJsonArray>
#include <QUuid>

/*********************************************
 * This handler will return the list of leaks
**********************************************/

CmdHandlerLeaksList::CmdHandlerLeaksList()
    : CmdHandlerBase("leaks_list", "Method returns list of leaks"){

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

	// validation and description input fields
    addInputDef("page").required().integer_().description("Number of page");
    addInputDef("onpage").required().integer_().description("How much rows in one page");
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksList::handle(ModelRequest *pRequest){

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

    if(jsonRequest.contains("name")){
        QString name = jsonRequest["name"].toString().trimmed();
        if(name != ""){
            filters << "(l.name = :name)";
            filter_values[":name"] = name;
        }
    }

    IUserToken *pUserToken = pRequest->userToken();
    bool bAdmin = false;
    if(pUserToken != NULL) bAdmin = pUserToken->isAdmin();
    if(jsonRequest.contains("search") && bAdmin){
        QString search = jsonRequest["search"].toString().trimmed();
        if(search != ""){
            filters << "(l.message LIKE :search)";
            filter_values[":search"] = "%" + search + "%";
        }
        jsonResponse["search"] = search.toStdString();
    }

    QString where = filters.join(" AND ");
    if(where.length() > 0){
        where = "WHERE " + where;
    }

    QSqlDatabase db = *(pDatabase->database());

    //count leaks
    {
        QSqlQuery query(db);
        query.prepare("SELECT count(*) as cnt FROM leaks l"
            " " + where
        );
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            jsonResponse["count"] = record.value("cnt").toInt();
        }
    }

    //data
    auto jsonLeaksList = nlohmann::json::array();
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM leaks l"
            " " + where +
            " ORDER BY id "
            " LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage)
        );
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
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
    : CmdHandlerBase("leaks_add", "Method adds a leak"){

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    addInputDef("uuid").required().string_().description("UUID of the leak")
        .addValidator(new ValidatorUUID());

    addInputDef("game_uuid").required().string_().description("UUID of the game")
        .addValidator(new ValidatorUUID());

    addInputDef("name").required().string_().description("Visible part of the content");
    addInputDef("content").required().string_().description("Content of the leak");
    addInputDef("score").required().integer_().description("Price of the leak");
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksAdd::handle(ModelRequest *pRequest){
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    ModelLeak *pModelLeak = new ModelLeak();
    pModelLeak->fillFrom(jsonRequest);

    if(pModelLeak->score() <= 0){
        // todo this check move to cmd input def
        pRequest->sendMessageError(cmd(), Error(400, "Score must be more then 0"));
        return;
    }

    EmployLeaks *pEmployLeaks = findEmploy<EmployLeaks>();
    std::string sError = "";
    int nResult = pEmployLeaks->addLeak(pModelLeak, sError);

    if(nResult == EmployResult::DATABASE_ERROR){
        pRequest->sendMessageError(cmd(), Error(500, sError));
        delete pModelLeak;
        return;
    }

    if(nResult == EmployResult::ALREADY_EXISTS){
        pRequest->sendMessageError(cmd(), Error(403, "Leak already exists with this uuid"));
        delete pModelLeak;
        return;
    }

    if(nResult == EmployResult::GAME_NOT_FOUND){
        pRequest->sendMessageError(cmd(), Error(404, "Game does not exists with this uuid"));
        delete pModelLeak;
        return;
    }

    if(nResult == EmployResult::OK){
        nlohmann::json jsonResponse;
        jsonResponse["data"] = pModelLeak->toJson();
        pRequest->sendMessageSuccess(cmd(), jsonResponse);
        return;
    }else{
        pRequest->sendMessageError(cmd(), Error(500, "Server error"));
        delete pModelLeak;
    }
}

/*********************************************
 * This handler will update a leak
**********************************************/

CmdHandlerLeaksUpdate::CmdHandlerLeaksUpdate()
    : CmdHandlerBase("leaks_update", "Method updates a leak"){

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    addInputDef("id").required().integer_().description("Leak id");
    addInputDef("name").optional().string_().description("Visible part of the content");
    addInputDef("content").optional().string_().description("Content of the leak");
    addInputDef("score").optional().integer_().description("Price of the leak");
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksUpdate::handle(ModelRequest *pRequest){

    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int id = jsonRequest["id"].toInt();

    QSqlQuery query(db);
    {
        query.prepare("SELECT id FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
        if (!query.next()){
            pRequest->sendMessageError(cmd(), Error(404, "leak with this id"));
            return;
        }
    }

    QString sName;
    if(jsonRequest.contains("name")){
        sName = jsonRequest.value("name").toString().trimmed().toHtmlEscaped();
        query.prepare("UPDATE leaks SET name=:name WHERE id = :id");
        query.bindValue(":name", sName);
        query.bindValue(":id", id);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
    }

    QString sContent;
    if(jsonRequest.contains("content")){
        sContent = jsonRequest.value("content").toString().trimmed().toHtmlEscaped();
        query.prepare("UPDATE leaks SET content=:content WHERE id = :id");
        query.bindValue(":content", sContent);
        query.bindValue(":id", id);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
    }

    int nScore;
    if(jsonRequest.contains("score")){
        nScore = jsonRequest.value("score").toInt();
        query.prepare("UPDATE leaks SET score=:score WHERE id = :id");
        query.bindValue(":score", nScore);
        query.bindValue(":id", id);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * This handler will delete a leak
**********************************************/

CmdHandlerLeaksDelete::CmdHandlerLeaksDelete()
    : CmdHandlerBase("leaks_delete", "Method deletes a leak"){

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    addInputDef("id").required().integer_().description("Leak id");
    //TODO admin password
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksDelete::handle(ModelRequest *pRequest){

    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);

    int id = jsonRequest["id"].toInt();
    {
        query.prepare("SELECT id FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
        if (!query.next()){
            pRequest->sendMessageError(cmd(), Error(404, "Not found leak with this id"));
            return;
        }
    }

    //deleting from users_leaks
    {
        query.prepare("DELETE FROM users_leaks WHERE leakid = :id");
        query.bindValue(":id", id);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
    }

    //deleting from leaks_files
    {
        query.prepare("DELETE FROM leaks_files WHERE leakid = :id");
        query.bindValue(":id", id);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
    }

    //deleting from leaks
    {
        query.prepare("DELETE FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * This handler allows you to buy a leak
**********************************************/

CmdHandlerLeaksBuy::CmdHandlerLeaksBuy()
    : CmdHandlerBase("leaks_buy", "Method buys a leak"){

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(false);

    // validation and description input fields
    addInputDef("id").required().integer_().description("Leak id");
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksBuy::handle(ModelRequest *pRequest){

    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);

    int id = jsonRequest["id"].toInt();
    {
        query.prepare("SELECT id FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
        if (!query.next()){
            pRequest->sendMessageError(cmd(), Error(404, "Not found leak with this id"));
            return;
        }
    }

    IUserToken *pUserToken = pRequest->userToken();
    int nUserID = 0;
    if(pUserToken != NULL) nUserID = pUserToken->userid();

    int nScore;
    {
        query.prepare("SELECT score FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
        if (!query.next()){
            pRequest->sendMessageError(cmd(), Error(404, "Not found leak with this id"));
            return;
        }
        else
        {
            QSqlRecord record = query.record();
            nScore = record.value("score").toInt();
        }
    }

    //update the user rating
    {
        query.prepare("UPDATE users SET rating = rating - :score WHERE id = :id");
        query.bindValue(":score", nScore);
        query.bindValue(":id", nUserID);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
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
    if (!query.exec()){
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
        return;
    }

    //increment of the number of leaks sold
    {
        query.prepare("UPDATE leaks SET sold = sold + 1 WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
