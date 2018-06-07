#include <model_server_sett_helper.h>

#include <log.h>

#include <QTextStream>
#include <QFile>
#include <QByteArray>
#include <QDateTime>
#include <QDir>

ModelServerSettHelper::ModelServerSettHelper(){
       TAG = "ModelServerSettHelper";
}

// ---------------------------------------------------------------------

ModelServerSettHelper::ModelServerSettHelper(const std::string &sGroup, const std::string &sName, QString sValue, bool bPassword) : ModelServerSettHelper(){
    m_sName = sName;
    m_sType = bPassword ? SETT_TYPE_PASSWORD : SETT_TYPE_STRING;
    m_sValueAsString = sValue;
    m_nValueAsInteger = 0;
    m_bValueAsBoolean = false;
    m_sGroup = sGroup;
}

// ---------------------------------------------------------------------

ModelServerSettHelper::ModelServerSettHelper(const std::string &sGroup, const std::string &sName, bool bValue) : ModelServerSettHelper(){
    m_sName = sName;
    m_sType = SETT_TYPE_BOOLEAN;
    m_sValueAsString = bValue ? "yes" : "no";
    m_nValueAsInteger = 0;
    m_bValueAsBoolean = bValue;
    m_sGroup = sGroup;
}

// ---------------------------------------------------------------------

ModelServerSettHelper::ModelServerSettHelper(const std::string &sGroup, const std::string &sName, int nValue) : ModelServerSettHelper(){
    m_sName = sName;
    m_sType = SETT_TYPE_INTEGER;
    m_sValueAsString = QString::number(nValue);
    m_nValueAsInteger = nValue;
    m_bValueAsBoolean = false;
    m_sGroup = sGroup;
}

// ---------------------------------------------------------------------

const std::string &ModelServerSettHelper::name(){
    return m_sName;
}

// ---------------------------------------------------------------------

QString ModelServerSettHelper::valueAsString(){
    return m_sValueAsString;
}

// ---------------------------------------------------------------------

QString ModelServerSettHelper::valueAsPassword(){
    return m_sValueAsString;
}

// ---------------------------------------------------------------------

int ModelServerSettHelper::valueAsInteger(){
    return m_nValueAsInteger;
}

// ---------------------------------------------------------------------

bool ModelServerSettHelper::valueAsBoolean(){
    return m_bValueAsBoolean;
}

// ---------------------------------------------------------------------

void ModelServerSettHelper::setValue(QString sValue){
    m_sValueAsString = sValue;
}

// ---------------------------------------------------------------------

void ModelServerSettHelper::setValue(bool bValue){
    m_bValueAsBoolean = bValue;
    m_sValueAsString = bValue ? "yes" : "no";
}

// ---------------------------------------------------------------------

void ModelServerSettHelper::setValue(int nValue){
    m_nValueAsInteger = nValue;
    m_sValueAsString = QString::number(nValue);
}

// ---------------------------------------------------------------------

const std::string &ModelServerSettHelper::group(){
    return m_sGroup;
}

// ---------------------------------------------------------------------

const std::string &ModelServerSettHelper::type(){
    return m_sType;
}

// ---------------------------------------------------------------------

bool ModelServerSettHelper::isString(){
    return m_sType == SETT_TYPE_STRING;
}

// ---------------------------------------------------------------------

bool ModelServerSettHelper::isBoolean(){
    return m_sType == SETT_TYPE_BOOLEAN;
}

// ---------------------------------------------------------------------

bool ModelServerSettHelper::isInteger(){
    return m_sType == SETT_TYPE_INTEGER;
}

// ---------------------------------------------------------------------

bool ModelServerSettHelper::isPassword(){
    return m_sType == SETT_TYPE_PASSWORD;
}

// ---------------------------------------------------------------------
