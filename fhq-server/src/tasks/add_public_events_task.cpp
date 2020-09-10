#include "add_public_events_task.h"
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <employ_database.h>
#include <QSqlQuery> // TODO redesign
#include <QSqlRecord> // TODO redesign
#include <QString>

AddPublicEventsTask::AddPublicEventsTask(
    const std::string &sType,
    const std::string &sMessage, 
    const nlohmann::json &jsonMeta
) {
    m_sType = sType;
    m_sMessage = sMessage;
    m_jsonMeta = jsonMeta;
    TAG = "AddPublicEventsTask";
};

AddPublicEventsTask::~AddPublicEventsTask() {
    
}

void AddPublicEventsTask::run() {
    WsjcppLog::info(TAG, "message " + m_sMessage);
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("INSERT INTO public_events(type,dt,message,meta) VALUES(:type,NOW(),:message,:meta)");
    query.bindValue(":type", QString::fromStdString(m_sType));
    query.bindValue(":message", QString::fromStdString(m_sMessage));
    std::string sMeta = m_jsonMeta.dump(); 
    // WsjcppLog::warn(TAG, "sMeta = " + sMeta);
    query.bindValue(":meta", QString::fromStdString(sMeta));
    if (!query.exec()) {
        WsjcppLog::err(TAG, query.lastError().text().toStdString());
    }
};
