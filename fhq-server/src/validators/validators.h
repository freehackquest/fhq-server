#ifndef VALIDATOR_GAME_STATE_H
#define VALIDATOR_GAME_STATE_H

#include <string>
#include <vector>
#include "validators_base.h"

// ---------------------------------------------------------------------

class ValidatorEventType : public ValidatorStringBase {
    public:
        ValidatorEventType();
        virtual std::string typeName();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vEventTypes;
};

// ---------------------------------------------------------------------

class ValidatorQuestSubject : public ValidatorStringBase {
    public:
        ValidatorQuestSubject();
        virtual std::string typeName();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vQuestSubjects;
};

// ---------------------------------------------------------------------

class ValidatorQuestState : public ValidatorStringBase {
    public:
        ValidatorQuestState();
        virtual std::string typeName();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vQuestStates;
};

// ---------------------------------------------------------------------

class ValidatorGameForm : public ValidatorStringBase {
    public:
        ValidatorGameForm();
        virtual std::string typeName();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vGameForms;
};

// ---------------------------------------------------------------------

class ValidatorGameState : public ValidatorStringBase {
    public:
        ValidatorGameState();
        virtual std::string typeName();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vGameStates;
};

// ---------------------------------------------------------------------

class ValidatorGameType : public ValidatorStringBase {
    public:
        ValidatorGameType();
        virtual std::string typeName();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vGameTypes;
};

#endif // VALIDATOR_GAME_STATE_H
