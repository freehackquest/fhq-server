#include <cmd_handlers_mails.h>
#include <log.h>
#include <runtasks.h>
#include <log.h>
#include <iostream>
#include <employ_settings.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <QtCore>
#include <SmtpMime>

/*****************************************
 * Mail Info
 *****************************************/


CmdHandlerMailInfo::CmdHandlerMailInfo()
    : CmdHandlerBase("mail_info", "This method Will be return info of mail"){
    TAG = "CmdHandlerMailInfo";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
}

// ---------------------------------------------------------------------

void CmdHandlerMailInfo::handle(ModelRequest *pRequest){
    // QJsonObject jsonRequest = pRequest->data();
    // QJsonObject jsonResponse;

    pRequest->sendMessageError(cmd(), Errors::NotImplementedYet());
}

/*****************************************
 * Send Mail
 *****************************************/

CmdHandlerMailSend::CmdHandlerMailSend()
    : CmdHandlerBase("mail_send", "Mail Send"){

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("to").required().email_().description("E-mail of the recipient"));
    m_vInputs.push_back(CmdInputDef("subject").required().string_().description("Subject of the message"));
    m_vInputs.push_back(CmdInputDef("body").required().string_().description("Body of the message"));

}

// ---------------------------------------------------------------------

void CmdHandlerMailSend::handle(ModelRequest *pRequest){
    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QString sEmail = jsonRequest["to"].toString();
    QString sSubject = jsonRequest["subject"].toString();
    QString sBody = jsonRequest["body"].toString();

    RunTasks::MailSend(pRequest->server(), sEmail, sSubject, sBody);

    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

/*****************************************
 * MailsList
 *****************************************/

CmdHandlerMailsList::CmdHandlerMailsList()
    : CmdHandlerBase("mails_list", "Mails list"){
    TAG = "CmdHandlerMailsList";

    m_modelCommandAccess.setAccessUnauthorized(false);
    m_modelCommandAccess.setAccessUser(false);
    m_modelCommandAccess.setAccessAdmin(true);

    // validation and description input fields
    m_vInputs.push_back(CmdInputDef("filter_email").string_().optional().description("Filter by email"));
    m_vInputs.push_back(CmdInputDef("filter_subject").string_().optional().description("Filter by subject"));
    m_vInputs.push_back(CmdInputDef("filter_message").string_().optional().description("Filter by message"));
    m_vInputs.push_back(CmdInputDef("onpage").integer_().optional().description("On page"));
    m_vInputs.push_back(CmdInputDef("page").integer_().optional().description("page"));
}

// ---------------------------------------------------------------------

void CmdHandlerMailsList::handle(ModelRequest *pRequest){
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();

    QJsonObject jsonRequest = pRequest->data();
    QJsonObject jsonResponse;

    QStringList filters;
    QMap<QString,QString> filter_values;

    int nPage = 0;
    int nOnPage = 5;
    int nCount = 0;

    if(jsonRequest.contains("filter_email")){
        QString sEmail = jsonRequest["filter_email"].toString().trimmed();
        if(sEmail != ""){
            filters << "(ed.to_email LIKE :email)";
            filter_values[":email"] = "%" + sEmail + "%";
        }
    }

    if(jsonRequest.contains("filter_subject")){
        QString sSubject = jsonRequest["filter_subject"].toString().trimmed();
        if(sSubject != ""){
            filters << "(ed.subject LIKE :subject)";
            filter_values[":subject"] = "%" + sSubject + "%";
        }
    }

    if(jsonRequest.contains("filter_message")){
        QString sMessage = jsonRequest["filter_message"].toString().trimmed();
        if(sMessage != ""){
            filters << "(ed.message LIKE :message)";
            filter_values[":message"] = "%" + sMessage + "%";
        }
    }

    if(jsonRequest.contains("page")){
        nPage = jsonRequest["page"].toInt();
    }

    if(jsonRequest.contains("onpage")){
        nOnPage = jsonRequest["onpage"].toInt();
    }


    QSqlDatabase db = *(pDatabase->database());
    QString where = filters.join(" AND ");
    if(where.length() > 0){
        where = "WHERE " + where;
    }

    // calculate count
    {
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(*) cnt FROM email_delivery ed " + where);
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        if(!query.exec()){
            pRequest->sendMessageError(cmd(), Error(500, query.lastError().text()));
            return;
        }
        if(query.next()) {
            QSqlRecord record = query.record();
            nCount = record.value("cnt").toInt();
        }
    }

    // emails
    QJsonArray emails;
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM email_delivery ed " + where + " ORDER BY ed.dt DESC LIMIT " + QString::number(nPage*nOnPage) + "," + QString::number(nOnPage));
        foreach(QString key, filter_values.keys() ){
            query.bindValue(key, filter_values.value(key));
        }
        query.exec();
        while (query.next()) {
            QSqlRecord record = query.record();
            QJsonObject email;

            email["id"] = record.value("id").toInt();
            email["email"] = record.value("to_email").toString();
            email["subject"] = record.value("subject").toString().toHtmlEscaped();
            email["message"] = record.value("message").toString().toHtmlEscaped();
            email["priority"] = record.value("priority").toString().toHtmlEscaped();
            email["status"] = record.value("status").toString().toHtmlEscaped();
            email["dt"] = record.value("dt").toString().toHtmlEscaped();
            emails.push_back(email);
        }
    }

    jsonResponse["data"] = emails;
    jsonResponse["onpage"] = nOnPage;
    jsonResponse["page"] = nPage;
    jsonResponse["count"] = nCount;
    pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
