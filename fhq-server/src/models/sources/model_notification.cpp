#include <model_notification.h>
#include <iostream>

// ---------------------------------------------------------------------

ModelNotification::ModelNotification(const std::string &sType, const std::string &sMessage){
    TAG = "ModelNotification";
    m_sType = sType;
    m_sMessage = sMessage;
}

// ---------------------------------------------------------------------

nlohmann::json ModelNotification::toJson(){
    nlohmann::json jsonNotification;
    jsonNotification["type"] = m_sType;
    jsonNotification["name"] = m_sMessage;
    return jsonNotification;
}

// ---------------------------------------------------------------------
