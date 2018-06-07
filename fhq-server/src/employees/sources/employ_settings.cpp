#include <employ_settings.h>
#include <employ_database.h>
#include <log.h>
#include <QTextStream>
#include <QFile>
#include <QByteArray>
#include <QDateTime>
#include <QDir>

REGISTRY_EMPLOY(EmploySettings)

// ---------------------------------------------------------------------

EmploySettings::EmploySettings()
    : EmployBase(EmploySettings::name(), {EmployDatabase::name()}){
    TAG = EmploySettings::name();
}

// ---------------------------------------------------------------------

bool EmploySettings::init(){
	Log::info(TAG, "Start init settings");

    std::string sGroupProfile = "profile";
    addNewSetting(new ModelServerSettHelper(sGroupProfile, "profile_change_nick", true));

    std::string sGroupMail = "mail";
    addNewSetting(new ModelServerSettHelper(sGroupMail, "mail_from", QString("freehackquest@gmail.com")));
    addNewSetting(new ModelServerSettHelper(sGroupMail, "mail_host", QString("smtp.gmail.com")));
    addNewSetting(new ModelServerSettHelper(sGroupMail, "mail_port", 465));
    addNewSetting(new ModelServerSettHelper(sGroupMail, "mail_username", QString("freehackquest@gmail.com")));
    addNewSetting(new ModelServerSettHelper(sGroupMail, "mail_password", QString("some"), true));
    addNewSetting(new ModelServerSettHelper(sGroupMail, "mail_auth", true));
    addNewSetting(new ModelServerSettHelper(sGroupMail, "mail_allow", true));
    addNewSetting(new ModelServerSettHelper(sGroupMail, "mail_system_message_admin_email", QString("")));

    // Google Map API
    std::string sGroupGoogleMap = "google_map";
    addNewSetting(new ModelServerSettHelper(sGroupGoogleMap, "google_map_api_key", QString("some")));

    // server folders
    std::string sGroupServerFolders = "server_folders";
    addNewSetting(new ModelServerSettHelper(sGroupServerFolders, "server_folder_games", QString("/var/www/html/fhq/files/games/")));
    addNewSetting(new ModelServerSettHelper(sGroupServerFolders, "server_folder_games_url", QString("https://freehackquest.com/files/games/")));
    addNewSetting(new ModelServerSettHelper(sGroupServerFolders, "server_folder_quests", QString("/var/www/html/fhq/files/quests/")));
    addNewSetting(new ModelServerSettHelper(sGroupServerFolders, "server_folder_quests_url", QString("https://freehackquest.com/files/quests/")));
    addNewSetting(new ModelServerSettHelper(sGroupServerFolders, "server_folder_users", QString("/var/www/html/fhq/files/quests/")));
    addNewSetting(new ModelServerSettHelper(sGroupServerFolders, "server_folder_users_url", QString("https://freehackquest.com/files/quests/")));

    // LXD
    std::string  sGroupLXD = "lxd";
    addNewSetting(new ModelServerSettHelper(sGroupLXD, "path_dir_lxc_ssl", QString("/etc/fhq-server/lxd")));
    addNewSetting(new ModelServerSettHelper(sGroupLXD, "lxd_server_ip", QString("127.0.0.1")));
    addNewSetting(new ModelServerSettHelper(sGroupLXD, "lxd_server_port", QString("8443")));
    addNewSetting(new ModelServerSettHelper(sGroupLXD, "lxd_mode", QString("disabled")));

    std::vector<std::string> vFoundInDatabase;

	EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());

    // load from database
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM settings");
        query.exec();
        while (query.next()) {
            QSqlRecord record = query.record();
            std::string sName = record.value("name").toString().toStdString();
            QString sValue = record.value("value").toString();
            std::string sType = record.value("type").toString().toStdString();
            std::string sGroup = record.value("group").toString().toStdString();

            vFoundInDatabase.push_back(sName);

            if(m_mapSettings.count(sName)){
                ModelServerSettHelper *pServerSettHelper = m_mapSettings[sName];
                if(sType != pServerSettHelper->type()){
                    Log::err(TAG, "Wrong type for setting '" + sName + "' (expected '" + pServerSettHelper->type() + "', but got: '" + sType + "'");
                    // TODO change type of setting or remove
                }else{
                    if(pServerSettHelper->isString()){
                        pServerSettHelper->setValue(sValue);
                    }else if(pServerSettHelper->isBoolean()){
                        pServerSettHelper->setValue(sValue == "yes");
                    }else if(pServerSettHelper->isInteger()){
                        // TODO check convertation string to int
                        pServerSettHelper->setValue(sValue.toInt());
                    }else if(pServerSettHelper->isPassword()){
                        pServerSettHelper->setValue(sValue);
                    }else{
                        Log::err(TAG, "No handle type for setting '" + sName + "'");
                    }
                }
            }else{
                Log::warn(TAG, "Undefined settings name in database: " + sName);
            }
        }
    }

    // check string settings in database
    std::map<std::string, ModelServerSettHelper*>::iterator it = m_mapSettings.begin();
    for (; it!=m_mapSettings.end(); ++it){
        std::string sName = it->first;
        
        if(std::find(vFoundInDatabase.begin(), vFoundInDatabase.end(), sName) == vFoundInDatabase.end()) {
			// not found in database
			ModelServerSettHelper *pServerSettHelper = m_mapSettings.at(sName);
            initSettingDatabase(pServerSettHelper);
		}
    }
    return true;
}

