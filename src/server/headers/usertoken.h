#ifndef USERTOKEN_H
#define USERTOKEN_H

#include <iserver.h>

#include <QString>
#include <QJsonObject>

class UserToken : public IUserToken {
	public:
		UserToken();
		UserToken(QJsonObject obj);
		UserToken(QString json);

		// IUserToken
		virtual void fillFromJson(QJsonObject obj);
		virtual bool isAdmin();
		virtual bool isUser();
		virtual bool isTester();
		virtual bool hasRole();
		virtual QString nick();
		virtual QString email();
		virtual int userid();
	private:
		QString m_sRole;
		QString m_sEmail;
		QString m_sNick;
		int m_nUserID;
};

#endif // USERTOKEN_H
