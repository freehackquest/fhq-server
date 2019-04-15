
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
// ValidatorEventType

ValidatorEventType::ValidatorEventType() {
    TAG = "ValidatorEventType";
    m_vEventTypes = {"info", "users", "games", "quests", "warning"};
};

// ---------------------------------------------------------------------

std::string ValidatorEventType::typeName() {
    return "event_type";
}

// ---------------------------------------------------------------------

bool ValidatorEventType::isValid(const std::string &sValue, std::string &sError) {
    if (std::find(m_vEventTypes.begin(), m_vEventTypes.end(), sValue) != m_vEventTypes.end()) {
        return true;
    }
    sError = " expected one of ['info', 'users', 'games', 'quests', 'warning']";
    return false;
}

// ---------------------------------------------------------------------