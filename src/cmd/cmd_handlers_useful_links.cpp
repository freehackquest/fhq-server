#include "cmd_handlers_useful_links.h"
#include <employ_database.h>
#include <employ_notify.h>
#include <employ_server_info.h>
#include <md5.h>
#include <runtasks.h>

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
  optionalStringParam("filter_by_tag", "Filter by tag");
  optionalIntegerParam("page_index", "Page Index");
  optionalIntegerParam("page_size", "Page Size (default 10)");
}

struct QueryFilter {
  std::string sColumnName;
  std::string sFilterName;
  std::string sValue;
};

struct QueryFilters {
  void like(std::vector<std::string> vFields, std::string sValue) {
    std::string sWhere = "";
    for (int i = 0; i < vFields.size(); i++) {
      QueryFilter f;
      f.sColumnName = vFields[i];
      f.sFilterName = ":" + vFields[i];
      f.sValue = "%" + sValue + "%";
      vFilters.push_back(f);
      if (sWhere == "") {
        sWhere = "(";
      } else {
        sWhere += " OR ";
      }
      sWhere += f.sColumnName + " LIKE " + f.sFilterName;
    }
    sWhere += ")";
    vCompiledWhere.push_back(sWhere);
  };

  void equal(std::string sField, std::string sValue) {
    std::string sWhere = "(";
    QueryFilter f;
    f.sColumnName = sField;
    f.sFilterName = ":" + sField;
    f.sValue = sValue;
    vFilters.push_back(f);
    sWhere += f.sColumnName + " = " + f.sFilterName;
    sWhere += ")";
    vCompiledWhere.push_back(sWhere);
  };

  std::string compileWhere() {
    if (vCompiledWhere.size() == 0) {
      return "";
    }
    std::string sWhere = " WHERE ";
    for (int i = 0; i < vCompiledWhere.size(); i++) {
      sWhere += (i > 0 ? " AND " : "") + vCompiledWhere[i];
    }
    return sWhere;
  };

