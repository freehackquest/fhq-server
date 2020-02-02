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

    WSJCppUserSession *pUserSession = pRequest->getUserSession();
    std::string sUsername = pUserSession->nick().toStdString();

    std::string sMessage = "";
    if (jsonRequest["message"].is_string()) {
        sMessage = jsonRequest["message"];
    }
    WSJCppCore::trim(sMessage);
    if (sMessage.length() == 0) {
        pRequest->sendMessageError(cmd(), WSJCppError(400, "Message could not be empty"));
        return;
    }

    std::string sType = "";
    if (jsonRequest["type"].is_string()) {
        sType = jsonRequest["type"];
    }

    pRequest->sendMessageSuccess(cmd(), jsonResponse);

    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
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

    EmployNotify *pNotify = findEmploy<EmployNotify>();
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

    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT user, message, dt FROM chatmessages ORDER BY dt DESC LIMIT 0,25");

    if (!query.exec()) {
        pRequest->sendMessageError(cmd(), WSJCppError(500, query.lastError().text().toStdString()));
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

REGISTRY_CMD(CmdHandlerChatReadMessage)

CmdHandlerChatReadMessage::CmdHandlerChatReadMessage()
    : CmdHandlerBase("chats_message_read", "Change chat message status from \"unread\" to \"read\"") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerChatReadMessage::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WSJCppError(501, "Not Implemented Yet"));
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
    pRequest->sendMessageError(cmd(), WSJCppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------

REGISTRY_CMD(CmdHandlerChatEditMessage)

CmdHandlerChatEditMessage::CmdHandlerChatEditMessage()
    : CmdHandlerBase("chats_message_edit", "Edit message") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerChatEditMessage::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WSJCppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------

REGISTRY_CMD(CmdHandlerChatDeleteMessage)

CmdHandlerChatDeleteMessage::CmdHandlerChatDeleteMessage()
    : CmdHandlerBase("chats_message_delete", "Delete message from chat") {
    
    setActivatedFromVersion("0.2.24");

    setAccessUnauthorized(false);
    setAccessUser(true);
    setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerChatDeleteMessage::handle(ModelRequest *pRequest) {
    pRequest->sendMessageError(cmd(), WSJCppError(501, "Not Implemented Yet"));
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
    pRequest->sendMessageError(cmd(), WSJCppError(501, "Not Implemented Yet"));
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
    pRequest->sendMessageError(cmd(), WSJCppError(501, "Not Implemented Yet"));
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
    pRequest->sendMessageError(cmd(), WSJCppError(501, "Not Implemented Yet"));
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
    pRequest->sendMessageError(cmd(), WSJCppError(501, "Not Implemented Yet"));
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
    pRequest->sendMessageError(cmd(), WSJCppError(501, "Not Implemented Yet"));
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
    pRequest->sendMessageError(cmd(), WSJCppError(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------