#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <iserver.h>

#include <QString>
#include <QJsonObject>
#include <QSettings>

class ServerConfig {
	public:
		ServerConfig();

		bool load(); 

		QString databaseHost();
		QString databaseName();
		QString databaseUser();
		QString databasePassword();

		QString emailUsername();
		QString emailPassword();
		QString emailSmtpHost();
		int emailSmtpPort();

		bool serverSslOn();
		int serverPort();
		int serverSslPort();
		QString serverSslKeyFile();
		QString serverSslCertFile();
		
	private:

		QString readStringFromSettings(QSettings &sett, QString settName, QString defaultValue);
		int readIntFromSettings(QSettings &sett, QString settName, int defaultValue);
		bool readBoolFromSettings(QSettings &sett, QString settName, bool defaultValue);

		// settings
        QString m_sFilename;
		QString m_sDatabase_host;
		QString m_sDatabase_name;
		QString m_sDatabase_user;
		QString m_sDatabase_password;
		
		QString m_sEmail_smtphost;
		int m_nEmail_smtpport;
		QString m_sEmail_username;
		QString m_sEmail_password;
		
		bool m_bServer_ssl_on;
		int m_nServer_port;
		int m_nServer_ssl_port;
		QString m_sServer_ssl_key_file;
		QString m_sServer_ssl_cert_file;
		
};

#endif // SERVER_CONFIG_H
