#include <model_server_config.h>
#include <QFile>
#include <log.h>

ModelServerConfig::ModelServerConfig(){
	// default settings
	TAG = "ServerConfig";

    QStringList sFilenames;
    sFilenames << "conf.ini";
    sFilenames << "/etc/freehackquest-backend/conf.ini";
    sFilenames << "/etc/fhq-server/conf.ini";
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
    m_bDatabase_usemysql = true;
    m_sDatabase_host = "localhost";
	m_sDatabase_name = "freehackquest";
	m_sDatabase_user = "freehackquest_u";
	m_sDatabase_password = "freehackquest_p";

	// local nosql
    m_sDatabase_path = "/var/lib/fhq-server/data";

	m_nServer_port = 1234;
	m_bServer_ssl_on = false;
	m_nServer_ssl_port = 4613;
	m_sServer_ssl_key_file = "/etc/ssl/private/localhost.key";
	m_sServer_ssl_cert_file = "/etc/ssl/certs/localhost.pem";
};

// ---------------------------------------------------------------------

bool ModelServerConfig::load(){
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

QString ModelServerConfig::readStringFromSettings(QSettings &sett, QString settName, QString defaultValue){
	QString sResult = defaultValue;
	if(sett.contains(settName)){
		sResult = sett.value(settName, sResult).toString();
	}else{
        Log::warn(TAG, settName + " - not found in " + m_sFilename + "\n\t Will be used default value: " + defaultValue);
	}
	return sResult;
}

// ---------------------------------------------------------------------

int ModelServerConfig::readIntFromSettings(QSettings &sett, QString settName, int defaultValue){
	int nResult = defaultValue;
	if(sett.contains(settName)){
		nResult = sett.value(settName, nResult).toInt();
	}else{
        Log::warn(TAG, settName + " - not found in " + m_sFilename + "\n\t Will be used default value: " + QString::number(defaultValue));
	}
	return nResult;
}

// ---------------------------------------------------------------------

bool ModelServerConfig::readBoolFromSettings(QSettings &sett, QString settName, bool defaultValue){
	bool bResult = defaultValue;
	if(sett.contains(settName)){
		bResult = sett.value(settName, bResult).toBool();
	}else{
        Log::warn(TAG, settName + " - not found in " + m_sFilename + "\n\t Will be used default value: " + (defaultValue ? "true" : "false"));
	}
	return bResult;
}

// ---------------------------------------------------------------------

QString ModelServerConfig::databaseHost(){
	return m_sDatabase_host;
}

// ---------------------------------------------------------------------

QString ModelServerConfig::databaseName(){
	return m_sDatabase_name;
}

// ---------------------------------------------------------------------

QString ModelServerConfig::databaseUser(){
	return m_sDatabase_user;
}

// ---------------------------------------------------------------------

QString ModelServerConfig::databasePassword(){
	return m_sDatabase_password;
}

// ---------------------------------------------------------------------

bool ModelServerConfig::databaseUseMySQL(){
	return m_bDatabase_usemysql;
}

// ---------------------------------------------------------------------

QString ModelServerConfig::databasePath(){
	return m_sDatabase_path;
}

// ---------------------------------------------------------------------

bool ModelServerConfig::serverSslOn(){
	return m_bServer_ssl_on;
}

// ---------------------------------------------------------------------

int ModelServerConfig::serverPort(){
	return m_nServer_port;
}

// ---------------------------------------------------------------------

int ModelServerConfig::serverSslPort(){
	return m_nServer_ssl_port;
}

// ---------------------------------------------------------------------

QString ModelServerConfig::serverSslKeyFile(){
	return m_sServer_ssl_key_file;
}

// ---------------------------------------------------------------------

QString ModelServerConfig::serverSslCertFile(){
	return m_sServer_ssl_cert_file;
}

// ---------------------------------------------------------------------
