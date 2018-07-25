#include <model_usertoken.h>
#include <utils_logger.h>

ModelUserToken::ModelUserToken(){
    TAG = "ModelUserToken";
}

ModelUserToken::ModelUserToken(nlohmann::json const& obj){
    this->fillFromJson(obj);
    TAG = "ModelUserToken";
}

ModelUserToken::ModelUserToken(QString json){
    this->fillFromJson(nlohmann::json::parse(json.toStdString()));
    TAG = "ModelUserToken";
}

void ModelUserToken::fillFromJson(const nlohmann::json &obj){

    auto itUser = obj.find("user");
    if(itUser != obj.end()){

        m_sRole   = itUser->value("role", m_sRole);
        m_nUserID = itUser->value("id", -1);
        m_sEmail  = itUser->value("email", m_sEmail);
        m_sNick   = itUser->value("nick", m_sNick);
    }
}

bool ModelUserToken::isAdmin(){
    return m_sRole == "admin";
}

bool ModelUserToken::isUser(){
    return m_sRole == "user";
}

bool ModelUserToken::isTester(){
    return m_sRole == "tester";
}

bool ModelUserToken::hasRole(){
    return m_sRole != "";
}

QString ModelUserToken::nick(){
    return QString::fromStdString(m_sNick);
}

void ModelUserToken::setNick(QString sNick){
    m_sNick = sNick.toStdString();
}

int ModelUserToken::userid(){
    return m_nUserID;
}

QString ModelUserToken::email(){
    return QString::fromStdString(m_sEmail);
}


