#include <employ_quests.h>
#include <employ_database.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMap>
#include <QString>
#include <QVariant>

REGISTRY_WJSCPP_EMPLOY(EmployQuests)

// ---------------------------------------------------------------------

EmployQuests::EmployQuests()
    : WsjcppEmployBase(EmployQuests::name(), {EmployDatabase::name()}) {
    TAG = EmployQuests::name();
}

// ---------------------------------------------------------------------

bool EmployQuests::init() {
    
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("SELECT subject, COUNT(*) as cnt FROM `quest` WHERE quest.state = :state GROUP BY subject");
    query.bindValue(":state", "open");

    if (!query.exec()) {
        WsjcppLog::err(TAG, query.lastError().text().toStdString());
        return false;
    }

    // cache for subjects
    while (query.next()) {
        QSqlRecord record = query.record();
        std::string sSubject = record.value("subject").toString().toStdString();
        int nCount = record.value("cnt").toInt();
        m_mapQuestsSubjects.insert(std::pair<std::string, int>(sSubject, nCount));
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployQuests::deinit() {
    // TODO
    return true;
}

// ---------------------------------------------------------------------
