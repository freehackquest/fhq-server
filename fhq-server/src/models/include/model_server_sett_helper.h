#ifndef MODEL_SERVER_SETT_HELPER_H
#define MODEL_SERVER_SETT_HELPER_H

#include <iserver.h>
#include <log.h>

class ModelServerSettHelper {

public:
    ModelServerSettHelper();
    ModelServerSettHelper(const std::string &sGroup, const std::string &sName, QString sValue, bool bPassword = false);
    ModelServerSettHelper(const std::string &sGroup, const std::string &sName, bool bValue);
    ModelServerSettHelper(const std::string &sGroup, const std::string &sName, int nValue);

    const std::string &name();
    QString valueAsString();
    QString valueAsPassword();
    int valueAsInteger();
    bool valueAsBoolean();
    void setValue(QString sValue);
    void setValue(bool bValue);
    void setValue(int nValue);

    const std::string &group();
    const std::string &type();
    bool isBoolean();
    bool isString();
    bool isInteger();
    bool isPassword();

private:
    std::string TAG;

    std::string m_sName;
    QString m_sValueAsString;
    int m_nValueAsInteger;
    bool m_bValueAsBoolean;
    std::string m_sType;
    std::string m_sGroup;

};

static std::string SETT_TYPE_UNKNOWN = "unknown";
static std::string SETT_TYPE_STRING = "string";
static std::string SETT_TYPE_BOOLEAN = "boolean";
static std::string SETT_TYPE_INTEGER = "integer";
static std::string SETT_TYPE_PASSWORD = "password";

#endif // MODEL_SERVER_SETT_HELPER_H
