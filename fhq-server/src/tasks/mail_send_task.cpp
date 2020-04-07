#include <mail_send_task.h>
#include <employ_database.h>
#include <fallen.h>
#include <curl/curl.h>
#include <sstream>
#include <iostream>
#include <time.h>
#include <QSqlQuery> // TODO redesign
#include <QSqlRecord> // TODO redesign

MailSendTask::MailSendTask(const std::string &sTo, const std::string &sSubject, const std::string &sContent) {
    TAG = "MailSendTask";
    m_sTo = sTo;
    m_sSubject = sSubject;
    m_sContent = sContent;
}

// ---------------------------------------------------------------------

MailSendTask::~MailSendTask() {

}

// ---------------------------------------------------------------------

bool MailSendTask::hasPayloadLine() {
    return m_nLineRead < m_vPayloadText.size();
}

// ---------------------------------------------------------------------

std::string MailSendTask::nextPayloadLine() {
    m_nLineRead++;
    return m_vPayloadText[m_nLineRead-1];
}

// ---------------------------------------------------------------------

size_t curl_task_send_mail(void *ptr, size_t size, size_t nmemb, void *userp) {
    MailSendTask *pTaskMailSend = (MailSendTask *)userp;

    if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
        return 0;
    }
    
    if (pTaskMailSend->hasPayloadLine()) {
        std::string sLine = pTaskMailSend->nextPayloadLine() + "\r\n";
        memcpy(ptr, sLine.c_str(), sLine.length());
        return sLine.length();
    }
    return 0;
}

// ---------------------------------------------------------------------

std::string MailSendTask::generateDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    // "Date: Mon, 2 Dec 2018 16:54:29 +0700"
    std::locale::global(std::locale("en_US.UTF8"));
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %z", &tstruct);

    return buf;
}
// ---------------------------------------------------------------------

std::string MailSendTask::generateMessageID() {
    // dcd7cb36-11db-487a-9f3a-e652a9458e00@rfcpedant.example.org
    std::string sResult = "";
    static const char alphanum[] =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < 23; ++i) {
        sResult += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    sResult += "@mail.freehackquest.com";
    return sResult;
}

// ---------------------------------------------------------------------

// trim from start (in place)
static void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}
// ---------------------------------------------------------------------

// trim from end (in place)
static void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// ---------------------------------------------------------------------

// trim from both ends (in place)
static void trim(std::string &s) {
    
    ltrim(s);
    rtrim(s);
}
// ---------------------------------------------------------------------


void MailSendTask::run() {
    WsjcppLog::info(TAG, "Try send mail to '" + m_sTo + "', with subject: '" + m_sSubject + "'");
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

    std::string sMailHost = pGlobalSettings->get("mail_host").getStringValue();
    long nMailPort = pGlobalSettings->get("mail_port").getNumberValue();
    std::string sMailPassword = pGlobalSettings->get("mail_password").getPasswordValue();
    std::string sMailFrom = pGlobalSettings->get("mail_from").getStringValue();
    std::string sMailSignatureText = pGlobalSettings->get("mail_signature_text").getTextValue();

    // write info about message into DB
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
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
    query.bindValue(":to_email", QString::fromStdString(m_sTo));
    query.bindValue(":subject", QString::fromStdString(m_sSubject));
    query.bindValue(":message", QString::fromStdString(m_sContent));
    query.bindValue(":priority", "");
    query.bindValue(":status", "loaded");
    if (!query.exec()) {
        WsjcppLog::err(TAG, query.lastError().text().toStdString());
        return;
    }

    int nMailID = query.lastInsertId().toInt();

    // try send mail

    std::string sCurlFrom = "<" + sMailFrom + ">";
    std::string sCurlTo = "<" + m_sTo + ">";

    m_vPayloadText.push_back("Date: " + this->generateDateTime());
    m_vPayloadText.push_back("To: " + sCurlTo);
    m_vPayloadText.push_back("From: Free Hack Quest " + sCurlFrom);
    m_vPayloadText.push_back("Message-ID: <" + this->generateMessageID() + ">");

    // return;
    m_vPayloadText.push_back("Subject: " + m_sSubject);
    m_vPayloadText.push_back(""); /* empty line to divide headers from body, see RFC5322 */

    std::stringstream _sContentText(m_sContent.c_str());
    std::string _sContentTextLine;
    while (std::getline(_sContentText, _sContentTextLine, '\n')) {
        trim(_sContentTextLine);
        m_vPayloadText.push_back(_sContentTextLine);
    }
    m_vPayloadText.push_back("");
    std::stringstream _sSignText(sMailSignatureText.c_str());
    std::string _sSignTextLine;
    while (std::getline(_sSignText, _sSignTextLine, '\n')) {
        trim(_sSignTextLine);
        m_vPayloadText.push_back(_sSignTextLine);
    }

    m_nLineRead = 0;

    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;

    curl = curl_easy_init();
    if (curl) {
        /* This is the URL for your mailserver */
        curl_easy_setopt(curl, CURLOPT_URL, sMailHost.c_str());
        // curl_easy_setopt(curl, CURLOPT_PORT, nMailPort);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_USERNAME, sMailFrom.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, sMailPassword.c_str());
        
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, sCurlFrom.c_str());
        recipients = curl_slist_append(recipients, sCurlTo.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, curl_task_send_mail);
        curl_easy_setopt(curl, CURLOPT_READDATA, this);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        // Send the message
        res = curl_easy_perform(curl);
        std::string sStatus = "sent";

        // Check for errors
        if (res != CURLE_OK) {
            WsjcppLog::err(TAG, "Error send mail to '" + m_sTo + "', with subject: '" + m_sSubject + "'. Error: "
                 + std::string(curl_easy_strerror(res)));
            sStatus = "error";
        }
        
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);

        query.prepare("UPDATE email_delivery SET status = :status WHERE id = :id");
        query.bindValue(":status", QString::fromStdString(sStatus));
        query.bindValue(":id", nMailID);
        if (!query.exec()) {
            WsjcppLog::err(TAG, query.lastError().text().toStdString());
            return;
        }
    }
}
