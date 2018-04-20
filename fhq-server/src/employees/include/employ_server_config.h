#ifndef EMPLOY_SERVER_CONFIG_H
#define EMPLOY_SERVER_CONFIG_H

#include <employees.h>

class EmployServerConfig : public EmployBase {
    public:
        EmployServerConfig();
        static std::string name() { return "EmployServerConfig"; }
        virtual bool init();
        
        // configs
        std::string databaseHost();
        int databasePort();
        std::string databaseName();
        std::string databaseUser();
        std::string databasePassword();
		bool databaseUseMySQL();
        std::string databasePath();

		bool serverSslOn();
		int serverPort();
		int serverSslPort();
        std::string serverSslKeyFile();
        std::string serverSslCertFile();
        
	private:
		std::string TAG;
		bool parseConfig(const std::string &sConfigFile);
		bool fileExists(const std::string &sFilename);
		void string_trim(std::string &sLine);
		std::map<std::string,std::string> m_mapConfigValues;
		
        std::string getStringValueFromConfig(const std::string &sParamName, const std::string &defaultValue);
        int getIntValueFromConfig(const std::string &sParamName, int defaultValue);
        bool getBoolValueFromConfig(const std::string &sParamName, bool defaultValue);

        std::string m_sFilename;

        // database settings
        bool m_bDatabase_usemysql;
        std::string m_sDatabase_host;
        int m_nDatabase_port;
        std::string m_sDatabase_name;
        std::string m_sDatabase_user;
        std::string m_sDatabase_password;
        std::string m_sDatabase_path;
		
        // server settings
		bool m_bServer_ssl_on;
		int m_nServer_port;
		int m_nServer_ssl_port;
        std::string m_sServer_ssl_key_file;
        std::string m_sServer_ssl_cert_file;
};

#endif // EMPLOY_SERVER_CONFIG_H
