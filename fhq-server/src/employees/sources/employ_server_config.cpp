#include <employ_server_config.h>
#include <log.h>
#include <QFile>
#include <sys/stat.h>
#include <fstream>
#include <regex>        // regex, sregex_token_iterator

REGISTRY_EMPLOY(EmployServerConfig)

// ---------------------------------------------------------------------

EmployServerConfig::EmployServerConfig()
    : EmployBase(EmployServerConfig::name(), {}){
	
	TAG = EmployServerConfig::name();
	
	// default settings
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
}

// ---------------------------------------------------------------------

bool EmployServerConfig::init(){
	
	std::vector<std::string> vSearchConfigFile;
	vSearchConfigFile.push_back("conf.ini");
	vSearchConfigFile.push_back("/etc/freehackquest-backend/conf.ini");
	vSearchConfigFile.push_back("/etc/fhq-server/conf.ini");
	vSearchConfigFile.push_back("etc/freehackquest-backend/conf.ini");
	vSearchConfigFile.push_back("/etc/fhq-server/fhq-server.conf");

    for(int i = 0; i < vSearchConfigFile.size(); i++){
        std::string tmp = vSearchConfigFile[i];
        if(this->fileExists(tmp)){
            m_sFilename = tmp;
			Log::info(TAG, "Found config file " + tmp);
            break;
        }else{
			Log::warn(TAG, "Not found possible config file " + tmp);
        }
    }
    
    if(m_sFilename == ""){
        Log::err(TAG, "Not found config file");
		return false;
	}

	EmployServerConfig::parseConfig(m_sFilename);

    QSettings sett(QString(m_sFilename.c_str()), QSettings::IniFormat);

    m_bDatabase_usemysql = readBoolFromSettings(sett, "DATABASE/usemysql", m_bDatabase_usemysql);
    if(m_bDatabase_usemysql){
		m_sDatabase_host = readStringFromSettings(sett, "DATABASE/host", m_sDatabase_host);
		m_sDatabase_name = readStringFromSettings(sett, "DATABASE/name", m_sDatabase_name);
		m_sDatabase_user = readStringFromSettings(sett, "DATABASE/user", m_sDatabase_user);
		m_sDatabase_password = readStringFromSettings(sett, "DATABASE/password", m_sDatabase_password);

		Log::info(TAG, "Database_host: " + m_sDatabase_host.toStdString());
		Log::info(TAG, "Database name: " + m_sDatabase_name.toStdString());
		Log::info(TAG, "Database user: " + m_sDatabase_user.toStdString());
	}else{
		m_sDatabase_path = readStringFromSettings(sett, "DATABASE/path", m_sDatabase_path);
		Log::info(TAG, "Database: using " + m_sDatabase_path.toStdString());
	}

	m_nServer_port = readIntFromSettings(sett, "SERVER/port", m_nServer_port);
	m_bServer_ssl_on = readBoolFromSettings(sett, "SERVER/ssl_on", m_bServer_ssl_on);
	m_nServer_ssl_port = readIntFromSettings(sett, "SERVER/ssl_port", m_nServer_ssl_port);
	m_sServer_ssl_key_file = readStringFromSettings(sett, "SERVER/ssl_key_file", m_sServer_ssl_key_file);
	m_sServer_ssl_cert_file = readStringFromSettings(sett, "SERVER/ssl_cert_file", m_sServer_ssl_cert_file);
    return true;
}

// ---------------------------------------------------------------------

bool EmployServerConfig::parseConfig(const std::string &sConfigFile){
	std::ifstream isConfigFile( sConfigFile );
	int nLineNumber = 0;
	for( std::string sLine; getline( isConfigFile, sLine ); ){
		nLineNumber++;
		std::string sOrigLine = sLine;
		std::size_t nFoundComment = sLine.find("#");
		if (nFoundComment != std::string::npos){
			// remove all after #
			sLine.erase (sLine.begin() + nFoundComment, sLine.end());
		}
		this->string_trim(sLine);
		if(sLine == ""){ // skip empty strings
			continue;
		}
		
		// std::cout << "Line (" << nLineNumber << "): [" << sLine << "]" << std::endl;
		
		std::size_t nFoundEqualChar = sLine.find("=");
		if (nFoundEqualChar != std::string::npos){
			// split name of param and value
			std::string sParamName = sLine;
			std::string sParamValue = sLine;
			
			sParamName.erase (sParamName.begin() + nFoundEqualChar, sParamName.end());
			sParamValue.erase (sParamValue.begin(), sParamValue.begin() + nFoundEqualChar + 1);
			this->string_trim(sParamName);
			this->string_trim(sParamValue);
			
			// std::cout << " [" << sParamName << "]  => [" << sParamValue << "]" << std::endl;
			
			if(m_mapConfigValues.count(sParamName)){
				Log::warn(TAG, "Ignoring duplicate of option line(" + std::to_string(nLineNumber) + ") in config: " + sConfigFile);
			}else{
				m_mapConfigValues.insert(std::pair<std::string,std::string>(sParamName, sParamValue));	
			}
		}else{
			Log::warn(TAG, "Ignoring invalid line(" + std::to_string(nLineNumber) + ") in config: " + sConfigFile);
		}
		
	}
	return true;
}

