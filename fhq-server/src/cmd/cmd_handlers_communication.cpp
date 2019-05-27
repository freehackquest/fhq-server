#include <cmd_handlers_communication.h>
#include <runtasks.h>
#include <fallen.h>
#include <utils.h>
#include <md5.h>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <employ_notify.h>

/*********************************************
 * Users chat
**********************************************/

REGISTRY_CMD(CmdHandlerChatSendMessage)

CmdHandlerChatSendMessage::CmdHandlerChatSendMessage()
    : CmdHandlerBase("chat_send_message", "Method will be send chat message and it sent to another users") {

    setAccessUnauthorized(true);
    setAccessUser(true);
    setAccessAdmin(true);

    requireStringParam("type", "Type"); // TODO validator chat type
    requireStringParam("message", "Message");
}

// ---------------------------------------------------------------------

void CmdHandlerChatSendMessage::handle(ModelRequest *pRequest) {
    nlohmann::json jsonRequest = pRequest->jsonRequest();
    nlohmann::json jsonResponse;

    WSJCppUserSession *pUserSession = pRequest->userSession();
    std::string sUsername = "";
    if (pUserSession != nullptr) {
        sUsername = pUserSession->nick().toStdString();
    } else {
        sUsername = "Guest";
    }

    std::string sMessage = "";
    if (jsonRequest["message"].is_string()) {
        sMessage = jsonRequest["message"];
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
        pRequest->sendMessageError(cmd(), Error(500, query.lastError().text().toStdString()));
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