  void bind(QSqlQuery &query) {
    for (int i = 0; i < vFilters.size(); i++) {
      query.bindValue(QString::fromStdString(vFilters[i].sFilterName), QString::fromStdString(vFilters[i].sValue));
    }
  };
  std::vector<QueryFilter> vFilters;
  std::vector<std::string> vCompiledWhere;
};

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
  std::string sFilterByTag = pRequest->getInputString("filter_by_tag", "");
  int nPageIndex = pRequest->getInputInteger("page_index", 0);
  int nPageSize = pRequest->getInputInteger("page_size", 10);
  int nLength = 0;
  nlohmann::json jsonData;
  QueryFilters queryFilters;
  sFilter = WsjcppCore::trim(sFilter);
  if (sFilter != "") {
    queryFilters.like({"url", "description"}, sFilter);
  }

  std::string sInnerJoinTags = "";
  if (sFilterByTag != "") {
    sInnerJoinTags = " INNER JOIN useful_links_tags t2 ON t0.id = t2.usefullinkid ";
    queryFilters.equal("tagvalue", sFilterByTag);
  }
  QString sWhere = "";

  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);

  // count
  query.prepare("SELECT COUNT(*) AS cnt FROM useful_links t0 "
                " LEFT JOIN useful_links_user_favorites t1 ON t1.usefullinkid "
                "= t0.id AND t1.userid = :userid " +
                QString::fromStdString(sInnerJoinTags) + QString::fromStdString(queryFilters.compileWhere()));
  query.bindValue(":userid", nUserId);
  queryFilters.bind(query);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }
  if (query.next()) {
    QSqlRecord record = query.record();
    nlohmann::json jsonLink;
    jsonData["total"] = record.value("cnt").toInt();
  }

  jsonData["page_index"] = nPageIndex;
  jsonData["page_size"] = nPageSize;

  query.prepare("SELECT t0.*, t1.userid FROM useful_links t0 "
                " LEFT JOIN useful_links_user_favorites t1 ON t1.usefullinkid "
                "= t0.id AND t1.userid = :userid " +
                QString::fromStdString(sInnerJoinTags) + QString::fromStdString(queryFilters.compileWhere()) +
                " ORDER BY user_favorites DESC, user_clicks DESC, dt DESC "
                " LIMIT " +
                QString::number(nPageIndex * nPageSize) + "," + QString::number(nPageSize));
  query.bindValue(":userid", nUserId);
  queryFilters.bind(query);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  nlohmann::json jsonItems = nlohmann::json::array();
  while (query.next()) {
    QSqlRecord record = query.record();
    nlohmann::json jsonLink;
    jsonLink["id"] = record.value("id").toInt();
    jsonLink["url"] = record.value("url").toString().toHtmlEscaped().toStdString();
    jsonLink["description"] = record.value("description").toString().toHtmlEscaped().toStdString();
    jsonLink["author"] = record.value("message").toString().toHtmlEscaped().toStdString();
    jsonLink["user_favorites"] = record.value("user_favorites").toInt();
    jsonLink["user_comments"] = record.value("user_comments").toInt();
    jsonLink["user_clicks"] = record.value("user_clicks").toInt();
    jsonLink["dt"] = record.value("dt").toString().toStdString();
    if (record.value("userid").toInt() == nUserId && nUserId != 0) {
      jsonLink["favorite"] = true;
    } else {
      jsonLink["favorite"] = false;
    }
    std::string sTags = record.value("tags").toString().toStdString();
    jsonLink["tags"] = WsjcppCore::split(sTags, ",");
    jsonItems.push_back(jsonLink);
  }
  jsonData["items"] = jsonItems;

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

  setAccessUnauthorized(true);
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
  query.prepare("SELECT t0.*, t1.userid FROM useful_links t0 "
                " LEFT JOIN useful_links_user_favorites t1 ON t1.usefullinkid "
                "= t0.id AND t1.userid = :userid "
                " WHERE t0.id = :useful_link_id");
  query.bindValue(":userid", nUserId);
  query.bindValue(":useful_link_id", nUsefulLinkId);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
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
    jsonLink["user_comments"] = record.value("user_comments").toInt();
    jsonLink["user_clicks"] = record.value("user_clicks").toInt();
    jsonLink["dt"] = record.value("dt").toString().toStdString();
    if (record.value("userid").toInt() == nUserId && nUserId != 0) {
      jsonLink["favorite"] = true;
    } else {
      jsonLink["favorite"] = false;
    }
  }

  // tags
  query.prepare("SELECT * FROM useful_links_tags WHERE usefullinkid = :useful_link_id");
  query.bindValue(":useful_link_id", nUsefulLinkId);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  nlohmann::json jsonLinkTags = nlohmann::json::array();
  while (query.next()) {
    QSqlRecord record = query.record();
    std::string sTag = record.value("tagvalue").toString().toHtmlEscaped().toStdString();
    jsonLinkTags.push_back(sTag);
  }
  jsonLink["tags"] = jsonLinkTags;

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
  query.prepare("INSERT INTO useful_links(url,description,author,tags,dt) "
                "VALUES(:url, :description, :author, :tags, NOW())");
  query.bindValue(":url", QString::fromStdString(sUrl));
  query.bindValue(":description", QString::fromStdString(sDescription));
  query.bindValue(":author", QString::fromStdString(sAuthor));
  query.bindValue(":tags", QString::fromStdString(""));

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  int nRowId = query.lastInsertId().toInt();

  nlohmann::json jsonResult;
  jsonResult["id"] = nRowId;

  nlohmann::json jsonResponse;
  jsonResponse["data"] = jsonResult;

  EmployNotify *pNotify = findWsjcppEmploy<EmployNotify>();
  nlohmann::json jsonMeta;
  jsonMeta["useful_link"] = nlohmann::json();
  jsonMeta["useful_link"]["id"] = nRowId;
  jsonMeta["useful_link"]["url"] = sUrl;
  jsonMeta["useful_link"]["action"] = "added";
  pNotify->notifyInfo("useful_links", "Added [useful_link#" + std::to_string(nRowId) + "] " + sUrl + "", jsonMeta);

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
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  query.prepare("DELETE FROM useful_links_comments WHERE usefullinkid = :useful_link_id");
  query.bindValue(":useful_link_id", nUsefulLinkId);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  query.prepare("DELETE FROM useful_links_user_favorites WHERE usefullinkid = "
                ":useful_link_id");
  query.bindValue(":useful_link_id", nUsefulLinkId);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  query.prepare("DELETE FROM useful_links_tags WHERE usefullinkid = :useful_link_id");
  query.bindValue(":useful_link_id", nUsefulLinkId);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  EmployNotify *pNotify = findWsjcppEmploy<EmployNotify>();
  nlohmann::json jsonMeta;
  jsonMeta["useful_link"] = nlohmann::json();
  jsonMeta["useful_link"]["id"] = nUsefulLinkId;
  jsonMeta["useful_link"]["action"] = "removed";
  pNotify->notifyInfo("useful_links", "Removed [useful_link#" + std::to_string(nUsefulLinkId) + "]", jsonMeta);

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
  query.prepare("UPDATE useful_links SET url = :url, description = "
                ":description WHERE id = :useful_link_id");
  query.bindValue(":url", QString::fromStdString(sUrl));
  query.bindValue(":description", QString::fromStdString(sDescription));
  query.bindValue(":useful_link_id", nUsefulLinkId);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  EmployNotify *pNotify = findWsjcppEmploy<EmployNotify>();
  nlohmann::json jsonMeta;
  jsonMeta["useful_link"] = nlohmann::json();
  jsonMeta["useful_link"]["id"] = nUsefulLinkId;
  jsonMeta["useful_link"]["url"] = sUrl;
  jsonMeta["useful_link"]["action"] = "updated";
  pNotify->notifyInfo("useful_links", "Updated [useful_link#" + std::to_string(nUsefulLinkId) + "] " + sUrl + "",
                      jsonMeta);

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

  query.prepare("SELECT t0.*, t1.userid FROM useful_links t0 INNER JOIN "
                "useful_links_user_favorites t1 ON t1.usefullinkid = t0.id AND "
                "t1.userid = :userid ORDER BY user_favorites DESC");
  query.bindValue(":userid", nUserId);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
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
    jsonLink["user_clicks"] = record.value("user_clicks").toInt();
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

  query.prepare("SELECT * FROM useful_links_user_favorites WHERE usefullinkid "
                "= :usefullinkid AND userid = :userid");
  query.bindValue(":usefullinkid", nUsefulLinkId);
  query.bindValue(":userid", nUserId);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  if (query.next()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(403, "Already exists"));
    return;
  }

  // TODO datetime
  // TODO check existing link
  // TODO check existing same row
  // TODO calculate count of favorites
  query.prepare("INSERT INTO useful_links_user_favorites(usefullinkid,userid,dt) "
                "VALUES(:usefullinkid,:userid, NOW())");
  query.bindValue(":usefullinkid", nUsefulLinkId);
  query.bindValue(":userid", nUserId);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }
  nlohmann::json jsonResult;
  jsonResult["id"] = query.lastInsertId().toInt();
  jsonResult["useful_link_id"] = nUsefulLinkId;

  nlohmann::json jsonResponse;
  jsonResponse["data"] = jsonResult;

  query.prepare("UPDATE `useful_links` SET user_favorites = (SELECT COUNT(*) "
                "FROM useful_links_user_favorites WHERE usefullinkid = "
                ":usefullinkid0) WHERE id = :usefullinkid1");
  query.bindValue(":usefullinkid0", nUsefulLinkId);
  query.bindValue(":usefullinkid1", nUsefulLinkId);
  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
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
  query.prepare("DELETE FROM useful_links_user_favorites WHERE usefullinkid = "
                ":usefullinkid AND userid = :userid");
  query.bindValue(":usefullinkid", nUsefulLinkId);
  query.bindValue(":userid", nUserId);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }
  nlohmann::json jsonResult;
  jsonResult["useful_link_id"] = nUsefulLinkId;

  nlohmann::json jsonResponse;
  jsonResponse["data"] = jsonResult;

  query.prepare("UPDATE `useful_links` SET user_favorites = (SELECT COUNT(*) "
                "FROM useful_links_user_favorites WHERE usefullinkid = "
                ":usefullinkid0) WHERE id = :usefullinkid1");
  query.bindValue(":usefullinkid0", nUsefulLinkId);
  query.bindValue(":usefullinkid1", nUsefulLinkId);
  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
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

  setAccessUnauthorized(true);
  setAccessUser(true);
  setAccessAdmin(true);

  // validation and description input fields
  requireIntegerParam("useful_link_id", "Id of useful link");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksClicked::handle(ModelRequest *pRequest) {
  int nUsefulLinkId = pRequest->getInputInteger("useful_link_id", 0);
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);

  query.prepare("UPDATE useful_links SET user_clicks = user_clicks + 1 WHERE "
                "id = :useful_link_id");
  query.bindValue(":useful_link_id", nUsefulLinkId);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  nlohmann::json jsonData;
  jsonData["useful_link_id"] = nUsefulLinkId;

  nlohmann::json jsonResponse;
  jsonResponse["data"] = jsonData;
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Useful Links list of comments

