#include <cmd_handlers_leaks.h>
#include <iostream>
#include <employ_database.h>
#include <QJsonArray>
#include <QUuid>

/*********************************************
 * This handler will return the list of leaks
**********************************************/

CmdHandlerLeaksList::CmdHandlerLeaksList()
    : CmdHandlerBase("leaks_list", "Method returns list of leaks"){

    m_modelCommandAccess.setAccessUnauthorized(true);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(true);

	// validation and description input fields
    m_vInputs.push_back(CmdInputDef("page").required().integer_().description("Number of page"));
    m_vInputs.push_back(CmdInputDef("onpage").required().integer_().description("How much rows in one page"));
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksList::handle(ModelRequest *pRequest){

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

    if(jsonRequest.contains("name")){
        QString name = jsonRequest["name"].toString().trimmed();
        if(name != ""){
            filters << "(l.name = :name)";
            filter_values[":name"] = name;
        }
    }

    if(jsonRequest.contains("search")){
        QString search = jsonRequest["search"].toString().trimmed();
        if(search != ""){
            filters << "(l.message LIKE :search)";
            filter_values[":search"] = "%" + search + "%";
        }
        jsonResponse["search"] = search;
    }

    QString where = filters.join(" AND ");
    if(where.length() > 0){
        where = "WHERE " + where;
    }

    QSqlDatabase db = *(pDatabase->database());

    //count leaks
    {
        QSqlQuery query(db);
        query.prepare("SELECT count(*) as cnt FROM leaks l "
            " " + where
        );
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            jsonResponse["count"] = record.value("cnt").toInt();
        }
    }

    //data
    QJsonArray leakslist;
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        while (query.next()) {
            QSqlRecord record = query.record();
            QJsonObject leak;
            leak["id"] = record.value("id").toInt();
            leak["uuid"] = record.value("uuid").toString().toHtmlEscaped();
            leak["gameid"] = record.value("gameid").toInt();
            leak["name"] = record.value("name").toString().toHtmlEscaped();
            leak["name"] = record.value("name").toString().toHtmlEscaped();
            leak["content"] = record.value("content").toString().toHtmlEscaped();
            leak["score"] = record.value("score").toInt();
            leak["created"] = record.value("created").toString().toHtmlEscaped();
            leak["updated"] = record.value("updated").toString().toHtmlEscaped();
            leak["sold"] = record.value("sold").toInt();

            leakslist.push_back(leak);
        }
    }

    jsonResponse["data"] = leakslist;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * This handler will add one new leak
**********************************************/

