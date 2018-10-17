#include <model_usertoken.h>
#include <utils_logger.h>

ModelUserToken::ModelUserToken(){
    TAG = "ModelUserToken";
}

ModelUserToken::ModelUserToken(nlohmann::json const& obj) : ModelUserToken() {
    this->fillFrom(obj);
}

ModelUserToken::ModelUserToken(QString json) : ModelUserToken(){
    this->fillFrom(nlohmann::json::parse(json.toStdString()));
}

void ModelUserToken::fillFrom(const nlohmann::json &obj){
    if(obj.find("user") != obj.end()){
        nlohmann::json user = obj.at("user");
       
        // user.role
        try {
            m_sRole = user.at("role").get<std::string>();
        } catch (const std::exception &e) {
            Log::err(TAG, "JSON: " + obj.dump());
            Log::err(TAG, "Something wrong param user.role in struct. " + std::string(e.what()));
            m_sRole = "";
        }

        // TODO check allow roles

        // user.id
        try {
            m_nUserID = user.at("id").get<int>();
        } catch (const std::exception &e) {
            Log::err(TAG, "JSON: " + obj.dump());
            Log::err(TAG, "Something wrong param user.id in struct. " + std::string(e.what()));
            m_nUserID = -1;
        }
        
        // user.email
        try {
            m_sEmail = user.at("email").get<std::string>();
        } catch (const std::exception &e) {
            Log::err(TAG, "JSON: " + obj.dump());
            Log::err(TAG, "Something wrong param user.email in struct. " + std::string(e.what()));
            m_sEmail = "";
        }

        // user.nick
        try {
            m_sNick = user.at("nick").get<std::string>();
        } catch (const std::exception &e) {
            Log::err(TAG, "JSON: " + obj.dump());
            Log::err(TAG, "Something wrong param user.nick in struct. " + std::string(e.what()));
            m_sNick = "";
        }
        
    }else{
        Log::warn(TAG, "Not found param 'user' in struct");
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


