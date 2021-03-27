#ifndef MAIL_SEND_TASK_H
#define MAIL_SEND_TASK_H

#include <QRunnable>
#include <string>
#include <vector>

class MailSendTask : public QRunnable {

    void run();

    public:
        MailSendTask(const std::string &sTo, const std::string &sSubject, const std::string &sContent);
        ~MailSendTask();
        bool hasPayloadLine();
        std::string nextPayloadLine();
    private:
        std::string generateDateTime();
        std::string generateMessageID();

        std::string TAG;
        std::string m_sTo;
        std::string m_sSubject;
        std::string m_sContent;
        std::vector<std::string> m_vPayloadText;
        int m_nLineRead;
};

#endif // MAIL_SEND_TASK_H

