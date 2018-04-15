#ifndef UPDATES_H
#define UPDATES_H

#include <string>
#include <vector>
#include <log.h>

#include <QString>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

class UpdateBase {

    public:
        UpdateBase(const std::string &sFromVersion, const std::string &sVersion, const std::string &sDescription);
        const std::string &from_version();
        const std::string &version();
        const std::string &description();
        virtual bool update(QSqlDatabase &db, std::string &error) = 0;

    protected:
        std::string TAG;
        std::string m_sFromVersion;
        std::string m_sVersion;
        std::string m_sDescription;
};

extern std::vector<UpdateBase*> *g_pUpdates;

class Updates {
    public:
        static void initGlobalVariables();
        static bool updateDatabase(QSqlDatabase *pDatabase);
};

// RegistryCmdHandler
#define REGISTRY_UPDATE( classname ) \
    static classname * pRegistry ## classname = new classname(); \


#endif // UPDATES_H
