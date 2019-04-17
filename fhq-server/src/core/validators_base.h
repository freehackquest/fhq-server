#ifndef VALIDATOR_BASE_H
#define VALIDATOR_BASE_H

#include <string>
#include <regex>

enum ValidatorType {
    VALIDATOR_STRING,
    VALIDATOR_NUMBER
};

// ----------------------------------------------------------------------

class ValidatorStringBase {
    public:
        virtual ValidatorType baseType();
        virtual bool isValid(const std::string &sValue, std::string &sError) = 0;
};

// ----------------------------------------------------------------------

class ValidatorNumberBase {
    public:
        virtual ValidatorType baseType();
        virtual bool isValid(int sValue, std::string &sError) = 0;
};

// ----------------------------------------------------------------------

class ValidatorEmail : public ValidatorStringBase {
    public:
        ValidatorEmail();
        virtual std::string typeName();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::regex m_rxEmail;
};

// ----------------------------------------------------------------------

class ValidatorUUID : public ValidatorStringBase {
    public:
        ValidatorUUID();
        virtual std::string typeName();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::regex m_rxUUID;
};

// ----------------------------------------------------------------------

class ValidatorStringLength : public ValidatorStringBase {
    public:
        ValidatorStringLength(int nMinLength, int nMaxLength);
        virtual std::string typeName();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        int m_nMinLength;
        int m_nMaxLength;
};

// ----------------------------------------------------------------------

class ValidatorLanguage : public ValidatorStringBase {
    public:
        ValidatorLanguage();
        virtual std::string typeName();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
};

// ----------------------------------------------------------------------

#endif // VALIDATOR_BASE_H