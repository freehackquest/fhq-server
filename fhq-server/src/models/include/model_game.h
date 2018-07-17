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
        const std::string &description();
        void setDescription(std::string sDescription);
        const std::string &state();
        void setState(std::string sState);
        const std::string &form();
        void setForm(std::string sForm);
        const std::string &type();
        void setType(std::string sType);
        const std::string &dateStart();
        void setDateStart(std::string sDateStart);
        const std::string &dateStop();
        void setDateStop(std::string sDateStop);
        const std::string &dateRestart();
        void setDateRestart(std::string sDateRestart);
        const std::string &organizators();
        void setOrganizators(std::string sOrganizators);
        int maxScore();
        void setMaxScore(int nMaxScore);
        
        nlohmann::json toJson();
        void fillFrom(nlohmann::json &jsonGame);
    private:
        std::string TAG;
        int m_nLocalId; // deprecated
        std::string m_sUuid;
        std::string m_sName;
        std::string m_sDescription;
        std::string m_sState;
        std::string m_sForm;
        std::string m_sType;
        std::string m_sDateStart;
        std::string m_sDateStop;
        std::string m_sDateRestart;
        std::string m_sOrganizators;
        int m_nMaxScore;
};

#endif // MODEL_GAME_H
