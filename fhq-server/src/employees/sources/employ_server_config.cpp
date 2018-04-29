#include <employ_server_config.h>
#include <log.h>
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
    m_nDatabase_port = 3306;
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
    vSearchConfigFile.push_back("fhq-server.conf");
    // vSearchConfigFile.push_back("/etc/freehackquest-backend/conf.ini");
    // vSearchConfigFile.push_back("/etc/fhq-server/conf.ini");
    // vSearchConfigFile.push_back("etc/freehackquest-backend/conf.ini");
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

    m_bDatabase_usemysql = this->getBoolValueFromConfig("usemysql", m_bDatabase_usemysql);
    if(m_bDatabase_usemysql){
        m_sDatabase_host = this->getStringValueFromConfig("dbhost", m_sDatabase_host);
        m_nDatabase_port = this->getIntValueFromConfig("dbport", m_nDatabase_port);
        m_sDatabase_name = this->getStringValueFromConfig("dbname", m_sDatabase_name);
        m_sDatabase_user = this->getStringValueFromConfig("dbuser", m_sDatabase_user);
        m_sDatabase_password = this->getStringValueFromConfig("dbpass", m_sDatabase_password);

        Log::info(TAG, "Database host: " + m_sDatabase_host);
        Log::info(TAG, "Database port: " + std::to_string(m_nDatabase_port));
        Log::info(TAG, "Database name: " + m_sDatabase_name);
        Log::info(TAG, "Database user: " + m_sDatabase_user);
	}else{
        m_sDatabase_path = this->getStringValueFromConfig("dbpath", m_sDatabase_path);
        Log::info(TAG, "Database: using " + m_sDatabase_path);
	}

    m_nServer_port = this->getIntValueFromConfig("port", m_nServer_port);
    m_bServer_ssl_on = this->getBoolValueFromConfig("ssl_on", m_bServer_ssl_on);
    if(m_bServer_ssl_on){
        m_nServer_ssl_port = this->getIntValueFromConfig("SERVER/ssl_port", m_nServer_ssl_port);
        m_sServer_ssl_key_file = this->getStringValueFromConfig("ssl_key_file", m_sServer_ssl_key_file);
        m_sServer_ssl_cert_file = this->getStringValueFromConfig("ssl_cert_file", m_sServer_ssl_cert_file);
    }
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

std::string EmployServerConfig::getStringValueFromConfig(const std::string &sParamName, const std::string &defaultValue){
	std::string sResult = defaultValue;

    if(m_mapConfigValues.count(sParamName)){
        sResult = m_mapConfigValues.at(sParamName);
    }else{
        Log::warn(TAG, sParamName + " - not found in " + m_sFilename + "\n\t Will be used default value: " + defaultValue);
    }
	return sResult;
}

// ---------------------------------------------------------------------

int EmployServerConfig::getIntValueFromConfig(const std::string &sParamName, int defaultValue){
    int nResult = defaultValue;
    if(m_mapConfigValues.count(sParamName)){
        std::string sParamValue = m_mapConfigValues.at(sParamName);
        std::istringstream isBuffer(sParamValue);
        isBuffer >> nResult;
    }else{
        Log::warn(TAG, sParamName + " - not found in " + m_sFilename + "\n\t Will be used default value: " + std::to_string(defaultValue));
    }
    return nResult;
}

// ---------------------------------------------------------------------

bool EmployServerConfig::getBoolValueFromConfig(const std::string &sParamName, bool defaultValue){
    bool bResult = defaultValue;

    if(m_mapConfigValues.count(sParamName)){
        std::string sParamValue = m_mapConfigValues.at(sParamName);
        std::transform(sParamValue.begin(), sParamValue.end(), sParamValue.begin(), ::tolower);
        bResult = (sParamValue == "yes" || sParamValue == "no");
    }else{
        Log::warn(TAG, sParamName + " - not found in " + m_sFilename + "\n\t Will be used default value: " + (defaultValue ? "yes" : "no"));
    }
    return bResult;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::databaseHost(){
	return m_sDatabase_host;
}

// ---------------------------------------------------------------------

int EmployServerConfig::databasePort(){
    return m_nDatabase_port;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::databaseName(){
	return m_sDatabase_name;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::databaseUser(){
	return m_sDatabase_user;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::databasePassword(){
	return m_sDatabase_password;
}

// ---------------------------------------------------------------------

bool EmployServerConfig::databaseUseMySQL(){
	return m_bDatabase_usemysql;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::databasePath(){
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

std::string EmployServerConfig::serverSslKeyFile(){
	return m_sServer_ssl_key_file;
}

// ---------------------------------------------------------------------

std::string EmployServerConfig::serverSslCertFile(){
	return m_sServer_ssl_cert_file;
}

// ---------------------------------------------------------------------
