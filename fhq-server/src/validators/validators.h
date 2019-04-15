#ifndef VALIDATOR_GAME_STATE_H
#define VALIDATOR_GAME_STATE_H

#include <string>
#include <vector>
#include "validators_base.h"

class ValidatorGameState : public ValidatorStringBase {
    public:
        ValidatorGameState();
        virtual std::string typeName();
        virtual bool isValid(const std::string &sValue, std::string &sError);

    private:
        std::string TAG;
        std::vector<std::string> m_vDataTypes;
};

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

#endif // VALIDATOR_GAME_STATE_H