REGISTRY_CMD(CmdHandlerUsefulLinksCommentList)

CmdHandlerUsefulLinksCommentList::CmdHandlerUsefulLinksCommentList()
    : CmdHandlerBase("useful_links_comment_list", "Useful Links list of comments") {

  setActivatedFromVersion("0.2.29");

  setAccessUnauthorized(true);
  setAccessUser(true);
  setAccessAdmin(true);

  // validation and description input fields
  requireIntegerParam("useful_link_id", "Id of useful link");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksCommentList::handle(ModelRequest *pRequest) {

  int nUsefulLinkId = pRequest->getInputInteger("useful_link_id", 0);

  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);

  query.prepare(""
                " SELECT ulc.*, u.nick, u.role, u.logo, u.university "
                " FROM useful_links_comments ulc "
                " INNER JOIN users u ON u.id = ulc.userid "
                " WHERE usefullinkid = :usefullinkid "
                " ORDER BY dt DESC");
  query.bindValue(":usefullinkid", nUsefulLinkId);

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  nlohmann::json jsonData = nlohmann::json::array();
  while (query.next()) {
    QSqlRecord record = query.record();
    nlohmann::json jsonComment;
    jsonComment["id"] = record.value("id").toInt();
    jsonComment["comment"] = record.value("comment").toString().toHtmlEscaped().toStdString();
    jsonComment["dt"] = record.value("dt").toString().toStdString();
    nlohmann::json jsonCommentUser;
    jsonCommentUser["role"] = record.value("role").toString().toStdString();
    jsonCommentUser["nick"] = record.value("nick").toString().toHtmlEscaped().toStdString();
    jsonCommentUser["logo"] = record.value("logo").toString().toStdString();
    jsonCommentUser["university"] = record.value("university").toString().toHtmlEscaped().toStdString();
    jsonComment["user"] = jsonCommentUser;

    jsonData.push_back(jsonComment);
  }
  nlohmann::json jsonResponse;
  jsonResponse["data"] = jsonData;
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Useful Links add comment

REGISTRY_CMD(CmdHandlerUsefulLinksCommentAdd)

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
  int nUsefulLinkId = pRequest->getInputInteger("useful_link_id", 0);
  std::string sCommentValue = pRequest->getInputString("comment", "");
  int nUserId = pRequest->getUserSession()->userid();
  sCommentValue = WsjcppCore::trim(sCommentValue);
  if (sCommentValue == "") {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(400, "comment could not be empty"));
    return;
  }

  if (sCommentValue.length() > 1024) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(400, "comment must be less then < 1024"));
    return;
  }

  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  // TODO redesign dt to long

  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);
  query.prepare("INSERT INTO useful_links_comments(usefullinkid, userid, comment, dt) "
                "VALUES(:useful_link_id, :userid, :comment, NOW())");
  query.bindValue(":useful_link_id", nUsefulLinkId);
  query.bindValue(":userid", nUserId);
  query.bindValue(":comment", QString::fromStdString(sCommentValue));
  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  query.prepare("UPDATE useful_links ul SET user_comments = (SELECT COUNT(*) "
                "FROM useful_links_comments WHERE usefullinkid = ul.id) WHERE "
                "ul.id = :useful_link_id");
  query.bindValue(":useful_link_id", nUsefulLinkId);
  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  EmployNotify *pNotify = findWsjcppEmploy<EmployNotify>();
  nlohmann::json jsonMeta;
  jsonMeta["useful_link"] = nlohmann::json();
  jsonMeta["useful_link"]["id"] = nUsefulLinkId;
  jsonMeta["useful_link"]["url"] = "link"; // TODO add url
  jsonMeta["useful_link"]["action"] = "comment_added";
  jsonMeta["user"] = nlohmann::json();
  jsonMeta["user"]["id"] = nUserId;
  jsonMeta["user"]["nick"] = pRequest->getUserSession()->nick().toStdString();

  std::string sMessage = "User added comment to [useful_link#" + std::to_string(nUsefulLinkId) + "]";

  pNotify->notifyInfo("useful_links", sMessage, jsonMeta);

  // TODO add url
  std::string sBody = "[user#" + std::to_string(nUserId) + "] " + pRequest->getUserSession()->nick().toStdString() +
                      " added comment to "
                      "[useful_link#" +
                      std::to_string(nUsefulLinkId) + "] : \n " + sCommentValue;

  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  std::string sMailToAdmin = pGlobalSettings->get("mail_system_message_admin_email").getStringValue();

  RunTasks::MailSend(sMailToAdmin, sMessage, sBody);

  nlohmann::json jsonResponse;
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Useful Links remove comment

