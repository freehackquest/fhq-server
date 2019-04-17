#include <model_notification.h>
#include <iostream>

// ---------------------------------------------------------------------

ModelNotification::ModelNotification(const std::string &sType,
                                     const std::string &sSection,
                                     const std::string &sMessage) {
    TAG = "ModelNotification";
    m_sType = sType;
    m_sSection = sSection;
    m_sMessage = sMessage;
}

// ---------------------------------------------------------------------

std::string ModelNotification::type() {
    return m_sType;
}

// ---------------------------------------------------------------------

std::string ModelNotification::message() {
    return m_sMessage;
}

// ---------------------------------------------------------------------

nlohmann::json ModelNotification::toJson() {
    nlohmann::json jsonNotification;
    jsonNotification["type"] = m_sType;
    jsonNotification["section"] = m_sSection;
    jsonNotification["message"] = m_sMessage;
    return jsonNotification;
}

// ---------------------------------------------------------------------
