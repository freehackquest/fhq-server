#include <mail_send_task.h>
#include <employ_settings.h>
#include <log.h>

MailSendTask::MailSendTask(IWebSocketServer *pWebSocketServer,  QString to, QString subject, QString content){
    TAG = "MailSendTask";
    m_to = to;
    m_subject = subject;
    m_content = content;
    m_pWebSocketServer = pWebSocketServer;
}

MailSendTask::~MailSendTask(){

}

void MailSendTask::run(){
    EmploySettings *pSettings = findEmploy<EmploySettings>();

    QSqlDatabase db = *(m_pWebSocketServer->database());
    QSqlQuery query(db);

    QString sMailHost = pSettings->getSettString("mail_host");
    int nMailPort = pSettings->getSettInteger("mail_port");
    QString sMailPassword = pSettings->getSettPassword("mail_password");
    QString sMailFrom = pSettings->getSettString("mail_from");

    SmtpClient smtp(sMailHost, nMailPort, SmtpClient::SslConnection);
    smtp.setUser(sMailFrom);
    smtp.setPassword(sMailPassword);

    MimeMessage message;

    EmailAddress sender(sMailFrom);
    message.setSender(&sender);

    EmailAddress to(m_to);
    message.addRecipient(&to);

    message.setSubject(m_subject);

    MimeText text;
    text.setText(m_content);

    message.addPart(&text);

    //write info about message into DB
    query.prepare("INSERT INTO email_delivery("
                  "to_email,"
                  "subject,"
                  "message,"
                  "priority,"
                  "status,"
                  "dt"
                  ")"
                  "VALUES("
                  ":to_email,"
                  ":subject,"
                  ":message,"
                  ":priority,"
                  ":status,"
                  "NOW()"
                  ")");
    query.bindValue(":to_email", m_to);
    query.bindValue(":subject", m_subject);
    query.bindValue(":message", m_content);
    query.bindValue(":priority", "");
    query.bindValue(":status", "loaded");
    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
        return;
    }

    int mail_id = query.lastInsertId().toInt();
    QString status = "sent";

    //send message
    if (!smtp.connectToHost()) {
        Log::err(TAG, "[MAIL] Failed to connect to host!");
        status = "error";
    }

    if (!smtp.login()) {
        Log::err(TAG, "[MAIL] Failed to login!");
        status = "error";
    }

    if (!smtp.sendMail(message)) {
        Log::err(TAG, "[MAIL] Failed to send mail!");
        status = "error";
    }
    smtp.quit();

    query.prepare("UPDATE email_delivery SET status = :status "
                  "WHERE id = :id");
    query.bindValue(":status", status);
    query.bindValue(":id", mail_id);
    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
        return;
    }
}
