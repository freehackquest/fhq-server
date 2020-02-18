#include "add_public_events_task.h"
#include <fallen.h>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <employ_database.h>
#include <QSqlQuery> // TODO redesign
#include <QSqlRecord> // TODO redesign
#include <QString>

AddPublicEventsTask::AddPublicEventsTask(const std::string &sType, const std::string &sMessage) {
    m_sType = sType;
    m_sMessage = sMessage;
    TAG = "AddPublicEventsTask";
};

AddPublicEventsTask::~AddPublicEventsTask() {
    
}

void AddPublicEventsTask::run() {
    WSJCppLog::info(TAG, "message " + m_sMessage);
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("INSERT INTO public_events(type,dt,message) VALUES(:type,NOW(),:message)");
    query.bindValue(":type", QString::fromStdString(m_sType));
    query.bindValue(":message", QString::fromStdString(m_sMessage));
    if (!query.exec()) {
        WSJCppLog::err(TAG, query.lastError().text().toStdString());
    }
};
