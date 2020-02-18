#ifndef WSJCPP_VALIDATOR_H
#define WSJCPP_VALIDATOR_H

#include <string>
#include <vector>
#include <regex>

enum WSJCppValidatorType {
    WSJCPP_VALIDATOR_STRING,
    WSJCPP_VALIDATOR_NUMBER
};

// ----------------------------------------------------------------------

class WSJCppValidators {
    public:
        static bool isValidDate(const std::string &sValue, std::string &sError);
        static bool isValidTimeH24(const std::string &sValue, std::string &sError);
        static bool isValidURLProtocol(const std::string &sValue, std::string &sError);
        static bool isValidDomainName(const std::string &sValue, std::string &sError);
        static bool isValidPort(const std::string &sValue, std::string &sError);
        static bool isValidPort(int nValue, std::string &sError);
        static bool isValidBase64(const std::string &sValue, std::string &sError);
        static bool isValidIPv4(const std::string &sValue, std::string &sError);
        static bool isValidIPv6(const std::string &sValue, std::string &sError);
};

/*
// ----------------------------------------------------------------------

class WSJCppValidatorBase {
    public:
        WSJCppValidatorBase(const std::string &sTypeName, WSJCppValidatorType nValidatorType);
        virtual WSJCppValidatorType getBaseType();
        virtual std::string getTypeName();
        virtual bool isValid(const std::string &sValue, std::string &sError) = 0;
    protected:
        std::string TAG;
    private:
        std::string m_sTypeName;
};

*/

// ----------------------------------------------------------------------

class WSJCppValidatorStringBase {
    public:
        WSJCppValidatorStringBase(const std::string &typeName);
        virtual WSJCppValidatorType getBaseType();
        virtual std::string getTypeName();
        virtual bool isValid(const std::string &sValue, std::string &sError) = 0;
    protected:
        std::string TAG;
    private:
        std::string m_sTypeName;
};

// ----------------------------------------------------------------------

class WSJCppValidatorStringRegexpBase : public WSJCppValidatorStringBase {
    public:
        WSJCppValidatorStringRegexpBase(const std::string &typeName, const std::string &sValidator);
        virtual bool isValid(const std::string &sValue, std::string &sError) override;
    private:
        std::string m_sValidator;
        std::regex m_rxValidator;
};

// ----------------------------------------------------------------------

class WSJCppValidatorStringListBase : public WSJCppValidatorStringBase {
    public:
        WSJCppValidatorStringListBase(const std::string &typeName, const std::vector<std::string> &vListValues);
        virtual bool isValid(const std::string &sValue, std::string &sError) override;
    private:
        std::vector<std::string> m_vListValues;
};

// ----------------------------------------------------------------------

class WSJCppValidatorEmail : public WSJCppValidatorStringRegexpBase {
    public:
        WSJCppValidatorEmail();
};

// ----------------------------------------------------------------------

class WSJCppValidatorUUID : public WSJCppValidatorStringRegexpBase {
    public:
        WSJCppValidatorUUID();
};

// ----------------------------------------------------------------------

class WSJCppValidatorStringLength : public WSJCppValidatorStringBase {
    public:
        WSJCppValidatorStringLength(int nMinLength, int nMaxLength);
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        int m_nMinLength;
        int m_nMaxLength;
};

// ----------------------------------------------------------------------

class WSJCppValidatorJWT : public WSJCppValidatorStringRegexpBase {
    public:
        WSJCppValidatorJWT();
};

// ----------------------------------------------------------------------

class WSJCppValidatorDate : public WSJCppValidatorStringBase {
    public:
        WSJCppValidatorDate();
        virtual bool isValid(const std::string &sValue, std::string &sError);
    private:
        std::string TAG;
};

// ----------------------------------------------------------------------

class WSJCppValidatorTimeH24 : public WSJCppValidatorStringBase {
    public:
        WSJCppValidatorTimeH24();
        virtual bool isValid(const std::string &sValue, std::string &sError);
    private:
        std::string TAG;
};

// ----------------------------------------------------------------------

class WSJCppValidatorDateTime : public WSJCppValidatorStringBase {
    public:
        WSJCppValidatorDateTime();
        virtual bool isValid(const std::string &sValue, std::string &sError);
    private:
        std::string TAG;
};

// ----------------------------------------------------------------------

class WSJCppValidatorURL : public WSJCppValidatorStringBase {
    public:
        WSJCppValidatorURL();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::regex m_rxLikeIPv4Format;
};

// ----------------------------------------------------------------------

class WSJCppValidatorBase64 : public WSJCppValidatorStringBase {
    public:
        WSJCppValidatorBase64();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
};

// ----------------------------------------------------------------------

class WSJCppValidatorNumber : public WSJCppValidatorStringBase {
    public:
        WSJCppValidatorNumber();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
};

// ----------------------------------------------------------------------

class WSJCppValidatorHex : public WSJCppValidatorStringBase {
    public:
        WSJCppValidatorHex();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
};

// ----------------------------------------------------------------------

#endif // WSJCPP_VALIDATOR_H
