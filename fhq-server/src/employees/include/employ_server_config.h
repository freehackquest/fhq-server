#ifndef EMPLOY_SERVER_CONFIG_H
#define EMPLOY_SERVER_CONFIG_H

#include <employees.h>
#include <QString>
#include <QSettings>

class EmployServerConfig : public EmployBase {
    public:
        EmployServerConfig();
        static std::string name() { return "EmployServerConfig"; }
        virtual bool init();
        
        // configs
        QString databaseHost();
		QString databaseName();
		QString databaseUser();
		QString databasePassword();
		bool databaseUseMySQL();
		QString databasePath();

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
		std::string TAG;
		bool parseConfig(const std::string &sConfigFile);
		bool fileExists(const std::string &sFilename);
		void string_trim(std::string &sLine);
		std::map<std::string,std::string> m_mapConfigValues;
		
		std::string getStringValueFromConfig(const std::string &sName, const std::string &defaultValue);
		int getIntValueFromConfig(const std::string &sName, int defaultValue);
		bool getBoolValueFromConfig(const std::string &sName, bool defaultValue);
		
		QString readStringFromSettings(QSettings &sett, QString settName, QString defaultValue); // deprecated
		int readIntFromSettings(QSettings &sett, QString settName, int defaultValue); // deprecated
		bool readBoolFromSettings(QSettings &sett, QString settName, bool defaultValue); // deprecated

		// settings
        std::string m_sFilename;
        bool m_bDatabase_usemysql;
		QString m_sDatabase_host;
		QString m_sDatabase_name;
		QString m_sDatabase_user;
		QString m_sDatabase_password;
		QString m_sDatabase_path;
		
		bool m_bServer_ssl_on;
		int m_nServer_port;
		int m_nServer_ssl_port;
		QString m_sServer_ssl_key_file;
		QString m_sServer_ssl_cert_file;
};

#endif // EMPLOY_SERVER_CONFIG_H
