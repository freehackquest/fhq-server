#include <cmd_handlers_mails.h>
#include <fallen.h>
#include <runtasks.h>
#include <iostream>
#include <employ_database.h>
#include <employ_server_info.h>
#include <QtCore>

/*****************************************
 * Mail Info
 *****************************************/


CmdHandlerMailInfo::CmdHandlerMailInfo()
    : CmdHandlerBase("mail_info", "This method Will be return info of mail") {
    TAG = "CmdHandlerMailInfo";

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

}

// ---------------------------------------------------------------------

void CmdHandlerMailInfo::handle(ModelRequest *pRequest) {
    // QJsonObject jsonRequest = pRequest->data();
    // QJsonObject jsonResponse;

    pRequest->sendMessageError(cmd(), WSJCppError(501, "Not Implemented Yet"));
}

/*****************************************
 * Send Mail
 *****************************************/

CmdHandlerMailSend::CmdHandlerMailSend()
    : CmdHandlerBase("mail_send", "Mail Send") {

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    // validation and description input fields
    requireStringParam("to", "E-mail of the recipient")
        .addValidator(new ValidatorEmail());
    requireStringParam("subject", "Subject of the message");
    requireStringParam("body", "Body of the message");

}

// ---------------------------------------------------------------------

void CmdHandlerMailSend::handle(ModelRequest *pRequest) {
    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    std::string sEmail = jsonRequest["to"].toString().toStdString();
    std::string sSubject = jsonRequest["subject"].toString().toStdString();
    std::string sBody = jsonRequest["body"].toString().toStdString();

    RunTasks::MailSend(sEmail, sSubject, sBody);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*****************************************
 * MailsList
 *****************************************/

CmdHandlerMailsList::CmdHandlerMailsList()
    : CmdHandlerBase("mails_list", "Mails list") {
    TAG = "CmdHandlerMailsList";

    setAccessUnauthorized(false);
    setAccessUser(false);
    setAccessAdmin(true);

    optionalStringParam("filter_email", "Filter by email");
    optionalStringParam("filter_subject", "Filter by subject");
    optionalStringParam("filter_message", "Filter by message");
    optionalIntegerParam("onpage", "On page"); // TODO validator
    optionalIntegerParam("page", "page"); // TODO validator
}

// ---------------------------------------------------------------------

void CmdHandlerMailsList::handle(ModelRequest *pRequest) {
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    nlohmann::json jsonResponse;

    QStringList filters;
    QMap<QString,QString> filter_values;

    int nPage = 0;
    int nOnPage = 5;
    int nCount = 0;

    if (jsonRequest.contains("filter_email")) {
        QString sEmail = jsonRequest["filter_email"].toString().trimmed();
        if (sEmail != "") {
            filters << "(ed.to_email LIKE :email)";
            filter_values[":email"] = "%" + sEmail + "%";
        }
    }

    if (jsonRequest.contains("filter_subject")) {
        QString sSubject = jsonRequest["filter_subject"].toString().trimmed();
        if (sSubject != "") {
            filters << "(ed.subject LIKE :subject)";
            filter_values[":subject"] = "%" + sSubject + "%";
        }
    }

    if (jsonRequest.contains("filter_message")) {
        QString sMessage = jsonRequest["filter_message"].toString().trimmed();
        if (sMessage != "") {
            filters << "(ed.message LIKE :message)";
            filter_values[":message"] = "%" + sMessage + "%";
        }
    }

    if (jsonRequest.contains("page")) {
        nPage = jsonRequest["page"].toInt();
    }

    if (jsonRequest.contains("onpage")) {
        nOnPage = jsonRequest["onpage"].toInt();
    }


    QSqlDatabase db = *(pDatabase->database());
    QString where = filters.join(" AND ");
    if (where.length() > 0) {
        where = "WHERE " + where;
    }

    // calculate count
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM email_delivery ed " + where);
        foreach (QString key, filter_values.keys()) {
            query.bindValue(key, filter_values.value(key));
        }
        if (!query.exec()) {
            pRequest->sendMessageError(cmd(), WSJCppError(500, query.lastError().text().toStdString()));
            return;
        }
        if (query.next()) {
            QSqlRecord record = query.record();
            nCount = record.value("cnt").toInt();
        }
    }

    // emails
    nlohmann::json jsonEmails = nlohmann::json::array();
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM email_delivery ed " + where + " ORDER BY ed.dt DESC LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage));
        foreach (QString key, filter_values.keys()) {
            query.bindValue(key, filter_values.value(key));
        }
        query.exec();
        while (query.next()) {
            QSqlRecord record = query.record();
            nlohmann::json jsonEmail;

            jsonEmail["id"] = record.value("id").toInt();
            jsonEmail["email"] = record.value("to_email").toString().toStdString();
            jsonEmail["subject"] = record.value("subject").toString().toHtmlEscaped().toStdString();
            jsonEmail["message"] = record.value("message").toString().toHtmlEscaped().toStdString();
            jsonEmail["priority"] = record.value("priority").toString().toHtmlEscaped().toStdString();
            jsonEmail["status"] = record.value("status").toString().toHtmlEscaped().toStdString();
            jsonEmail["dt"] = record.value("dt").toString().toHtmlEscaped().toStdString();
            jsonEmails.push_back(jsonEmail);
        }
    }

    jsonResponse["data"] = jsonEmails;
    jsonResponse["onpage"] = nOnPage;
    jsonResponse["page"] = nPage;
    jsonResponse["count"] = nCount;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