REGISTRY_CMD(CmdHandlerUsefulLinksCommentDelete)

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

  int nUsefulLinkCommentId = pRequest->getInputInteger("useful_link_comment_id", 0);

  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  // TODO redesign database like Json =
  // findInTable("useful_links_comments").where("id", nUsefulLinkCommentId);

  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);

  query.prepare("SELECT * FROM useful_links_comments WHERE id = :comment_id");
  query.bindValue(":comment_id", nUsefulLinkCommentId);
  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  int nUsefulLinkId = 0;
  if (query.next()) {
    QSqlRecord record = query.record();
    nUsefulLinkId = record.value("usefullinkid").toInt();
  } else {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(404, "COMMENT_NOT_FOUND"));
    return;
  }

  query.prepare("DELETE FROM useful_links_comments WHERE id = :comment_id");
  query.bindValue(":comment_id", nUsefulLinkCommentId);
  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  // update count of comments
  query.prepare("UPDATE useful_links ul SET user_comments = (SELECT COUNT(*) "
                "FROM useful_links_comments WHERE usefullinkid = ul.id) WHERE "
                "ul.id = :useful_link_id");
  query.bindValue(":useful_link_id", nUsefulLinkId);
  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  // TODO add notification about removed comments

  nlohmann::json jsonResponse;
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Useful Links List of tags

