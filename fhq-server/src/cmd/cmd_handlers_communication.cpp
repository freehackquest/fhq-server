#include "cmd_handlers_communication.h"
#include <runtasks.h>
#include <fallen.h>
#include <md5.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <employ_notify.h>

/*********************************************
 * Users chat
**********************************************/

REGISTRY_CMD(CmdHandlerChatSendMessage)

CmdHandlerChatSendMessage::CmdHandlerChatSendMessage()
    : CmdHandlerBase("chat_send_message", "Method will be send chat message and it sent to another users") {

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    requireStringParam("type", "Type"); // TODO validator chat type
    requireStringParam("message", "Message");
}

// ---------------------------------------------------------------------

void CmdHandlerChatSendMessage::handle(ModelRequest *pRequest) {
    nlohmann::json jsonRequest = pRequest->jsonRequest();
    nlohmann::json jsonResponse;

    WsjcppUserSession *pUserSession = pRequest->getUserSession();
    std::string sUsername = pUserSession->nick().toStdString();

    std::string sMessage = "";
    if (jsonRequest["message"].is_string()) {
        sMessage = jsonRequest["message"];
    }
    WsjcppCore::trim(sMessage);
    if (sMessage.length() == 0) {
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Message could not be empty"));
        return;
    }

    std::string sType = "";
    if (jsonRequest["type"].is_string()) {
        sType = jsonRequest["type"];
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("INSERT INTO chatmessages(user, message, dt) VALUES(:user,:message, NOW())");
    query.bindValue(":user", QString::fromStdString(sUsername));
    query.bindValue(":message", QString::fromStdString(sMessage));
    query.exec();

    nlohmann::json jsonData2;
    jsonData2["cmd"] = "chat";
    jsonData2["type"] = sType;
    jsonData2["user"] = sUsername;
    jsonData2["message"] = QString::fromStdString(sMessage).toHtmlEscaped().toStdString();
    jsonData2["dt"] = QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss").toStdString();

    pRequest->server()->sendToAll(jsonData2);

    EmployNotify *pNotify = findWsjcppEmploy<EmployNotify>();
    ModelNotification notification("info", "chat", "Income chat message " + sMessage);
    pNotify->sendNotification(notification);

    // TODO show notifications to all
}

/*********************************************
 * chat_latest_messages
**********************************************/

REGISTRY_CMD(CmdHandlerChatLastestMessages)

CmdHandlerChatLastestMessages::CmdHandlerChatLastestMessages()
    : CmdHandlerBase("chat_latest_messages", "Method will be send chat message and it sent to another users") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void CmdHandlerChatLastestMessages::handle(ModelRequest *pRequest) {
    nlohmann::json jsonMessages = nlohmann::json::array();

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT user, message, dt FROM chatmessages ORDER BY dt DESC LIMIT 0,25");

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        nlohmann::json jsonChatRow;
        jsonChatRow["user"] = record.value("user").toString().toHtmlEscaped().toStdString();
        jsonChatRow["type"] = "chat";
        jsonChatRow["message"] = record.value("message").toString().toHtmlEscaped().toStdString();
        jsonChatRow["dt"] = record.value("dt").toString().toStdString();
        jsonMessages.push_back(jsonChatRow);
    }
    
    nlohmann::json jsonResponse;
    jsonResponse["data"] = jsonMessages;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------

REGISTRY_CMD(CmdHandlerChatSendMessage_new)

CmdHandlerChatSendMessage_new::CmdHandlerChatSendMessage_new()
    : CmdHandlerBase("chats_message_send", "Method will be send chat message and it sent to another users") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    requireStringParam("chat", "Chat for sending a message");
    requireStringParam("message", "Message");
}

// ---------------------------------------------------------------------

