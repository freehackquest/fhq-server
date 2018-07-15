#ifndef MODEL_GAME_H
#define MODEL_GAME_H

#include <string>
#include <json.hpp>

class ModelGame {
    public:
        ModelGame();
        
        int localId();
        void setLocalId(int nLocalId);
        const std::string &uuid();
        void setUuid(std::string sUuid);
        const std::string &name();
        void setName(std::string sName);

        
        nlohmann::json toJson();
        void fillFrom(nlohmann::json &jsonGame);
    private:
        std::string TAG;
        int m_nLocalId; // deprecated
        std::string m_sUuid;
        std::string m_sName;
};

#endif // MODEL_GAME_H
