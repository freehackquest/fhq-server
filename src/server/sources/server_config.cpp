#include <server_config.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <log.h>

ServerConfig::ServerConfig(){
	// default settings
	TAG = "ServerConfig";

    QStringList sFilenames;
    sFilenames << "conf.ini";
    sFilenames << "/etc/freehackquest-backend/conf.ini";
    sFilenames << "etc/freehackquest-backend/conf.ini";
    for(int i = 0; i < sFilenames.size(); i++){
        QString tmp = sFilenames[i];
        if(QFile::exists(tmp)){
            m_sFilename = tmp;
			Log::info(TAG, "Found config file " + tmp);
            break;
        }else{
			Log::warn(TAG, "Not found possible config file " + tmp);
        }
    }

	m_bServer_ssl_on = false;
	m_bDatabase_usemysql = true;

	// sql
	m_sDatabase_host = "localhost";
	m_sDatabase_name = "freehackquest";
	m_sDatabase_user = "freehackquest_u";
	m_sDatabase_password = "freehackquest_p";

	// local nosql
	m_sDatabase_path = "/var/lib/freehackquest-backend/data";

	m_nServer_port = 1234;
	m_bServer_ssl_on = false;
	m_nServer_ssl_port = 4613;
	m_sServer_ssl_key_file = "/etc/ssl/private/localhost.key";
	m_sServer_ssl_cert_file = "/etc/ssl/certs/localhost.pem";
};

// ---------------------------------------------------------------------

bool ServerConfig::load(){
    if(m_sFilename == ""){
        Log::err(TAG, "Not found config file");
		return false;
	}

    QSettings sett(m_sFilename, QSettings::IniFormat);
    m_bDatabase_usemysql = readBoolFromSettings(sett, "DATABASE/usemysql", m_bDatabase_usemysql);
    if(m_bDatabase_usemysql){
		m_sDatabase_host = readStringFromSettings(sett, "DATABASE/host", m_sDatabase_host);
		m_sDatabase_name = readStringFromSettings(sett, "DATABASE/name", m_sDatabase_name);
		m_sDatabase_user = readStringFromSettings(sett, "DATABASE/user", m_sDatabase_user);
		m_sDatabase_password = readStringFromSettings(sett, "DATABASE/password", m_sDatabase_password);

		Log::info(TAG, "Database_host: " + m_sDatabase_host);
		Log::info(TAG, "Database name: " + m_sDatabase_name);
		Log::info(TAG, "Database user: " + m_sDatabase_user);
	}else{
		m_sDatabase_path = readStringFromSettings(sett, "DATABASE/path", m_sDatabase_path);
		Log::info(TAG, "Database: using " + m_sDatabase_path);
	}

	m_nServer_port = readIntFromSettings(sett, "SERVER/port", m_nServer_port);
	m_bServer_ssl_on = readBoolFromSettings(sett, "SERVER/ssl_on", m_bServer_ssl_on);
	m_nServer_ssl_port = readIntFromSettings(sett, "SERVER/ssl_port", m_nServer_ssl_port);
	m_sServer_ssl_key_file = readStringFromSettings(sett, "SERVER/ssl_key_file", m_sServer_ssl_key_file);
	m_sServer_ssl_cert_file = readStringFromSettings(sett, "SERVER/ssl_cert_file", m_sServer_ssl_cert_file);
	return true;
}

// ---------------------------------------------------------------------

QString ServerConfig::readStringFromSettings(QSettings &sett, QString settName, QString defaultValue){
	QString sResult = defaultValue;
	if(sett.contains(settName)){
		sResult = sett.value(settName, sResult).toString();
	}else{
		Log::warn(TAG, settName + " - not found in " + m_sFilename + "\n\t Will be used default value: " + defaultValue);
	}
	return sResult;
}

// ---------------------------------------------------------------------

int ServerConfig::readIntFromSettings(QSettings &sett, QString settName, int defaultValue){
	int nResult = defaultValue;
	if(sett.contains(settName)){
		nResult = sett.value(settName, nResult).toInt();
	}else{
		Log::warn(TAG, settName + " - not found in " + m_sFilename + "\n\t Will be used default value: " + defaultValue);
	}
	return nResult;
}

// ---------------------------------------------------------------------

bool ServerConfig::readBoolFromSettings(QSettings &sett, QString settName, bool defaultValue){
	bool bResult = defaultValue;
	if(sett.contains(settName)){
		bResult = sett.value(settName, bResult).toBool();
	}else{
		Log::warn(TAG, settName + " - not found in " + m_sFilename + "\n\t Will be used default value: " + defaultValue);
	}
	return bResult;
}

// ---------------------------------------------------------------------

QString ServerConfig::databaseHost(){
	return m_sDatabase_host;
}

// ---------------------------------------------------------------------

QString ServerConfig::databaseName(){
	return m_sDatabase_name;
}

// ---------------------------------------------------------------------

QString ServerConfig::databaseUser(){
	return m_sDatabase_user;
}

// ---------------------------------------------------------------------

QString ServerConfig::databasePassword(){
	return m_sDatabase_password;
}

// ---------------------------------------------------------------------

bool ServerConfig::databaseUseMySQL(){
	return m_bDatabase_usemysql;
}

// ---------------------------------------------------------------------

QString ServerConfig::databasePath(){
	return m_sDatabase_path;
}

// ---------------------------------------------------------------------

bool ServerConfig::serverSslOn(){
	return m_bServer_ssl_on;
}

// ---------------------------------------------------------------------

int ServerConfig::serverPort(){
	return m_nServer_port;
}

// ---------------------------------------------------------------------

int ServerConfig::serverSslPort(){
	return m_nServer_ssl_port;
}

// ---------------------------------------------------------------------

QString ServerConfig::serverSslKeyFile(){
	return m_sServer_ssl_key_file;
}

// ---------------------------------------------------------------------

QString ServerConfig::serverSslCertFile(){
	return m_sServer_ssl_cert_file;
}

// ---------------------------------------------------------------------
