#ifndef USERTOKEN_H
#define USERTOKEN_H

#include <QString>
#include <QJsonObject>

class UserToken {
	public:
		UserToken();
		UserToken(QJsonObject obj);
		UserToken(QString json);

		void fillFromJson(QJsonObject obj);
		bool isAdmin();
		bool isUser();
		bool isTester();
		bool hasRole();
		QString nick();
		QString email();
		int userid();
	private:
		QString m_sRole;
		QString m_sEmail;
		QString m_sNick;
		int m_nUserID;
};

#endif // USERTOKEN_H