// ---------------------------------------------------------------------

void EmploySettings::addNewSetting(ModelServerSettHelper* pServerSettHelper){
    std::string sName = pServerSettHelper->name();
    if(!m_mapSettings.count(sName)){
        m_mapSettings.insert(std::pair<std::string, ModelServerSettHelper*>(sName,pServerSettHelper));
    }else{
        Log::warn(TAG, "Duplicate setting '" + sName + "'. Skip");
    }
}

// ---------------------------------------------------------------------

QString EmploySettings::getSettString(const std::string &sName){
    QMutexLocker locker (&m_mtxServerSettings);
    QString sResult = "";
    if(m_mapSettings.count(sName)){
        ModelServerSettHelper* pServerSettHelper = m_mapSettings.at(sName);
        if(!pServerSettHelper->isString()){
            Log::err(TAG, "Wrong type setting string (get): " + sName);
        }else{
            sResult = pServerSettHelper->valueAsString();
        }
    }else{
        Log::err(TAG, "Not found server setting string (get): " + sName);
    }
    return sResult;
}

// ---------------------------------------------------------------------

void EmploySettings::setSettString(const std::string &sName, QString sValue){
    QMutexLocker locker (&m_mtxServerSettings);
    if(m_mapSettings.count(sName)){
        ModelServerSettHelper* pServerSettHelper = m_mapSettings.at(sName);
        if(!pServerSettHelper->isString()){
            Log::err(TAG, "Wrong type setting string (set): " + sName);
        }else{
            pServerSettHelper->setValue(sValue);
            updateSettingDatabase(pServerSettHelper);
        }
    }else{
        Log::err(TAG, "Not found server setting string (set): " + sName);
    }
}

// ---------------------------------------------------------------------

QString EmploySettings::getSettPassword(const std::string &sName){
    QMutexLocker locker (&m_mtxServerSettings);
    QString sResult = "";
    if(m_mapSettings.count(sName)){
        ModelServerSettHelper* pServerSettHelper = m_mapSettings.at(sName);
        if(!pServerSettHelper->isPassword()){
            Log::err(TAG, "Wrong type setting password (get): " + sName);
        }else{
            sResult = pServerSettHelper->valueAsString();
        }
    }else{
        Log::err(TAG, "Not found server setting password (get): " + sName);
    }
    return sResult;
}

// ---------------------------------------------------------------------

void EmploySettings::setSettPassword(const std::string &sName, QString sValue){
    QMutexLocker locker (&m_mtxServerSettings);
    if(m_mapSettings.count(sName)){
        ModelServerSettHelper* pServerSettHelper = m_mapSettings.at(sName);
        if(!pServerSettHelper->isPassword()){
            Log::err(TAG, "Wrong type setting string (set): " + sName);
        }else{
            pServerSettHelper->setValue(sValue);
            updateSettingDatabase(pServerSettHelper);
        }
    }else{
        Log::err(TAG, "Not found server setting string (set): " + sName);
    }
}

// ---------------------------------------------------------------------

int EmploySettings::getSettInteger(const std::string &sName){
    QMutexLocker locker (&m_mtxServerSettings);
    int nResult = 0;
    if(m_mapSettings.count(sName)){
        ModelServerSettHelper* pServerSettHelper = m_mapSettings.at(sName);
        if(!pServerSettHelper->isInteger()){
            Log::err(TAG, "Wrong type setting integer (get): " + sName);
        }else{
            nResult = pServerSettHelper->valueAsInteger();
        }
    }else{
        Log::err(TAG, "Not found server setting integer (get): " + sName);
    }
    return nResult;
}

// ---------------------------------------------------------------------

void EmploySettings::setSettInteger(const std::string &sName, int nValue){
    QMutexLocker locker (&m_mtxServerSettings);
    if(m_mapSettings.count(sName)){
        ModelServerSettHelper* pServerSettHelper = m_mapSettings.at(sName);
        if(!pServerSettHelper->isInteger()){
            Log::err(TAG, "Wrong type setting integer (set): " + sName);
        }else{
            pServerSettHelper->setValue(nValue);
            updateSettingDatabase(pServerSettHelper);
        }
    }else{
        Log::err(TAG, "Not found server setting integer (set): " + sName);
    }
}

// ---------------------------------------------------------------------

bool EmploySettings::getSettBoolean(const std::string &sName){
    QMutexLocker locker (&m_mtxServerSettings);
    bool bResult = false;
    if(m_mapSettings.count(sName)){
        ModelServerSettHelper* pServerSettHelper = m_mapSettings.at(sName);
        if(!pServerSettHelper->isBoolean()){
            Log::err(TAG, "Wrong type setting boolean (get): " + sName);
        }else{
            bResult = pServerSettHelper->valueAsBoolean();
        }
    }else{
        Log::err(TAG, "Not found server setting boolean (get): " + sName);
    }
    return bResult;
}

