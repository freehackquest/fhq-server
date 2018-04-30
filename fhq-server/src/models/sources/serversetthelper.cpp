#include <serversetthelper.h>

#include <log.h>

#include <QTextStream>
#include <QFile>
#include <QByteArray>
#include <QDateTime>
#include <QDir>

ServerSettHelper::ServerSettHelper(){
       TAG = "ServerSettHelper";
}

// ---------------------------------------------------------------------

ServerSettHelper::ServerSettHelper(const std::string &sGroup, const std::string &sName, QString sValue, bool bPassword) : ServerSettHelper(){
    m_sName = sName;
    m_sType = bPassword ? SETT_TYPE_PASSWORD : SETT_TYPE_STRING;
    m_sValueAsString = sValue;
    m_nValueAsInteger = 0;
    m_bValueAsBoolean = false;
    m_sGroup = sGroup;
}

// ---------------------------------------------------------------------

ServerSettHelper::ServerSettHelper(const std::string &sGroup, const std::string &sName, bool bValue) : ServerSettHelper(){
    m_sName = sName;
    m_sType = SETT_TYPE_BOOLEAN;
    m_sValueAsString = bValue ? "yes" : "no";
    m_nValueAsInteger = 0;
    m_bValueAsBoolean = bValue;
    m_sGroup = sGroup;
}

// ---------------------------------------------------------------------

ServerSettHelper::ServerSettHelper(const std::string &sGroup, const std::string &sName, int nValue) : ServerSettHelper(){
    m_sName = sName;
    m_sType = SETT_TYPE_INTEGER;
    m_sValueAsString = QString::number(nValue);
    m_nValueAsInteger = nValue;
    m_bValueAsBoolean = false;
    m_sGroup = sGroup;
}

// ---------------------------------------------------------------------

const std::string &ServerSettHelper::name(){
    return m_sName;
}

// ---------------------------------------------------------------------

QString ServerSettHelper::valueAsString(){
    return m_sValueAsString;
}

// ---------------------------------------------------------------------

QString ServerSettHelper::valueAsPassword(){
    return m_sValueAsString;
}

// ---------------------------------------------------------------------

int ServerSettHelper::valueAsInteger(){
    return m_nValueAsInteger;
}

// ---------------------------------------------------------------------

bool ServerSettHelper::valueAsBoolean(){
    return m_bValueAsBoolean;
}

// ---------------------------------------------------------------------

void ServerSettHelper::setValue(QString sValue){
    m_sValueAsString = sValue;
}

// ---------------------------------------------------------------------

void ServerSettHelper::setValue(bool bValue){
    m_bValueAsBoolean = bValue;
    m_sValueAsString = bValue ? "yes" : "no";
}

// ---------------------------------------------------------------------

void ServerSettHelper::setValue(int nValue){
    m_nValueAsInteger = nValue;
    m_sValueAsString = QString::number(nValue);
}

// ---------------------------------------------------------------------

const std::string &ServerSettHelper::group(){
    return m_sGroup;
}

// ---------------------------------------------------------------------

const std::string &ServerSettHelper::type(){
    return m_sType;
}

// ---------------------------------------------------------------------

bool ServerSettHelper::isString(){
    return m_sType == SETT_TYPE_STRING;
}

// ---------------------------------------------------------------------

bool ServerSettHelper::isBoolean(){
    return m_sType == SETT_TYPE_BOOLEAN;
}

// ---------------------------------------------------------------------

bool ServerSettHelper::isInteger(){
    return m_sType == SETT_TYPE_INTEGER;
}

// ---------------------------------------------------------------------

bool ServerSettHelper::isPassword(){
    return m_sType == SETT_TYPE_PASSWORD;
}

// ---------------------------------------------------------------------
