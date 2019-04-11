
#include "validators.h"

// ---------------------------------------------------------------------

ValidatorGameState::ValidatorGameState() {
    TAG = "ValidatorGameState";
    m_vDataTypes = {"original", "copy", "unlicensed_copy"};
};

// ---------------------------------------------------------------------

std::string ValidatorGameState::typeName() {
    return "game_state";
}

// ---------------------------------------------------------------------

bool ValidatorGameState::isValid(const std::string &sValue, std::string &sError) {
    bool bValid = (sValue == "original"
        || sValue == "copy"
        || sValue == "unlicensed_copy");

    if (!bValid) {
        sError = TAG + " expected {'original', 'copy', 'unlicensed_copy'}";
    }
   
    return bValid;
}

// ---------------------------------------------------------------------