// ---------------------------------------------------------------------

void EmploySettings::setSettBoolean(const std::string &sName, bool bValue){
    QMutexLocker locker (&m_mtxServerSettings);
    if(m_mapSettings.count(sName)){
        ModelServerSettHelper* pServerSettHelper = m_mapSettings.at(sName);
        if(!pServerSettHelper->isBoolean()){
            Log::err(TAG, "Wrong type setting boolean (set): " + sName);
        }else{
            pServerSettHelper->setValue(bValue);
            updateSettingDatabase(pServerSettHelper);
        }
    }else{
        Log::err(TAG, "Not found server setting integer (set): " + sName);
    }
}

// ---------------------------------------------------------------------

bool EmploySettings::hasSett(const std::string &sName){
    return m_mapSettings.count(sName) != 0;
}

// ---------------------------------------------------------------------

const std::string &EmploySettings::getSettType(const std::string &sName){
    if(m_mapSettings.count(sName)){
        return m_mapSettings.at(sName)->type();
    }
    return SETT_TYPE_UNKNOWN;
}

// ---------------------------------------------------------------------

const std::map<std::string, ModelServerSettHelper*> &EmploySettings::listSettings(){
	return m_mapSettings;
}

// ---------------------------------------------------------------------

void EmploySettings::printSettings(){
	std::map<std::string, ModelServerSettHelper*>::iterator it = m_mapSettings.begin();
    for (; it!=m_mapSettings.end(); ++it){
        std::string sName = it->first;
        ModelServerSettHelper *pServerSettHelper = it->second;

		std::cout << " * [" << pServerSettHelper->name() << "] => [";
        if(pServerSettHelper->isBoolean()){
			std::cout << pServerSettHelper->valueAsBoolean() ? "true" : "false";
        }else if(pServerSettHelper->isString()){
			std::cout << pServerSettHelper->valueAsString().toStdString();
        }else if(pServerSettHelper->isInteger()){
			std::cout << pServerSettHelper->valueAsInteger();
        }else if(pServerSettHelper->isPassword()){
			std::cout << pServerSettHelper->valueAsString().toStdString();
        }else{
			std::cout << pServerSettHelper->valueAsString().toStdString();
        }
        std::cout << "]\n";

        // jsonSett["group"] = pServerSettHelper->group();
        // jsonSett["type"] = pServerSettHelper->type();
    }
}
// ---------------------------------------------------------------------

nlohmann::json EmploySettings::toJson(){
    auto jsonSettings = nlohmann::json::array();

    std::map<std::string, ModelServerSettHelper*>::iterator it = m_mapSettings.begin();
    for (; it!=m_mapSettings.end(); ++it){
        std::string sName = it->first;
        ModelServerSettHelper *pServerSettHelper = it->second;

        nlohmann::json jsonSett;
        jsonSett["name"] = pServerSettHelper->name();
        if(pServerSettHelper->isBoolean()){
            jsonSett["value"] = pServerSettHelper->valueAsBoolean();
        }else if(pServerSettHelper->isString()){
            jsonSett["value"] = pServerSettHelper->valueAsString().toStdString();
        }else if(pServerSettHelper->isInteger()){
            jsonSett["value"] = pServerSettHelper->valueAsInteger();
        }else if(pServerSettHelper->isPassword()){
            jsonSett["value"] = "******";
        }else{
            jsonSett["value"] = pServerSettHelper->valueAsString().toStdString();
        }

        jsonSett["group"] = pServerSettHelper->group();
        jsonSett["type"] = pServerSettHelper->type();
        jsonSettings.push_back(jsonSett);
    }
    return jsonSettings;
}

// ---------------------------------------------------------------------

void EmploySettings::initSettingDatabase(ModelServerSettHelper *pServerSettHelper){
    Log::info(TAG, "Init settings to database: " + pServerSettHelper->name());
    EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("INSERT INTO settings (`name`, `value`, `group`, `type`) VALUES (:name, :value, :group, :type)");
    query.bindValue(":name", QString(pServerSettHelper->name().c_str()));
    query.bindValue(":value", pServerSettHelper->valueAsString());
    query.bindValue(":group", QString(pServerSettHelper->group().c_str()));
    query.bindValue(":type", QString(pServerSettHelper->type().c_str()));
    if(!query.exec()){
        Log::err(TAG, query.lastError().text().toStdString());
    }
}

// ---------------------------------------------------------------------

void EmploySettings::updateSettingDatabase(ModelServerSettHelper *pServerSettHelper){
	EmployDatabase *pDatabase = findEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    QSqlQuery query(db);
    query.prepare("UPDATE settings SET value = :value WHERE name = :name");
    query.bindValue(":value", pServerSettHelper->valueAsString());
    query.bindValue(":name", QString(pServerSettHelper->name().c_str()));
    if(!query.exec()){
        Log::err(TAG, query.lastError().text().toStdString());
    }
}

// ---------------------------------------------------------------------

