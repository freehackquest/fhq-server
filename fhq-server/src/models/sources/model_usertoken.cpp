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

    if(obj.find("user") != obj.end()){
        nlohmann::json user = obj.at("user");

        m_sRole   = obj.value("role", m_sRole);
        m_nUserID = obj.value("id", -1);
        m_sEmail  = obj.value("email", m_sEmail);
        m_sNick   = obj.value("nick", m_sNick);
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


