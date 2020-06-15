#include "cmd_handlers_useful_links.h"
#include <fallen.h>
#include <md5.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <employ_notify.h>

/*********************************************
 * Useful Links List
**********************************************/

REGISTRY_CMD(CmdHandlerUsefulLinksList)

CmdHandlerUsefulLinksList::CmdHandlerUsefulLinksList()
    : CmdHandlerBase("useful_links_list", "Method will be return list of useful links") {
    
    setActivatedFromVersion("0.2.21");

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    optionalStringParam("filter", "Filter by word");
}


// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksList::handle(ModelRequest *pRequest) {
    nlohmann::json jsonRequest = pRequest->jsonRequest();
    int nUserId = 0;
    WsjcppUserSession *pSession = pRequest->getUserSession();
    if (pSession != nullptr) {
        nUserId = pSession->userid();
    }

    bool bIsAdmin = pRequest->isAdmin();

    std::string sFilter = pRequest->getInputString("filter", "");

    nlohmann::json jsonData = nlohmann::json::array();
    QString sWhere = "";
    if (!bIsAdmin) {
        // sWhere = " WHERE status = 'ok' ";
    }

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    // SELECT t0.*, t1.userid FROM useful_links t0 LEFT JOIN useful_links_user_favorites t1 ON t1.usefullinkid = t0.id WHERE (t1.userid = 50 OR ISNULL(t1.userid)) ORDER BY user_favorites DESC

    // TODO paginator
    query.prepare("SELECT t0.*, t1.userid FROM useful_links t0 LEFT JOIN useful_links_user_favorites t1 ON t1.usefullinkid = t0.id AND t1.userid = :userid ORDER BY user_favorites DESC");
    query.bindValue(":userid", nUserId);

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        nlohmann::json jsonLink;
        jsonLink["id"] = record.value("id").toInt();
        jsonLink["url"] = record.value("url").toString().toHtmlEscaped().toStdString();
        jsonLink["description"] = record.value("description").toString().toHtmlEscaped().toStdString();
        jsonLink["author"] = record.value("message").toString().toHtmlEscaped().toStdString();
        jsonLink["user_favorites"] = record.value("user_favorites").toInt();
        jsonLink["dt"] = record.value("dt").toString().toStdString();
        if (record.value("userid").toInt() == nUserId && nUserId != 0) {
            jsonLink["favorite"] = true;
        } else {
            jsonLink["favorite"] = false;
        }
        jsonData.push_back(jsonLink);
    }
    
    nlohmann::json jsonResponse;
    jsonResponse["data"] = jsonData;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * Useful Links Retrieve
**********************************************/

REGISTRY_CMD(CmdHandlerUsefulLinksRetrieve)

