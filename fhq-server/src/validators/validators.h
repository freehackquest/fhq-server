#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <string>
#include <vector>
#include <wsjcpp_validators.h>

// ---------------------------------------------------------------------

class ValidatorEventType : public WSJCppValidatorStringBase {
    public:
        ValidatorEventType();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vEventTypes;
};

// ---------------------------------------------------------------------

class ValidatorQuestSubject : public WSJCppValidatorStringBase {
    public:
        ValidatorQuestSubject();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vQuestSubjects;
};

// ---------------------------------------------------------------------

class ValidatorQuestState : public WSJCppValidatorStringBase {
    public:
        ValidatorQuestState();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vQuestStates;
};

// ---------------------------------------------------------------------

class ValidatorGameForm : public WSJCppValidatorStringBase {
    public:
        ValidatorGameForm();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vGameForms;
};

// ---------------------------------------------------------------------

class ValidatorGameState : public WSJCppValidatorStringBase {
    public:
        ValidatorGameState();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vGameStates;
};

// ---------------------------------------------------------------------

class ValidatorGameType : public WSJCppValidatorStringBase {
    public:
        ValidatorGameType();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vGameTypes;
};

// ---------------------------------------------------------------------

class ValidatorLXDFileActionType : public WSJCppValidatorStringBase {
    public:
        ValidatorLXDFileActionType();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vActionTypes;
};

// ----------------------------------------------------------------------

class ValidatorLanguage : public WSJCppValidatorStringListBase {
    public:
        ValidatorLanguage();
};

// ----------------------------------------------------------------------

#endif // VALIDATORS_H