REGISTRY_CMD(CmdHandlerUsefulLinksTagList)

CmdHandlerUsefulLinksTagList::CmdHandlerUsefulLinksTagList()
    : CmdHandlerBase("useful_links_tag_list", "Useful Links - List of tags") {

  setActivatedFromVersion("0.2.29");

  setAccessUnauthorized(true);
  setAccessUser(true);
  setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksTagList::handle(ModelRequest *pRequest) {

  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);

  // TODO get from employ
  query.prepare("SELECT tagvalue, COUNT(*) AS cnt FROM `useful_links_tags` "
                "GROUP BY tagvalue");

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  nlohmann::json jsonData = nlohmann::json::array();
  while (query.next()) {
    QSqlRecord record = query.record();
    nlohmann::json jsonTagInfo;
    jsonTagInfo["tag"] = record.value("tagvalue").toString().toHtmlEscaped().toStdString();
    jsonTagInfo["counter"] = record.value("cnt").toInt();
    jsonData.push_back(jsonTagInfo);
  }
  nlohmann::json jsonResponse;
  jsonResponse["data"] = jsonData;
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Useful Links add tag

REGISTRY_CMD(CmdHandlerUsefulLinksTagAdd)

CmdHandlerUsefulLinksTagAdd::CmdHandlerUsefulLinksTagAdd()
    : CmdHandlerBase("useful_links_tag_add", "Useful Links add tag") {

  setActivatedFromVersion("0.2.29");

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  requireIntegerParam("useful_link_id", "Id of useful link");
  requireStringParam("tag", "Tag Value");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksTagAdd::handle(ModelRequest *pRequest) {
  int nUsefulLinkId = pRequest->getInputInteger("useful_link_id", 0);
  std::string sTagValue = pRequest->getInputString("tag", "");
  sTagValue = WsjcppCore::trim(sTagValue);
  if (sTagValue == "") {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(400, "tag could not be empty"));
    return;
  }

  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);

  // TODO get from employ
  query.prepare("SELECT * FROM useful_links_tags WHERE usefullinkid = "
                ":useful_link_id AND tagvalue = :tagvalue");
  query.bindValue(":useful_link_id", nUsefulLinkId);
  query.bindValue(":tagvalue", QString::fromStdString(sTagValue));

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  if (query.next()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(400, "TAG_ALREADY_EXISTS"));
    return;
  } else {
    query.prepare("INSERT INTO useful_links_tags(usefullinkid, tagvalue) "
                  "VALUES(:useful_link_id, :tagvalue)");
    query.bindValue(":useful_link_id", nUsefulLinkId);
    query.bindValue(":tagvalue", QString::fromStdString(sTagValue));
    if (!query.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
      return;
    }

    // update field useful_links.tags
    query.prepare("SELECT * FROM useful_links_tags WHERE usefullinkid = :useful_link_id");
    query.bindValue(":useful_link_id", nUsefulLinkId);
    if (!query.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
      return;
    }

    std::string sTags = "";
    while (query.next()) {
      QSqlRecord record = query.record();
      std::string sTag = record.value("tagvalue").toString().toHtmlEscaped().toStdString();
      sTags += sTags.length() == 0 ? "" : ",";
      sTags += sTag;
    }

    query.prepare("UPDATE useful_links SET tags = :tags WHERE id = :useful_link_id");
    query.bindValue(":tags", QString::fromStdString(sTags));
    query.bindValue(":useful_link_id", nUsefulLinkId);
    if (!query.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
      return;
    }
  }
  nlohmann::json jsonResponse;
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Useful Links remove tag

