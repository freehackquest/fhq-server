#include "../headers/usertoken.h"
#include <log.h>
#include <QJsonDocument>
#include <QJsonObject>

UserToken::UserToken(){
	TAG = "UserToken";
};

UserToken::UserToken(QJsonObject obj){
	this->fillFromJson(obj);
	TAG = "UserToken";
};

UserToken::UserToken(QString json){
	QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
	this->fillFromJson(doc.object());
	TAG = "UserToken";
};

void UserToken::fillFromJson(QJsonObject obj){
	
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

bool UserToken::isAdmin(){
	return m_sRole == "admin";
}

bool UserToken::isUser(){
	return m_sRole == "user";
}

bool UserToken::isTester(){
	return m_sRole == "tester";
}

bool UserToken::hasRole(){
	return m_sRole != "";
}

QString UserToken::nick(){
	return m_sNick;
}

void UserToken::setNick(QString sNick){
    m_sNick = sNick;
}

int UserToken::userid(){
	return m_nUserID;
}

QString UserToken::email(){
	return m_sEmail;
}


