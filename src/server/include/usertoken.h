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
        virtual void setNick(QString);
		virtual QString email();
		virtual int userid();
	private:
		QString m_sRole;
		QString m_sEmail;
		QString m_sNick;
		int m_nUserID;
		QString TAG;
};

#endif // USERTOKEN_H