CmdHandlerLeaksAdd::CmdHandlerLeaksAdd()
    : CmdHandlerBase("leaks_add", "Method adds a leak"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("game_uuid").required().uuid_().description("UUID of the game"));
    m_vInputs.push_back(CmdInputDef("leak_uuid").required().uuid_().description("UUID of the leak"));
    m_vInputs.push_back(CmdInputDef("name").required().string_().description("Visible part of the content"));
    m_vInputs.push_back(CmdInputDef("content").required().string_().description("Content of the leak"));
    m_vInputs.push_back(CmdInputDef("score").required().integer_().description("Price of the leak"));
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksAdd::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    nlohmann::json jsonRequest = pRequest->jsonRequest();

    // game_uuid - new leak uuid
    // TODO trim
    std::string sGameUuid = "";
    if(jsonRequest["game_uuid"].is_string()){
        sGameUuid = jsonRequest["game_uuid"];
    }

    // leak_uuid - new leak uuid
    // TODO trim
    std::string sLeakUuid = "";
    if(jsonRequest["leak_uuid"].is_string()){
        sLeakUuid = jsonRequest["leak_uuid"];
    }

    // name
    // TODO trim
    std::string sName = "";
    if(jsonRequest["name"].is_string()){
        sName = jsonRequest["name"];
    }

    // content 
    // TODO trim
    std::string sContent = "";
    if(jsonRequest["content"].is_string()){
        sContent = jsonRequest["content"];
    }

    // score
    int nScore = 0;
    if(jsonRequest["score"].is_number_integer()){
        nScore = jsonRequest.at("score");
    }
    if(nScore <= 0){
        // todo this check move to cmd input def
        pRequest->sendMessageError(cmd(), Error(400, "Score must be more then 0"));
        return;
    }

    int nGameId = 0;

    QSqlDatabase db = *(pDatabase->database());

    // check the game
    {
        QSqlQuery query(db);
        query.prepare("SELECT id,uuid FROM games WHERE uuid = :game_uuid");
        query.bindValue(":game_uuid", QString(sGameUuid.c_str()));
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if (!query.next()){
            pRequest->sendMessageError(cmd(), Errors::NotFound("Game does not exists with this uuid"));
            return;
        }

        QSqlRecord record = query.record();
        nGameId = record.value("id").toInt();
    }
    
    // check the leak (possible raise condition)
    {
        QSqlQuery query(db);
        query.prepare("SELECT id FROM leaks WHERE uuid = :leak_uuid");
        query.bindValue(":leak_uuid", QString(sLeakUuid.c_str()));
        // TODO use define ?
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if (query.next()){
            pRequest->sendMessageError(cmd(), Error(403, "Leak already exists with this uuid"));
            return;
        }
    }

    // insert new leak
    {
        QSqlQuery query(db);
        query.prepare(
            "INSERT INTO leaks( "
            " uuid, gameid, name, "
            " content, score, sold, "
            " created, updated "
            ") "
            "VALUES( "
            " :uuid, :gameid, :name,"
            " :content, :score, :sold, "
            " NOW(), NOW()"
            ");");
        query.bindValue(":uuid", QString(sLeakUuid.c_str()));
        query.bindValue(":gameid", nGameId);
        query.bindValue(":name", QString(sName.c_str()));
        query.bindValue(":content", QString(sContent.c_str()));
        query.bindValue(":score", nScore);
        query.bindValue(":sold", 0);

        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    // get leak for return
    nlohmann::json jsonLeak;
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM leaks WHERE uuid = :leak_uuid");
        query.bindValue(":leak_uuid", QString(sLeakUuid.c_str()));
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if (!query.next()){
            pRequest->sendMessageError(cmd(), Errors::NotFound("game with this uuid"));
            return;
        }

        QSqlRecord record = query.record();
        jsonLeak["uuid"] = record.value("uuid").toString().toStdString();
        jsonLeak["gameid"] = record.value("gameid").toInt();
        // TODO game uuid
        jsonLeak["score"] = record.value("score").toInt();
        jsonLeak["sold"] = record.value("sold").toInt();
        jsonLeak["name"] = record.value("name").toString().toHtmlEscaped().toStdString();
        jsonLeak["content"] = record.value("content").toString().toHtmlEscaped().toStdString();
    }

    nlohmann::json jsonResponse;
    jsonResponse["data"] = jsonLeak;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * This handler will update a leak
**********************************************/

CmdHandlerLeaksUpdate::CmdHandlerLeaksUpdate()
    : CmdHandlerBase("leaks_update", "Method updates a leak"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("id").required().integer_().description("Leak id"));
    m_vInputs.push_back(CmdInputDef("name").optional().string_().description("Visible part of the content"));
    m_vInputs.push_back(CmdInputDef("content").optional().string_().description("Content of the leak"));
    m_vInputs.push_back(CmdInputDef("score").optional().integer_().description("Price of the leak"));
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksUpdate::handle(ModelRequest *pRequest){

    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    int id = jsonRequest["id"].toInt();

    QSqlQuery query(db);
    {
        query.prepare("SELECT id FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if (!query.next()){
            pRequest->sendMessageError(cmd(), Errors::NotFound("leak with this id"));
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("id").required().integer_().description("Leak id"));
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksDelete::handle(ModelRequest *pRequest){

    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);

    int id = jsonRequest["id"].toInt();
    {
        query.prepare("SELECT id FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if (!query.next()){
            pRequest->sendMessageError(cmd(), Errors::NotFound("leak with this id"));
            return;
        }
    }

    //deleting from users_leaks
    {
        query.prepare("DELETE FROM users_leaks WHERE leakid = :id");
        query.bindValue(":id", id);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    //deleting from leaks_files
    {
        query.prepare("DELETE FROM leaks_files WHERE leakid = :id");
        query.bindValue(":id", id);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    //deleting from leaks
    {
        query.prepare("DELETE FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(true);
    m_modelCommandAccess.setAccessAdmin(false);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("id").required().integer_().description("Leak id"));
}

// ---------------------------------------------------------------------

void CmdHandlerLeaksBuy::handle(ModelRequest *pRequest){

    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QSqlDatabase db = *(pDatabase->database());

    QSqlQuery query(db);

    int id = jsonRequest["id"].toInt();
    {
        query.prepare("SELECT id FROM leaks WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if (!query.next()){
            pRequest->sendMessageError(cmd(), Errors::NotFound("leak with this id"));
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if (!query.next()){
            pRequest->sendMessageError(cmd(), Errors::NotFound("leak with this id"));
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
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
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
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
        return;
    }

    //increment of the number of leaks sold
    {
        query.prepare("UPDATE leaks SET sold = sold + 1 WHERE id = :id");
        query.bindValue(":id", id);
        if (!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
