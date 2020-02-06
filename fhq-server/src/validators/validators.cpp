
#include "validators.h"

// ---------------------------------------------------------------------
// ValidatorEventType

ValidatorEventType::ValidatorEventType() : WSJCppValidatorStringBase("event_type") {
    TAG = "ValidatorEventType";
    m_vEventTypes = {"info", "users", "games", "quests", "warning"};
};

// ---------------------------------------------------------------------

bool ValidatorEventType::isValid(const std::string &sValue, std::string &sError) {
    if (std::find(m_vEventTypes.begin(), m_vEventTypes.end(), sValue) != m_vEventTypes.end()) {
        return true;
    }
    // TODO Fallen::join
    sError = " expected one of ['info', 'users', 'games', 'quests', 'warning']";
    return false;
}

// ---------------------------------------------------------------------
// ValidatorQuestSubject
 
ValidatorQuestSubject::ValidatorQuestSubject() : WSJCppValidatorStringBase("quest_subject") {
    TAG = "ValidatorQuestSubject";
    m_vQuestSubjects = {
        "trivia", "hashes", "stego", "reverse",
        "recon", "crypto", "forensics", "network",
        "web", "ppc", "admin", "enjoy", "unknown"
    };
};

// ---------------------------------------------------------------------

bool ValidatorQuestSubject::isValid(const std::string &sValue, std::string &sError) {
    if (std::find(m_vQuestSubjects.begin(), m_vQuestSubjects.end(), sValue) != m_vQuestSubjects.end()) {
        return true;
    }
    // TODO Fallen::join
    sError = " expected one of [";
    for (int i = 0; i < m_vQuestSubjects.size(); i++) {
        sError += "'" + m_vQuestSubjects[i] + "'";
        if (i < m_vQuestSubjects.size() - 1) {
            sError += ", ";
        }
    }
    sError += "]";
    return false;
}

// ---------------------------------------------------------------------
// ValidatorQuestSubject
 
ValidatorQuestState::ValidatorQuestState() : WSJCppValidatorStringBase("quest_state") {
    TAG = "ValidatorQuestState";
    m_vQuestStates = {"open", "broken", "closed"};
};

// ---------------------------------------------------------------------

bool ValidatorQuestState::isValid(const std::string &sValue, std::string &sError) {
    if (std::find(m_vQuestStates.begin(), m_vQuestStates.end(), sValue) != m_vQuestStates.end()) {
        return true;
    }
    // TODO Fallen::join
    sError = " expected one of [";
    for (int i = 0; i < m_vQuestStates.size(); i++) {
        sError += "'" + m_vQuestStates[i] + "'";
        if (i < m_vQuestStates.size() - 1) {
            sError += ", ";
        }
    }
    sError += "]";
    return false;
}

// ---------------------------------------------------------------------
// ValidatorGameForm
 
ValidatorGameForm::ValidatorGameForm() : WSJCppValidatorStringBase("game_form") {
    TAG = "ValidatorGameForm";
    m_vGameForms = {"online", "offline"};
};

// ---------------------------------------------------------------------

bool ValidatorGameForm::isValid(const std::string &sValue, std::string &sError) {
    if (std::find(m_vGameForms.begin(), m_vGameForms.end(), sValue) != m_vGameForms.end()) {
        return true;
    }
    // TODO Fallen::join
    sError = " expected one of [";
    for (int i = 0; i < m_vGameForms.size(); i++) {
        sError += "'" + m_vGameForms[i] + "'";
        if (i < m_vGameForms.size() - 1) {
            sError += ", ";
        }
    }
    sError += "]";
    return false;
}


// ---------------------------------------------------------------------
// ValidatorGameState
 
ValidatorGameState::ValidatorGameState() : WSJCppValidatorStringBase("game_state") {
    TAG = "ValidatorGameForm";
    m_vGameStates = {"original", "copy", "unlicensed_copy"};
};

// ---------------------------------------------------------------------

bool ValidatorGameState::isValid(const std::string &sValue, std::string &sError) {
    if (std::find(m_vGameStates.begin(), m_vGameStates.end(), sValue) != m_vGameStates.end()) {
        return true;
    }
    // TODO Fallen::join
    sError = " expected one of [";
    for (int i = 0; i < m_vGameStates.size(); i++) {
        sError += "'" + m_vGameStates[i] + "'";
        if (i < m_vGameStates.size() - 1) {
            sError += ", ";
        }
    }
    sError += "]";
    return false;
}

// ---------------------------------------------------------------------
// ValidatorGameType
 
ValidatorGameType::ValidatorGameType() : WSJCppValidatorStringBase("game_type") {
    TAG = "ValidatorGameType";
    m_vGameTypes = {"jeopardy"};
};

// ---------------------------------------------------------------------

bool ValidatorGameType::isValid(const std::string &sValue, std::string &sError) {
    if (std::find(m_vGameTypes.begin(), m_vGameTypes.end(), sValue) != m_vGameTypes.end()) {
        return true;
    }
    // TODO Fallen::join
    sError = " expected one of [";
    for (int i = 0; i < m_vGameTypes.size(); i++) {
        sError += "'" + m_vGameTypes[i] + "'";
        if (i < m_vGameTypes.size() - 1) {
            sError += ", ";
        }
    }
    sError += "]";
    return false;
}

// ---------------------------------------------------------------------
// ValidatorLXDFileActionType
 
ValidatorLXDFileActionType::ValidatorLXDFileActionType() : WSJCppValidatorStringBase("lxd_action_type") {
    TAG = "ValidatorLXDFileActionType";
    m_vActionTypes = {"pull", "push", "delete"};
};

// ---------------------------------------------------------------------

bool ValidatorLXDFileActionType::isValid(const std::string &sValue, std::string &sError) {
    if (std::find(m_vActionTypes.begin(), m_vActionTypes.end(), sValue) != m_vActionTypes.end()) {
        return true;
    }
    // TODO Fallen::join
    sError = " expected one of [";
    for (int i = 0; i < m_vActionTypes.size(); i++) {
        sError += "'" + m_vActionTypes[i] + "'";
        if (i < m_vActionTypes.size() - 1) {
            sError += ", ";
        }
    }
    sError += "]";
    return false;
}

// ----------------------------------------------------------------------
// ValidatorLanguage

ValidatorLanguage::ValidatorLanguage() 
: WSJCppValidatorStringListBase(
    "lang", {"en", "ru", "de"} 
) {
    TAG = "ValidatorLanguage";
}

// ---------------------------------------------------------------------