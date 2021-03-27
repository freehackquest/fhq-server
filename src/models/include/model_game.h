#ifndef MODEL_GAME_H
#define MODEL_GAME_H

#include <string>
#include <json.hpp>
#include <validators.h>

class ModelGame {
    public:

        ModelGame();
        
        int localId() const;
        void setLocalId(int nLocalId);
        const std::string &uuid() const;
        void setUuid(std::string sUuid);
        const std::string &name() const;
        void setName(std::string sName);
        const std::string &description() const;
        void setDescription(std::string sDescription);
        const std::string &state() const;
        void setState(const std::string &sState);
        const std::string &form() const;
        void setForm(std::string sForm);
        const std::string &type() const;
        void setType(std::string sType);
        const std::string &dateStart() const;
        void setDateStart(std::string sDateStart);
        const std::string &dateStop() const;
        void setDateStop(std::string sDateStop);
        const std::string &dateRestart() const;
        void setDateRestart(std::string sDateRestart);
        const std::string &organizators() const;
        void setOrganizators(std::string sOrganizators);
        int maxScore() const;
        void setMaxScore(int nMaxScore);

        void copy(const ModelGame &modelGame);
        ModelGame *clone() const;
        nlohmann::json toJson();
        void fillFrom(const nlohmann::json &jsonGame);

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
        ValidatorGameType *m_pValidatorGameType;
        ValidatorGameState *m_pValidatorGameState;
        ValidatorGameForm *m_pValidatorGameForm;
};

#endif // MODEL_GAME_H