void CmdHandlerChatSendMessage_new::handle(ModelRequest *pRequest) {
    
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    WsjcppUserSession *pUserSession = pRequest->getUserSession();
    int nUserId = pUserSession->userid();

    std::string sMessage = "";
    if (jsonRequest["message"].is_string()) {
        sMessage = jsonRequest["message"];
    }

    sMessage = WsjcppCore::trim(sMessage);
    if (sMessage.length() == 0) {
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Message could not be empty"));
        return;
    }

    std::string sChat = "";
    if (jsonRequest["chat"].is_string()) {
        sChat = jsonRequest["chat"];
    }

    if (sChat != "0") {
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Only '0' (global) chat allowed"));
        return;
    }

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    query.prepare("INSERT INTO chats_messages(chatid, userid, message, dt, status) VALUES(:chat, :user, :message, NOW(), 'unread')");
    query.bindValue(":chat", QString::fromStdString(sChat));
    query.bindValue(":user", nUserId);
    query.bindValue(":message", QString::fromStdString(sMessage));
    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    nlohmann::json jsonResponse;

    QSqlQuery query2(db);
    query2.prepare("SELECT status, dt, id FROM chats_messages WHERE id = :msg_id");

    query2.bindValue(":msg_id", query.lastInsertId().toInt());
    
    if (!query2.exec() || !query2.next()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    QSqlRecord record = query2.record();

    jsonResponse["user_id"] = nUserId;
    jsonResponse["message"] = sMessage;
    jsonResponse["dt"] = record.value("dt").toString().toStdString();
    jsonResponse["message_id"] = record.value("id").toInt();
    jsonResponse["status"] = record.value("status").toString().toStdString();

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------

REGISTRY_CMD(CmdHandlerChatReadMessage)

CmdHandlerChatReadMessage::CmdHandlerChatReadMessage()
    : CmdHandlerBase("chats_message_read", "Get all messages in chat") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    requireStringParam("chat", "Chat for reading messages");
}

// ---------------------------------------------------------------------

void CmdHandlerChatReadMessage::handle(ModelRequest *pRequest) {
    nlohmann::json jsonRequest = pRequest->jsonRequest();
    
    std::string sChat = "";
    if (jsonRequest["chat"].is_string()) {
        sChat = jsonRequest["chat"];
    }

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT id, userid, message, dt, status FROM chats_messages WHERE chatid = :chat ORDER BY dt");
    query.bindValue(":chat", QString::fromStdString(sChat));

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    nlohmann::json jsonMessages = nlohmann::json::array();
    while (query.next()) {
        QSqlRecord record = query.record();
        nlohmann::json jsonChatRow;
        jsonChatRow["user"] = record.value("userid").toInt();
        jsonChatRow["message_id"] = record.value("id").toInt();
        jsonChatRow["message"] = record.value("message").toString().toHtmlEscaped().toStdString();
        jsonChatRow["dt"] = record.value("dt").toString().toStdString();
        jsonMessages.push_back(jsonChatRow);
    }
    
    nlohmann::json jsonResponse;
    jsonResponse["messages"] = jsonMessages;
    jsonResponse["chat"] = sChat;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------

REGISTRY_CMD(CmdHandlerChatShowDialogs)

CmdHandlerChatShowDialogs::CmdHandlerChatShowDialogs()
    : CmdHandlerBase("chats_dialogs_list", "Show list of dialogs") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerChatShowDialogs::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------

REGISTRY_CMD(CmdHandlerChatEditMessage)

CmdHandlerChatEditMessage::CmdHandlerChatEditMessage()
    : CmdHandlerBase("chats_message_edit", "Edit message") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    requireIntegerParam("message_id", "Message id");
    requireStringParam("message_new", "New message");
}

// ---------------------------------------------------------------------

void CmdHandlerChatEditMessage::handle(ModelRequest *pRequest) {
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    WsjcppUserSession *pUserSession = pRequest->getUserSession();
    int nUserId = pUserSession->userid();

    std::string sNewMessage = "";
    if (jsonRequest["message_new"].is_string()) {
        sNewMessage = jsonRequest["message_new"];
    }

    sNewMessage = WsjcppCore::trim(sNewMessage);
    if (sNewMessage.length() == 0) {
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Message could not be empty"));
        return;
    }

    int nIdMessage = 0;
    if (jsonRequest["message_id"].is_number_integer()) {
        nIdMessage = jsonRequest["message_id"];
    }

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    query.prepare("SELECT id, userid FROM chats_messages WHERE id = :msg_id");
    query.bindValue(":msg_id", nIdMessage);


    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    if (!query.next() || query.size() < 1) {
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Message not found"));
        return;
    }

    QSqlRecord record = query.record();
    int nMsgUserId = record.value("userid").toInt();

    if (nUserId != nMsgUserId) {
        pRequest->sendMessageError(cmd(), WsjcppError(403, "Editing message allowed only for owner"));
        return;
    }

    QSqlQuery query2(db);
    query2.prepare("UPDATE chats_messages SET message = :message_new WHERE id = :msg_id");
    query2.bindValue(":msg_id", nIdMessage);
    query2.bindValue(":message_new", QString::fromStdString(sNewMessage));

    if (!query2.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    nlohmann::json jsonResponse;

    jsonResponse["user_id"] = nUserId;
    jsonResponse["message_id"] = nIdMessage;
    jsonResponse["message"] = sNewMessage;

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------

REGISTRY_CMD(CmdHandlerChatDeleteMessage)

CmdHandlerChatDeleteMessage::CmdHandlerChatDeleteMessage()
    : CmdHandlerBase("chats_message_delete", "Delete message from chat") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    requireIntegerParam("message_id", "Message id for remove");
}

// ---------------------------------------------------------------------

void CmdHandlerChatDeleteMessage::handle(ModelRequest *pRequest) {
   
    nlohmann::json jsonRequest = pRequest->jsonRequest();

    WsjcppUserSession *pUserSession = pRequest->getUserSession();
    int nUserId = pUserSession->userid();

    int nIdMessage = 0;
    if (jsonRequest["message_id"].is_number_integer()) {
        nIdMessage = jsonRequest["message_id"];
    }

    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);

    query.prepare("SELECT id, userid FROM chats_messages WHERE id = :msg_id");
    query.bindValue(":msg_id", nIdMessage);

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    if (!query.next() || query.size() < 1) {
        pRequest->sendMessageError(cmd(), WsjcppError(400, "Message not found"));
        return;
    }

    QSqlRecord record = query.record();
    int nMsgUserId = record.value("userid").toInt();

    if (nUserId != nMsgUserId) {
        pRequest->sendMessageError(cmd(), WsjcppError(403, "Editing message allowed only for owner"));
        return;
    }

    QSqlQuery query2(db);
    query2.prepare("DELETE FROM chats_messages WHERE id = :msg_id");
    query2.bindValue(":msg_id", nIdMessage);

    if (!query2.exec()) {
        pRequest->sendMessageError(cmd(), WsjcppError(500, query.lastError().text().toStdString()));
        return;
    }

    nlohmann::json jsonResponse;

    jsonResponse["status"] = "ok";

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------

REGISTRY_CMD(CmdHandlerChatAddToChat)

CmdHandlerChatAddToChat::CmdHandlerChatAddToChat()
    : CmdHandlerBase("chats_add_to_chat", "Add user to some chat") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerChatAddToChat::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------

REGISTRY_CMD(CmdHandlerChatDeleteFromChat)

CmdHandlerChatDeleteFromChat::CmdHandlerChatDeleteFromChat()
    : CmdHandlerBase("chats_delete_from_chat", "Delete user from chat") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerChatDeleteFromChat::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------

REGISTRY_CMD(CmdHandlerChatChangeOwner)

CmdHandlerChatChangeOwner::CmdHandlerChatChangeOwner()
    : CmdHandlerBase("chats_change_owner", "Change owner of the chat") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerChatChangeOwner::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------

REGISTRY_CMD(CmdHandlerChatAddToBlackList)

CmdHandlerChatAddToBlackList::CmdHandlerChatAddToBlackList()
    : CmdHandlerBase("chats_add_to_black_list", "Add user to black list") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerChatAddToBlackList::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------

REGISTRY_CMD(CmdHandlerChatDeleteFromBlackList)

CmdHandlerChatDeleteFromBlackList::CmdHandlerChatDeleteFromBlackList()
    : CmdHandlerBase("chats_delete_from_black_list", "Delete user from black list") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerChatDeleteFromBlackList::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------

REGISTRY_CMD(CmdHandlerChatCreateGroupChat)

CmdHandlerChatCreateGroupChat::CmdHandlerChatCreateGroupChat()
    : CmdHandlerBase("chats_create_group_chat", "Create group chat") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerChatCreateGroupChat::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WsjcppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------