REGISTRY_CMD(CmdHandlerUsefulLinksTagDelete)

CmdHandlerUsefulLinksTagDelete::CmdHandlerUsefulLinksTagDelete()
    : CmdHandlerBase("useful_links_tag_delete", "Useful Links remove tag") {

  setActivatedFromVersion("0.2.29");

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  requireIntegerParam("useful_link_id", "Useful Link Id");
  requireStringParam("tag", "Tag Value");
}

// ---------------------------------------------------------------------

void CmdHandlerUsefulLinksTagDelete::handle(ModelRequest *pRequest) {
  int nUsefulLinkId = pRequest->getInputInteger("useful_link_id", 0);
  std::string sTagValue = pRequest->getInputString("tag", "");
  sTagValue = WsjcppCore::trim(sTagValue);
  if (sTagValue == "") {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(400, "tag could not be empty"));
    return;
  }

  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();

  QSqlDatabase db = *(pDatabase->database());
  QSqlQuery query(db);

  // TODO get from employ
  query.prepare("SELECT * FROM useful_links_tags WHERE usefullinkid = "
                ":useful_link_id AND tagvalue = :tagvalue");
  query.bindValue(":useful_link_id", nUsefulLinkId);
  query.bindValue(":tagvalue", QString::fromStdString(sTagValue));

  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  if (query.next()) {
    query.prepare("DELETE FROM useful_links_tags WHERE usefullinkid = "
                  ":useful_link_id AND tagvalue = :tagvalue");
    query.bindValue(":useful_link_id", nUsefulLinkId);
    query.bindValue(":tagvalue", QString::fromStdString(sTagValue));
    if (!query.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
      return;
    }

    // update field useful_links.tags
    query.prepare("SELECT * FROM useful_links_tags WHERE usefullinkid = :useful_link_id");
    query.bindValue(":useful_link_id", nUsefulLinkId);
    if (!query.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
      return;
    }

    std::string sTags = "";
    while (query.next()) {
      QSqlRecord record = query.record();
      std::string sTag = record.value("tagvalue").toString().toHtmlEscaped().toStdString();
      sTags += sTags.length() == 0 ? "" : ",";
      sTags += sTag;
    }

    query.prepare("UPDATE useful_links SET tags = :tags WHERE id = :useful_link_id");
    query.bindValue(":tags", QString::fromStdString(sTags));
    query.bindValue(":useful_link_id", nUsefulLinkId);
    if (!query.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
      return;
    }
  }
  nlohmann::json jsonResponse;
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// Useful Links propose link by user

REGISTRY_CMD(CmdHandlerUsefulLinksUserPropose)

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
  pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------
// Useful Links propose link by user

REGISTRY_CMD(CmdHandlerUsefulLinksUserProposeApprove)

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
  pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(501, "Not Implemented Yet"));
}