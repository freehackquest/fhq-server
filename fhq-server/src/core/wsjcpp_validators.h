#ifndef WJSCPP_VALIDATOR_H
#define WJSCPP_VALIDATOR_H

#include <string>
#include <vector>
#include <regex>

enum WJSCppValidatorType {
    WJSCPP_VALIDATOR_STRING,
    WJSCPP_VALIDATOR_NUMBER
};

/*
// ----------------------------------------------------------------------

class WJSCppValidatorBase {
    public:
        WJSCppValidatorBase(const std::string &sTypeName, WJSCppValidatorType nValidatorType);
        virtual WJSCppValidatorType getBaseType();
        virtual std::string getTypeName();
        virtual bool isValid(const std::string &sValue, std::string &sError) = 0;
    protected:
        std::string TAG;
    private:
        std::string m_sTypeName;
};

*/

// ----------------------------------------------------------------------

class ValidatorStringBase {
    public:
        ValidatorStringBase(const std::string &typeName);
        virtual WJSCppValidatorType getBaseType();
        virtual std::string getTypeName();
        virtual bool isValid(const std::string &sValue, std::string &sError) = 0;
    protected:
        std::string TAG;
    private:
        std::string m_sTypeName;
};

// ----------------------------------------------------------------------

class ValidatorStringRegexpBase : public ValidatorStringBase {
    public:
        ValidatorStringRegexpBase(const std::string &typeName, const std::string &sValidator);
        virtual bool isValid(const std::string &sValue, std::string &sError) override;
    private:
        std::string m_sValidator;
        std::regex m_rxValidator;
};

// ----------------------------------------------------------------------

class ValidatorStringListBase : public ValidatorStringBase {
    public:
        ValidatorStringListBase(const std::string &typeName, const std::vector<std::string> &vListValues);
        virtual bool isValid(const std::string &sValue, std::string &sError) override;
    private:
        std::vector<std::string> m_vListValues;
};

// ----------------------------------------------------------------------

class ValidatorEmail : public ValidatorStringRegexpBase {
    public:
        ValidatorEmail();
};

// ----------------------------------------------------------------------

class ValidatorUUID : public ValidatorStringRegexpBase {
    public:
        ValidatorUUID();
};

// ----------------------------------------------------------------------

class ValidatorStringLength : public ValidatorStringBase {
    public:
        ValidatorStringLength(int nMinLength, int nMaxLength);
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        int m_nMinLength;
        int m_nMaxLength;
};

// ----------------------------------------------------------------------

class ValidatorLanguage : public ValidatorStringListBase {
    public:
        ValidatorLanguage();
};

// ----------------------------------------------------------------------

#endif // WJSCPP_VALIDATOR_H