// ---------------------------------------------------------------------

void EmployServerConfig::string_trim(std::string &sLine){
	// trim trailing spaces
	std::size_t endpos = sLine.find_last_not_of(" \t");
	std::size_t startpos = sLine.find_first_not_of(" \t");
	if( std::string::npos != endpos ) {
		sLine = sLine.substr( 0, endpos+1 );
		sLine = sLine.substr( startpos );
	} else {
		sLine.erase(std::remove(std::begin(sLine), std::end(sLine), ' '), std::end(sLine));
	}

	// trim leading spaces
	std::size_t nStartPos = sLine.find_first_not_of(" \t");
	if( std::string::npos != nStartPos ) {
		sLine = sLine.substr( nStartPos );
	}
}

// ---------------------------------------------------------------------

bool EmployServerConfig::fileExists(const std::string &sFilename){
	struct stat buffer;   
	return (stat (sFilename.c_str(), &buffer) == 0); 
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::getStringValueFromConfig(const std::string &sName, const std::string &defaultValue){
	std::string sResult = defaultValue;
	/*if(sett.contains(settName)){
		sResult = sett.value(settName, sResult).toString();
	}else{
        Log::warn(TAG, settName.toStdString() + " - not found in " + m_sFilename + "\n\t Will be used default value: " + defaultValue.toStdString());
	}*/
	// TODO
	return sResult;
}

// ---------------------------------------------------------------------

int EmployServerConfig::getIntValueFromConfig(const std::string &sName, int defaultValue){
}

// ---------------------------------------------------------------------

bool EmployServerConfig::getBoolValueFromConfig(const std::string &sName, bool defaultValue){
}

// ---------------------------------------------------------------------

// deprecated
QString EmployServerConfig::readStringFromSettings(QSettings &sett, QString settName, QString defaultValue){
	QString sResult = defaultValue;
	if(sett.contains(settName)){
		sResult = sett.value(settName, sResult).toString();
	}else{
        Log::warn(TAG, settName.toStdString() + " - not found in " + m_sFilename + "\n\t Will be used default value: " + defaultValue.toStdString());
	}
	return sResult;
}

// ---------------------------------------------------------------------

// deprecated
int EmployServerConfig::readIntFromSettings(QSettings &sett, QString settName, int defaultValue){
	int nResult = defaultValue;
	if(sett.contains(settName)){
		nResult = sett.value(settName, nResult).toInt();
	}else{
        Log::warn(TAG, settName.toStdString() + " - not found in " + m_sFilename + "\n\t Will be used default value: " + QString::number(defaultValue).toStdString());
	}
	return nResult;
}

// ---------------------------------------------------------------------

// deprecated
bool EmployServerConfig::readBoolFromSettings(QSettings &sett, QString settName, bool defaultValue){
	bool bResult = defaultValue;
	if(sett.contains(settName)){
		bResult = sett.value(settName, bResult).toBool();
	}else{
        Log::warn(TAG, settName.toStdString() + " - not found in " + m_sFilename + "\n\t Will be used default value: " + (defaultValue ? "true" : "false"));
	}
	return bResult;
}

// ---------------------------------------------------------------------

QString EmployServerConfig::databaseHost(){
	return m_sDatabase_host;
}

// ---------------------------------------------------------------------

QString EmployServerConfig::databaseName(){
	return m_sDatabase_name;
}

// ---------------------------------------------------------------------

QString EmployServerConfig::databaseUser(){
	return m_sDatabase_user;
}

// ---------------------------------------------------------------------

QString EmployServerConfig::databasePassword(){
	return m_sDatabase_password;
}

// ---------------------------------------------------------------------

bool EmployServerConfig::databaseUseMySQL(){
	return m_bDatabase_usemysql;
}

// ---------------------------------------------------------------------

QString EmployServerConfig::databasePath(){
	return m_sDatabase_path;
}

// ---------------------------------------------------------------------

bool EmployServerConfig::serverSslOn(){
	return m_bServer_ssl_on;
}

// ---------------------------------------------------------------------

int EmployServerConfig::serverPort(){
	return m_nServer_port;
}

// ---------------------------------------------------------------------

int EmployServerConfig::serverSslPort(){
	return m_nServer_ssl_port;
}

// ---------------------------------------------------------------------

QString EmployServerConfig::serverSslKeyFile(){
	return m_sServer_ssl_key_file;
}

// ---------------------------------------------------------------------

QString EmployServerConfig::serverSslCertFile(){
	return m_sServer_ssl_cert_file;
}

// ---------------------------------------------------------------------
