#ifndef MODEL_USERTOKEN_H
#define MODEL_USERTOKEN_H

#include <iserver.h>
#include <json.hpp>
#include <QString>

class ModelUserToken : public IUserToken {
    public:
        ModelUserToken();
        ModelUserToken(nlohmann::json const& obj);
        ModelUserToken(QString json);

        // IUserToken
        virtual void fillFromJson(nlohmann::json const& obj) override;
        virtual bool isAdmin();
        virtual bool isUser();
        virtual bool isTester();
        virtual bool hasRole();
        virtual QString nick();
        virtual void setNick(QString);
        virtual QString email();
        virtual int userid();
    private:
        std::string m_sRole;
        std::string m_sEmail;
        std::string m_sNick;
        int m_nUserID;
        QString TAG;
};

#endif // MODEL_USERTOKEN_H
