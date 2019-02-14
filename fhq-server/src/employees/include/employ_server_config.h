#ifndef EMPLOY_SERVER_CONFIG_H
#define EMPLOY_SERVER_CONFIG_H

#include <employees.h>

class EmployServerConfig : public EmployBase {
    public:
        EmployServerConfig();
        static std::string name() { return "EmployServerConfig"; }
        virtual bool init();
        
        // configs
        std::string storageType();
        std::string filepathConf();
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

        std::string m_sFilepathConf;

        // database settings
        std::string m_sStorageType;
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
