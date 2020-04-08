#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <string>
#include <vector>
#include <wsjcpp_validators.h>

// ---------------------------------------------------------------------

class ValidatorEventType : public WsjcppValidatorStringBase {
    public:
        ValidatorEventType();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vEventTypes;
};

// ---------------------------------------------------------------------

class ValidatorQuestSubject : public WsjcppValidatorStringBase {
    public:
        ValidatorQuestSubject();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vQuestSubjects;
};

// ---------------------------------------------------------------------

class ValidatorQuestState : public WsjcppValidatorStringBase {
    public:
        ValidatorQuestState();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vQuestStates;
};

// ---------------------------------------------------------------------

class ValidatorGameForm : public WsjcppValidatorStringBase {
    public:
        ValidatorGameForm();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vGameForms;
};

// ---------------------------------------------------------------------

class ValidatorGameState : public WsjcppValidatorStringBase {
    public:
        ValidatorGameState();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vGameStates;
};

// ---------------------------------------------------------------------

class ValidatorGameType : public WsjcppValidatorStringBase {
    public:
        ValidatorGameType();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vGameTypes;
};

// ---------------------------------------------------------------------

class ValidatorLXDFileActionType : public WsjcppValidatorStringBase {
    public:
        ValidatorLXDFileActionType();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vActionTypes;
};

// ----------------------------------------------------------------------

class ValidatorLanguage : public WsjcppValidatorStringListBase {
    public:
        ValidatorLanguage();
};

// ----------------------------------------------------------------------

#endif // VALIDATORS_H
