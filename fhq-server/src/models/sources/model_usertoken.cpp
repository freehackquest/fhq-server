#include <model_usertoken.h>
#include <log.h>
#include <QJsonDocument>
#include <QJsonObject>

ModelUserToken::ModelUserToken(){
	TAG = "ModelUserToken";
}

ModelUserToken::ModelUserToken(QJsonObject obj){
	this->fillFromJson(obj);
    TAG = "ModelUserToken";
}

ModelUserToken::ModelUserToken(QString json){
	QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
	this->fillFromJson(doc.object());
    TAG = "ModelUserToken";
}

void ModelUserToken::fillFromJson(QJsonObject obj){
	
	if(obj.contains("user")){
		QJsonObject user = obj["user"].toObject();
		if(user.contains("role")){
			m_sRole = user["role"].toString();
		}
		
		if(user.contains("id")){
			m_nUserID = user["id"].toString().toInt();
		}else{
			m_nUserID = -1;
		}
		
		if(user.contains("email")){
			m_sEmail = user["email"].toString();
		}
		
		if(user.contains("nick")){
			m_sNick = user["nick"].toString();
		}
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
	return m_sNick;
}

void ModelUserToken::setNick(QString sNick){
    m_sNick = sNick;
}

int ModelUserToken::userid(){
	return m_nUserID;
}

QString ModelUserToken::email(){
	return m_sEmail;
}


