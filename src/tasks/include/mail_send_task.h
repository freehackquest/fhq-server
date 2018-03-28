#ifndef MAIL_SEND_TASK_H
#define MAIL_SEND_TASK_H

#include <QRunnable>
#include <SmtpMime>
#include <iserver.h>

class MailSendTask : public QRunnable {

    void run();

    public:
        MailSendTask(IWebSocketServer *pWebSocketServer,  QString to, QString subject, QString content);
        ~MailSendTask();
    private:
        QString TAG;
        QString m_to;
        QString m_subject;
        QString m_content;
        IWebSocketServer *m_pWebSocketServer;
};

#endif // MAIL_SEND_TASK_H

