#ifndef MODEL_USERTOKEN_H
#define MODEL_USERTOKEN_H

#include <iserver.h>

#include <QString>
#include <QJsonObject>

class ModelUserToken : public IUserToken {
	public:
		ModelUserToken();
		ModelUserToken(QJsonObject obj);
		ModelUserToken(QString json);

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

#endif // MODEL_USERTOKEN_H
