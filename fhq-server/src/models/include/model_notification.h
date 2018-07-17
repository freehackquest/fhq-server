#ifndef MODEL_NOTIFICATION_H
#define MODEL_NOTIFICATION_H

#include <string>
#include <json.hpp>

class ModelNotification {
    public:
        ModelNotification(const std::string &sType, const std::string &sSection, const std::string &sMessage);
        std::string type();
        std::string message();
        nlohmann::json toJson();
    private:
        std::string TAG;
        std::string m_sType;
        std::string m_sSection;
        std::string m_sMessage;
};

#endif // MODEL_NOTIFICATION_H