CmdHandlerUsefulLinksRetrieve::CmdHandlerUsefulLinksRetrieve()
    : CmdHandlerBase("useful_links_retrieve", "Method for retrieve useful link") {

    setActivatedFromVersion("0.2.28");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("useful_link_id", "Id of useful link");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksRetrieve::handle(ModelRequest *pRequest) {
    int nUsefulLinkId = pRequest->getInputInteger("useful_link_id", 0);
    int nUserId = 0;
    WsjcppUserSession *pSession = pRequest->getUserSession();
    if (pSession != nullptr) {
        nUserId = pSession->userid();
    }

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT t0.*, t1.userid FROM useful_links t0 LEFT JOIN useful_links_user_favorites t1 ON t1.usefullinkid = t0.id AND t1.userid = :userid WHERE t0.id = :useful_link_id");
    query.bindValue(":userid", nUserId);
    query.bindValue(":useful_link_id", nUsefulLinkId);

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    nlohmann::json jsonLink;
    if (query.next()) {
        QSqlRecord record = query.record();
        jsonLink["id"] = record.value("id").toInt();
        jsonLink["url"] = record.value("url").toString().toHtmlEscaped().toStdString();
        jsonLink["description"] = record.value("description").toString().toHtmlEscaped().toStdString();
        jsonLink["author"] = record.value("message").toString().toHtmlEscaped().toStdString();
        jsonLink["user_favorites"] = record.value("user_favorites").toInt();
        jsonLink["dt"] = record.value("dt").toString().toStdString();
        if (record.value("userid").toInt() == nUserId && nUserId != 0) {
            jsonLink["favorite"] = true;
        } else {
            jsonLink["favorite"] = false;
        }
    }
    
    nlohmann::json jsonResponse;
    jsonResponse["data"] = jsonLink;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}


/*********************************************
 * Useful Links Add
**********************************************/

REGISTRY_CMD(CmdHandlerUsefulLinksAdd)

CmdHandlerUsefulLinksAdd::CmdHandlerUsefulLinksAdd()
    : CmdHandlerBase("useful_links_add", "Method for add new useful link") {

    setActivatedFromVersion("0.2.21");

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    
    requireStringParam("url", "URL"); // TODO validator
    requireStringParam("description", "Description");
    requireStringParam("author", "Author");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksAdd::handle(ModelRequest *pRequest) {
    std::string sUrl = pRequest->getInputString("url", "");
    std::string sDescription = pRequest->getInputString("description", "");
    std::string sAuthor = pRequest->getInputString("author", "");

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    // TODO uuid
    query.prepare("INSERT INTO useful_links(url,description,author,stars,dt) VALUES(:url, :description, :author, 0, NOW())");
    query.bindValue(":url", QString::fromStdString(sUrl));
    query.bindValue(":description", QString::fromStdString(sDescription));
    query.bindValue(":author", QString::fromStdString(sAuthor));

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    nlohmann::json jsonResult;
    jsonResult["id"] = query.lastInsertId().toInt();

    nlohmann::json jsonResponse;
    jsonResponse["data"] = jsonResult;

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * Useful Links Delete
**********************************************/

REGISTRY_CMD(CmdHandlerUsefulLinksDelete)

CmdHandlerUsefulLinksDelete::CmdHandlerUsefulLinksDelete()
    : CmdHandlerBase("useful_links_delete", "Method for delete link by admin") {
    
    setActivatedFromVersion("0.2.21");

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("useful_link_id", "Id of useful link");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksDelete::handle(ModelRequest *pRequest) {

    int nUsefulLinkId = pRequest->getInputInteger("useful_link_id", 0);
    
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    // TODO uuid
    query.prepare("DELETE FROM useful_links WHERE id = :useful_link_id");
    query.bindValue(":useful_link_id", nUsefulLinkId);

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    query.prepare("DELETE FROM useful_links_comments WHERE usefullinkid = :useful_link_id");
    query.bindValue(":useful_link_id", nUsefulLinkId);
    
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    query.prepare("DELETE FROM useful_links_user_favorites WHERE usefullinkid = :useful_link_id");
    query.bindValue(":useful_link_id", nUsefulLinkId);
    
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    nlohmann::json jsonResponse;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*********************************************
 * Useful Links Update
**********************************************/

REGISTRY_CMD(CmdHandlerUsefulLinksUpdate)

CmdHandlerUsefulLinksUpdate::CmdHandlerUsefulLinksUpdate()
    : CmdHandlerBase("useful_links_update", "Method for update useful link") {
    
    setActivatedFromVersion("0.2.21");

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("useful_link_id", "Id of useful link");
    requireStringParam("url", "URL"); // TODO validator
    requireStringParam("description", "Description");
    requireStringParam("author", "Author");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksUpdate::handle(ModelRequest *pRequest) {
    
    int nUsefulLinkId = pRequest->getInputInteger("useful_link_id", 0);
    std::string sUrl = pRequest->getInputString("url", "");
    std::string sDescription = pRequest->getInputString("description", "");
    std::string sAuthor = pRequest->getInputString("author", "");

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    // TODO uuid
    query.prepare("UPDATE useful_links SET url = :url, description = :description WHERE id = :useful_link_id");
    query.bindValue(":url", QString::fromStdString(sUrl));
    query.bindValue(":description", QString::fromStdString(sDescription));
    query.bindValue(":useful_link_id", nUsefulLinkId);

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    nlohmann::json jsonResponse;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Useful Links list of favorite

REGISTRY_CMD(CmdHandlerUsefulLinksUserFavoriteList)

CmdHandlerUsefulLinksUserFavoriteList::CmdHandlerUsefulLinksUserFavoriteList()
    : CmdHandlerBase("useful_links_user_favorite_list", "Useful Links list of favorite") {
    
    setActivatedFromVersion("0.2.29");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksUserFavoriteList::handle(ModelRequest *pRequest) {
    nlohmann::json jsonRequest = pRequest->jsonRequest();
    int nUserId = 0;
    WsjcppUserSession *pSession = pRequest->getUserSession();
    if (pSession != nullptr) {
        nUserId = pSession->userid();
    }

    bool bIsAdmin = pRequest->isAdmin();

    std::string sFilter = pRequest->getInputString("filter", "");

    nlohmann::json jsonData = nlohmann::json::array();
    QString sWhere = "";
    if (!bIsAdmin) {
        // sWhere = " WHERE status = 'ok' ";
    }

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    query.prepare("SELECT t0.*, t1.userid FROM useful_links t0 INNER JOIN useful_links_user_favorites t1 ON t1.usefullinkid = t0.id AND t1.userid = :userid ORDER BY user_favorites DESC");
    query.bindValue(":userid", nUserId);

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        nlohmann::json jsonLink;
        jsonLink["id"] = record.value("id").toInt();
        jsonLink["url"] = record.value("url").toString().toHtmlEscaped().toStdString();
        jsonLink["description"] = record.value("description").toString().toHtmlEscaped().toStdString();
        jsonLink["author"] = record.value("message").toString().toHtmlEscaped().toStdString();
        jsonLink["user_favorites"] = record.value("user_favorites").toInt();
        jsonLink["dt"] = record.value("dt").toString().toStdString();
        if (record.value("userid").toInt() == nUserId && nUserId != 0) {
            jsonLink["favorite"] = true;
        } else {
            jsonLink["favorite"] = false;
        }
        jsonData.push_back(jsonLink);
    }
    
    nlohmann::json jsonResponse;
    jsonResponse["data"] = jsonData;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Useful Links add to favorite

REGISTRY_CMD(CmdHandlerUsefulLinksUserFavorite)

CmdHandlerUsefulLinksUserFavorite::CmdHandlerUsefulLinksUserFavorite()
    : CmdHandlerBase("useful_links_user_favorite", "Useful Links add to favorite") {
    
    setActivatedFromVersion("0.2.29");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("useful_link_id", "Id of useful link");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksUserFavorite::handle(ModelRequest *pRequest) {
    int nUsefulLinkId = pRequest->getInputInteger("useful_link_id", 0);
    int nUserId = 0;
    WsjcppUserSession *pSession = pRequest->getUserSession();
    if (pSession != nullptr) {
        nUserId = pSession->userid();
    }

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    query.prepare("SELECT * FROM useful_links_user_favorites WHERE usefullinkid = :usefullinkid AND userid = :userid");
    query.bindValue(":usefullinkid", nUsefulLinkId);
    query.bindValue(":userid", nUserId);

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    if (query.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(403, "Already exists"));
        return;
    }

    // TODO datetime
    // TODO check existing link
    // TODO check existing same row
    // TODO calculate count of favorites 
    query.prepare("INSERT INTO useful_links_user_favorites(usefullinkid,userid,dt) VALUES(:usefullinkid,:userid, NOW())");
    query.bindValue(":usefullinkid", nUsefulLinkId);
    query.bindValue(":userid", nUserId);

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    nlohmann::json jsonResult;
    jsonResult["id"] = query.lastInsertId().toInt();
    jsonResult["useful_link_id"] = nUsefulLinkId;

    nlohmann::json jsonResponse;
    jsonResponse["data"] = jsonResult;

    query.prepare("UPDATE `useful_links` SET user_favorites = (SELECT COUNT(*) FROM useful_links_user_favorites WHERE usefullinkid = :usefullinkid0) WHERE id = :usefullinkid1");
    query.bindValue(":usefullinkid0", nUsefulLinkId);
    query.bindValue(":usefullinkid1", nUsefulLinkId);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Useful Links removed from favorite

REGISTRY_CMD(CmdHandlerUsefulLinksUserUnfavorite)

CmdHandlerUsefulLinksUserUnfavorite::CmdHandlerUsefulLinksUserUnfavorite()
    : CmdHandlerBase("useful_links_user_unfavorite", "Useful Links removed from favorite") {
    
    setActivatedFromVersion("0.2.29");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("useful_link_id", "Id of useful link");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksUserUnfavorite::handle(ModelRequest *pRequest) {
    int nUsefulLinkId = pRequest->getInputInteger("useful_link_id", 0);
    int nUserId = 0;
    WsjcppUserSession *pSession = pRequest->getUserSession();
    if (pSession != nullptr) {
        nUserId = pSession->userid();
    }

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    // TODO datetime
    // TODO check existing link
    // TODO check existing same row
    query.prepare("DELETE FROM useful_links_user_favorites WHERE usefullinkid = :usefullinkid AND userid = :userid");
    query.bindValue(":usefullinkid", nUsefulLinkId);
    query.bindValue(":userid", nUserId);

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }
    nlohmann::json jsonResult;
    jsonResult["useful_link_id"] = nUsefulLinkId;

    nlohmann::json jsonResponse;
    jsonResponse["data"] = jsonResult;

    query.prepare("UPDATE `useful_links` SET user_favorites = (SELECT COUNT(*) FROM useful_links_user_favorites WHERE usefullinkid = :usefullinkid0) WHERE id = :usefullinkid1");
    query.bindValue(":usefullinkid0", nUsefulLinkId);
    query.bindValue(":usefullinkid1", nUsefulLinkId);
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Useful Links counter clicker

REGISTRY_CMD(CmdHandlerUsefulLinksClicked)

CmdHandlerUsefulLinksClicked::CmdHandlerUsefulLinksClicked()
    : CmdHandlerBase("useful_links_clicked", "Useful Links removed from favorite") {
    
    setActivatedFromVersion("0.2.29");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("useful_link_id", "Id of useful link");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksClicked::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------
// Useful Links list of comments

REGISTRY_CMD(CmdHandlerUsefulLinksCommentList)

CmdHandlerUsefulLinksCommentList::CmdHandlerUsefulLinksCommentList()
    : CmdHandlerBase("useful_links_comment_list", "Useful Links list of comments") {
    
    setActivatedFromVersion("0.2.29");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("useful_link_id", "Id of useful link");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksCommentList::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------
// Useful Links add comment

CmdHandlerUsefulLinksCommentAdd::CmdHandlerUsefulLinksCommentAdd()
    : CmdHandlerBase("useful_links_comment_add", "Useful Links add comment") {
    
    setActivatedFromVersion("0.2.29");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("useful_link_id", "Id of useful link");
    requireStringParam("comment", "Comment");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksCommentAdd::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------
// Useful Links remove comment

CmdHandlerUsefulLinksCommentDelete::CmdHandlerUsefulLinksCommentDelete()
    : CmdHandlerBase("useful_links_comment_delete", "Useful Links remove comment") {
    
    setActivatedFromVersion("0.2.29");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
    requireIntegerParam("useful_link_comment_id", "Comment Id for useful link");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksCommentDelete::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}


// ---------------------------------------------------------------------
// Useful Links List of tags

CmdHandlerUsefulLinksTagList::CmdHandlerUsefulLinksTagList()
    : CmdHandlerBase("useful_links_tag_list", "Useful Links - List of tags") {
    
    setActivatedFromVersion("0.2.29");

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksTagList::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------
// Useful Links add tag

CmdHandlerUsefulLinksTagAdd::CmdHandlerUsefulLinksTagAdd()
    : CmdHandlerBase("useful_links_tag_add", "Useful Links add tag") {
    
    setActivatedFromVersion("0.2.29");

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireIntegerParam("useful_link_id", "Id of useful link");
    requireStringParam("tagname", "tag name");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksTagAdd::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------
// Useful Links remove tag

CmdHandlerUsefulLinksTagDelete::CmdHandlerUsefulLinksTagDelete()
    : CmdHandlerBase("useful_links_tag_delete", "Useful Links remove tag") {
    
    setActivatedFromVersion("0.2.29");

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireIntegerParam("useful_link_tag_id", "Tag Id for useful link");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksTagDelete::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------
// Useful Links propose link by user

CmdHandlerUsefulLinksUserPropose::CmdHandlerUsefulLinksUserPropose()
    : CmdHandlerBase("useful_links_user_propose", "Useful Links propose link by user") {
    
    setActivatedFromVersion("0.2.29");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    requireStringParam("url", "Url");
    requireStringParam("description", "Description");
    requireStringParam("tags", "tags");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksUserPropose::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------
// Useful Links propose link by user

CmdHandlerUsefulLinksUserProposeApprove::CmdHandlerUsefulLinksUserProposeApprove()
    : CmdHandlerBase("useful_links_user_propose_approve", "Useful Links Approve propose link by admin") {
    
    setActivatedFromVersion("0.2.29");

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    requireIntegerParam("useful_links_propose_id", "Id of propose for useful_links");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksUserProposeApprove::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}