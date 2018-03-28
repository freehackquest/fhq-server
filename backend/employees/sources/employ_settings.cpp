#include <employ_settings.h>

REGISTRY_EMPLOY(EmploySettings)

// ---------------------------------------------------------------------

#include <log.h>

#include <QTextStream>
#include <QFile>
#include <QByteArray>
#include <QDateTime>
#include <QDir>

// ---------------------------------------------------------------------

void EmploySettings::test(){

}

// ---------------------------------------------------------------------

void EmploySettings::initSettings(IWebSocketServer *pWebSocketServer){
    TAG = "MemoryCacheServerSettings";

    Log::info(TAG, "Start init settings");
    m_pWebSocketServer = pWebSocketServer;


    QString sGroupProfile = "profile";
    addNewSetting(new ServerSettHelper(sGroupProfile, "profile_change_nick", true));

    QString sGroupMail = "mail";
    addNewSetting(new ServerSettHelper(sGroupMail, "mail_from", QString("fhqdebug@gmail.com")));
    addNewSetting(new ServerSettHelper(sGroupMail, "mail_host", QString("smtp.gmail.com")));
    addNewSetting(new ServerSettHelper(sGroupMail, "mail_port", 465));
    addNewSetting(new ServerSettHelper(sGroupMail, "mail_username", QString("fhqdebug@gmail.com")));
    addNewSetting(new ServerSettHelper(sGroupMail, "mail_password", QString("Friday-73"), true));
    addNewSetting(new ServerSettHelper(sGroupMail, "mail_auth", true));
    addNewSetting(new ServerSettHelper(sGroupMail, "mail_allow", true));
    addNewSetting(new ServerSettHelper(sGroupMail, "mail_system_message_admin_email", QString("")));

    // Google Map API
    QString sGroupGoogleMap = "google_map";
    addNewSetting(new ServerSettHelper(sGroupGoogleMap, "google_map_api_key", QString("some")));

    // server folders
    QString sGroupServerFolders = "server_folders";
    addNewSetting(new ServerSettHelper(sGroupServerFolders, "server_folder_games", QString("/var/www/html/fhq/files/games/")));
    addNewSetting(new ServerSettHelper(sGroupServerFolders, "server_folder_games_url", QString("https://freehackquest.com/files/games/")));
    addNewSetting(new ServerSettHelper(sGroupServerFolders, "server_folder_quests", QString("/var/www/html/fhq/files/quests/")));
    addNewSetting(new ServerSettHelper(sGroupServerFolders, "server_folder_quests_url", QString("https://freehackquest.com/files/quests/")));
    addNewSetting(new ServerSettHelper(sGroupServerFolders, "server_folder_users", QString("/var/www/html/fhq/files/quests/")));
    addNewSetting(new ServerSettHelper(sGroupServerFolders, "server_folder_users_url", QString("https://freehackquest.com/files/quests/")));

    // LXD
    QString sGroupLXD = "lxd";
    addNewSetting(new ServerSettHelper(sGroupLXD, "path_dir_lxc_ssl", QString("/home/fhq/.config/lxc")));
    addNewSetting(new ServerSettHelper(sGroupLXD, "lxd_server_ip", QString("127.0.0.1")));
    addNewSetting(new ServerSettHelper(sGroupLXD, "lxd_server_port", QString("8443")));
    addNewSetting(new ServerSettHelper(sGroupLXD, "lxd_passwd", QString("freehackquest")));

    QStringList listFoundInDatabase;

    QSqlDatabase db = *(pWebSocketServer->database());

    // load from database
    {
        QSqlQuery query(db);
        query.prepare("SELECT * FROM settings");
        query.exec();
        while (query.next()) {
            QSqlRecord record = query.record();
            QString sName = record.value("name").toString();
            QString sValue = record.value("value").toString();
            QString sType = record.value("type").toString();
            QString sGroup = record.value("group").toString();

            listFoundInDatabase << sName;

            if(m_mapSettings.contains(sName)){
                ServerSettHelper *pServerSettHelper = m_mapSettings[sName];
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
    foreach( QString sKey, m_mapSettings.keys()){
        if(!listFoundInDatabase.contains(sKey)){
            ServerSettHelper *pServerSettHelper = m_mapSettings.value(sKey);
            initSettingDatabase(pServerSettHelper);
        }
    }
}

// ---------------------------------------------------------------------

void EmploySettings::addNewSetting(ServerSettHelper* pServerSettHelper){
    QString sName = pServerSettHelper->name();
    if(!m_mapSettings.contains(sName)){
        m_mapSettings[sName] = pServerSettHelper;
    }else{
        Log::warn(TAG, "Duplicate setting '" + sName + "'. Skip");
    }
}


// ---------------------------------------------------------------------

QString EmploySettings::getSettString(QString sName){
    QMutexLocker locker (&m_mtxServerSettings);
    QString sResult = "";
    if(m_mapSettings.contains(sName)){
        ServerSettHelper* pServerSettHelper = m_mapSettings.value(sName);
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

void EmploySettings::setSettString(QString sName, QString sValue){
    QMutexLocker locker (&m_mtxServerSettings);
    if(m_mapSettings.contains(sName)){
        ServerSettHelper* pServerSettHelper = m_mapSettings.value(sName);
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

QString EmploySettings::getSettPassword(QString sName){
    QMutexLocker locker (&m_mtxServerSettings);
    QString sResult = "";
    if(m_mapSettings.contains(sName)){
        ServerSettHelper* pServerSettHelper = m_mapSettings.value(sName);
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

void EmploySettings::setSettPassword(QString sName, QString sValue){
    QMutexLocker locker (&m_mtxServerSettings);
    if(m_mapSettings.contains(sName)){
        ServerSettHelper* pServerSettHelper = m_mapSettings.value(sName);
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

int EmploySettings::getSettInteger(QString sName){
    QMutexLocker locker (&m_mtxServerSettings);
    int nResult = 0;
    if(m_mapSettings.contains(sName)){
        ServerSettHelper* pServerSettHelper = m_mapSettings.value(sName);
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

void EmploySettings::setSettInteger(QString sName, int nValue){
    QMutexLocker locker (&m_mtxServerSettings);
    if(m_mapSettings.contains(sName)){
        ServerSettHelper* pServerSettHelper = m_mapSettings.value(sName);
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

bool EmploySettings::getSettBoolean(QString sName){
    QMutexLocker locker (&m_mtxServerSettings);
    bool bResult = false;
    if(m_mapSettings.contains(sName)){
        ServerSettHelper* pServerSettHelper = m_mapSettings.value(sName);
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

void EmploySettings::setSettBoolean(QString sName, bool bValue){
    QMutexLocker locker (&m_mtxServerSettings);
    if(m_mapSettings.contains(sName)){
        ServerSettHelper* pServerSettHelper = m_mapSettings.value(sName);
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

bool EmploySettings::hasSett(QString sName){
    return m_mapSettings.contains(sName);
}

// ---------------------------------------------------------------------

QString EmploySettings::getSettType(QString sName){
    if(m_mapSettings.contains(sName)){
        return m_mapSettings[sName]->type();
    }
    return "";
}

// ---------------------------------------------------------------------

QJsonArray EmploySettings::toJsonArray(){
    QJsonArray res;
    foreach( QString sName, m_mapSettings.keys()){
        ServerSettHelper* pServerSettHelper = m_mapSettings.value(sName);

        QJsonObject sett;
        sett["name"] = pServerSettHelper->name();
        if(pServerSettHelper->isBoolean()){
            sett["value"] = pServerSettHelper->valueAsBoolean();
        }else if(pServerSettHelper->isString()){
            sett["value"] = pServerSettHelper->valueAsString();
        }else if(pServerSettHelper->isInteger()){
            sett["value"] = pServerSettHelper->valueAsInteger();
        }else if(pServerSettHelper->isPassword()){
            sett["value"] = "******";
        }else{
            sett["value"] = pServerSettHelper->valueAsString();
        }

        sett["group"] = pServerSettHelper->group();
        sett["type"] = pServerSettHelper->type();
        res.append(sett);
    }
    return res;
}

// ---------------------------------------------------------------------

void EmploySettings::initSettingDatabase(ServerSettHelper *pServerSettHelper){
    Log::info(TAG, "Init settings to database: " + pServerSettHelper->name());
    QSqlDatabase db = *(m_pWebSocketServer->database());
    QSqlQuery query(db);
    query.prepare("INSERT INTO settings (`name`, `value`, `group`, `type`) VALUES (:name, :value, :group, :type)");
    query.bindValue(":name", pServerSettHelper->name());
    query.bindValue(":value", pServerSettHelper->valueAsString());
    query.bindValue(":group", pServerSettHelper->group());
    query.bindValue(":type", pServerSettHelper->type());
    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
    }
}

// ---------------------------------------------------------------------

void EmploySettings::updateSettingDatabase(ServerSettHelper *pServerSettHelper){
    QSqlDatabase db = *(m_pWebSocketServer->database());
    QSqlQuery query(db);
    query.prepare("UPDATE settings SET value = :value WHERE name = :name");
    query.bindValue(":value", pServerSettHelper->valueAsString());
    query.bindValue(":name", pServerSettHelper->name());
    if(!query.exec()){
        Log::err(TAG, query.lastError().text());
    }
}

// ---------------------------------------------------------------------

