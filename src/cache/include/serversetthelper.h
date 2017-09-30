#ifndef SERVERSETT_HELPER_H
#define SERVERSETT_HELPER_H

#include <iserver.h>
#include <log.h>

class ServerSettHelper {

public:
    ServerSettHelper();
    ServerSettHelper(QString sGroup, QString sName, QString sValue);
    ServerSettHelper(QString sGroup, QString sName, bool bValue);
    ServerSettHelper(QString sGroup, QString sName, int nValue);

    QString name();
    QString valueAsString();
    int valueAsInteger();
    bool valueAsBoolean();
    void setValue(QString sValue);
    void setValue(bool bValue);
    void setValue(int nValue);

    QString group();
    QString type();
    bool isBoolean();
    bool isString();
    bool isInteger();

private:

    QString m_sName;
    QString m_sValueAsString;
    int m_nValueAsInteger;
    bool m_bValueAsBoolean;
    QString m_sType;
    QString m_sGroup;

    QString TAG;
};

static QString SETT_TYPE_STRING = "string";
static QString SETT_TYPE_BOOLEAN = "boolean";
static QString SETT_TYPE_INTEGER = "integer";

#endif // SERVERSETT_HELPER_H
