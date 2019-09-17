#include "wsjcpp_validators.h"

// ----------------------------------------------------------------------
// ValidatorStringBase

ValidatorStringBase::ValidatorStringBase(const std::string &sTypeName) {
    TAG = "ValidatorStringBase";
    m_sTypeName = sTypeName;
}

// ----------------------------------------------------------------------

WJSCppValidatorType ValidatorStringBase::getBaseType() {
    return WJSCppValidatorType::WJSCPP_VALIDATOR_STRING;
}

// ----------------------------------------------------------------------

std::string ValidatorStringBase::getTypeName() {
    return m_sTypeName;
}

// ----------------------------------------------------------------------
// ValidatorStringRegexpBase

ValidatorStringRegexpBase::ValidatorStringRegexpBase(const std::string &typeName, const std::string &sValidator)
: ValidatorStringBase(typeName) {
    TAG = "ValidatorStringRegexpBase"; 
    m_sValidator = sValidator;
    m_rxValidator = std::regex(sValidator);
}

// ----------------------------------------------------------------------

bool ValidatorStringRegexpBase::isValid(const std::string &sValue, std::string &sError) {
    if (!std::regex_match(sValue, m_rxValidator)) {
        sError = "Value must have " + getTypeName();
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------
// ValidatorStringListBase

ValidatorStringListBase::ValidatorStringListBase(const std::string &sTypeName, const std::vector<std::string> &vListValues)
: ValidatorStringBase(sTypeName) {
    m_vListValues = vListValues;
}

// ----------------------------------------------------------------------

bool ValidatorStringListBase::isValid(const std::string &sValue, std::string &sError) {
    if (std::find(m_vListValues.begin(), m_vListValues.end(), sValue) != m_vListValues.end()) {
        return true;
    }
    sError = getTypeName() + " expected one of [";
    for (int i = 0; i < m_vListValues.size(); i++) {
        sError += "'" + m_vListValues[i] + "'";
        if (i < m_vListValues.size() - 1) {
            sError += ", ";
        }
    }
    sError += "]";
    return false;
}

// ----------------------------------------------------------------------
// ValidatorEmail

ValidatorEmail::ValidatorEmail()
: ValidatorStringRegexpBase(
    "email",
    "^[0-9a-zA-Z]{1}[0-9a-zA-Z-._]*[0-9a-zA-Z]{1}@[0-9a-zA-Z]{1}[-.0-9a-zA-Z]*[0-9a-zA-Z]{1}\\.[a-zA-Z]{2,6}$"
) {
    TAG = "ValidatorEmail";
}

// ----------------------------------------------------------------------
// ValidatorUUID

ValidatorUUID::ValidatorUUID()
: ValidatorStringRegexpBase(
    "uuid",
    "^[0-9A-Fa-f]{8}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{4}-[0-9A-Fa-f]{12}$"
) {
    TAG = "ValidatorUUID";
}

// ----------------------------------------------------------------------
// ValidatorStringLenght

ValidatorStringLength::ValidatorStringLength(int nMinLength, int nMaxLength) : ValidatorStringBase("string_length") {
    TAG = "ValidatorStringLenght";
    m_nMinLength = nMinLength;
    m_nMaxLength = nMaxLength;
}

// ----------------------------------------------------------------------

bool ValidatorStringLength::isValid(const std::string &sValue, std::string &sError) {
    if (sValue.length() < m_nMinLength) {
        sError = "Value must have more than " + std::to_string(m_nMinLength) + " simbols";
        return false;
    }

    if (sValue.length() > m_nMaxLength) {
        sError = "Value must have less than " + std::to_string(m_nMaxLength) + " simbols";
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------
// ValidatorLanguage

ValidatorLanguage::ValidatorLanguage() 
: ValidatorStringListBase(
    "lang", {"en", "ru", "de"} 
) {
    TAG = "ValidatorLanguage";
}

// ----------------------------------------------------------------------