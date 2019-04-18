#include "validators_base.h"

// ----------------------------------------------------------------------
// ValidatorStringBase

ValidatorType ValidatorStringBase::baseType() {
    return ValidatorType::VALIDATOR_STRING;
}

// ----------------------------------------------------------------------
// ValidatorNumberBase

ValidatorType ValidatorNumberBase::baseType() {
    return ValidatorType::VALIDATOR_NUMBER;
}

// ----------------------------------------------------------------------
// ValidatorEmail

ValidatorEmail::ValidatorEmail() {
    TAG = "ValidatorEmail";
    m_rxEmail = std::regex("^[0-9a-zA-Z]{1}[0-9a-zA-Z-._]*[0-9a-zA-Z]{1}@[0-9a-zA-Z]{1}[-.0-9a-zA-Z]*[0-9a-zA-Z]{1}\\.[a-zA-Z]{2,6}$");
}

// ----------------------------------------------------------------------

std::string ValidatorEmail::typeName() {
    return "email";
}

// ----------------------------------------------------------------------

bool ValidatorEmail::isValid(const std::string &sValue, std::string &sError) {
    if (!std::regex_match(sValue, m_rxEmail)) {
        sError = "Value must have format email";
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------
// ValidatorUUID

ValidatorUUID::ValidatorUUID() {
    TAG = "ValidatorUUID";
    m_rxUUID = std::regex("^[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}$");
}

// ----------------------------------------------------------------------

std::string ValidatorUUID::typeName() {
    return "uuid";
}

// ----------------------------------------------------------------------

bool ValidatorUUID::isValid(const std::string &sValue, std::string &sError) {
    if (!std::regex_match(sValue, m_rxUUID)) {
        sError = "Value must have format uuid";
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------