#include <cmd_handler_mails_list.h>
#include <QJsonArray>
#include <SmtpMime>
// #include <memory_cache_serverinfo.h>
#include <employ_settings.h>

CmdHandlerMailsList::CmdHandlerMailsList(){
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

std::string CmdHandlerMailsList::cmd(){
        return "mails_list";
}

// ---------------------------------------------------------------------

const ModelCommandAccess & CmdHandlerMailsList::access(){
    return m_modelCommandAccess;
}

// ---------------------------------------------------------------------

const std::vector<CmdInputDef> &CmdHandlerMailsList::inputs(){
	return m_vInputs;
}

// ---------------------------------------------------------------------

std::string CmdHandlerMailsList::description(){
        return "Mails list";
}

// ---------------------------------------------------------------------

void CmdHandlerMailsList::handle(ModelRequest *pRequest){
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


    QSqlDatabase db = *(pRequest->server()->database());
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
