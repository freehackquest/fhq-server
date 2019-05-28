#ifndef VALIDATOR_GAME_STATE_H
#define VALIDATOR_GAME_STATE_H

#include <string>
#include <vector>
#include <wjscpp_validators.h>

// ---------------------------------------------------------------------

class ValidatorEventType : public ValidatorStringBase {
    public:
        ValidatorEventType();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vEventTypes;
};

// ---------------------------------------------------------------------

class ValidatorQuestSubject : public ValidatorStringBase {
    public:
        ValidatorQuestSubject();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vQuestSubjects;
};

// ---------------------------------------------------------------------

class ValidatorQuestState : public ValidatorStringBase {
    public:
        ValidatorQuestState();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vQuestStates;
};

// ---------------------------------------------------------------------

class ValidatorGameForm : public ValidatorStringBase {
    public:
        ValidatorGameForm();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vGameForms;
};

// ---------------------------------------------------------------------

class ValidatorGameState : public ValidatorStringBase {
    public:
        ValidatorGameState();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vGameStates;
};

// ---------------------------------------------------------------------

class ValidatorGameType : public ValidatorStringBase {
    public:
        ValidatorGameType();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vGameTypes;
};

// ---------------------------------------------------------------------

class ValidatorLXDFileActionType : public ValidatorStringBase {
    public:
        ValidatorLXDFileActionType();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vActionTypes;
};

#endif // VALIDATORS_H
