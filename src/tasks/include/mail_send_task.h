#ifndef MAIL_SEND_TASK_H
#define MAIL_SEND_TASK_H

#include <QRunnable>
#include <SmtpMime>
#include <iserver.h>

class MailSendTask : public QRunnable {

    void run();

    public:
        MailSendTask(IWebSocketServer *pWebSocketServer,  MimeMessage message);
        ~MailSendTask();
    private:
        MimeMessage m_message;
        IWebSocketServer *m_pWebSocketServer;

};

#endif // MAIL_SEND_TASK_H

