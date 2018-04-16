#include "updates.h"


// ****************************
// **** CmdHandlerBase
// ****************************

// ---------------------------------------------------------------------

UpdateBase::UpdateBase(const std::string &sFromVersion, const std::string &sVersion, const std::string &sDescription){
    TAG = "Update_" + sVersion;
    m_sFromVersion = sFromVersion;
    m_sVersion = sVersion;
    m_sDescription = sDescription;
    Updates::initGlobalVariables();
    g_pUpdates->push_back(this);
}

// ---------------------------------------------------------------------

const std::string &UpdateBase::from_version(){
    return m_sFromVersion;
}

// ---------------------------------------------------------------------

const std::string &UpdateBase::version(){
    return m_sVersion;
}

// ---------------------------------------------------------------------

const std::string &UpdateBase::description(){
    return m_sDescription;
}

// ---------------------------------------------------------------------

// ****************************
// **** CmdHandlers Map
// ****************************

std::vector<UpdateBase*> *g_pUpdates = NULL;

// ****************************
// **** Updates
// ****************************

void Updates::initGlobalVariables(){
    if(g_pUpdates == NULL){
        // Log::info(std::string(), "Create list updates");
        g_pUpdates = new std::vector<UpdateBase*>();
    }
}

// ---------------------------------------------------------------------

bool Updates::updateDatabase(QSqlDatabase *pDatabase){
    Updates::initGlobalVariables();
    std::string TAG = "UpdateDatabase";

    QSqlDatabase db = *pDatabase;
    QSqlQuery query(db);

    query.prepare("SELECT * FROM updates ORDER BY id DESC LIMIT 0,1");
    query.exec();
    std::string sLastVersion = "";
    if (query.next()) {
        QSqlRecord record = query.record();
        // int updateid = record.value("id").toInt();
        sLastVersion = record.value("version").toString().toStdString();
        // std::string description = record.value("description").toString().toStdString();
    }

    Log::info(TAG, "Last Version -> " + sLastVersion);

    bool bHasUpdates = true;
    while(bHasUpdates){
        bHasUpdates = false;
        for(int i = 0; i < g_pUpdates->size(); i++){
            UpdateBase* pUpdateBase = g_pUpdates->at(i);
            if(sLastVersion == pUpdateBase->from_version()){
                Log::info(TAG, "Installing update " + pUpdateBase->from_version() + " -> " + pUpdateBase->version() + ": " + pUpdateBase->description());
                sLastVersion = pUpdateBase->version();
                bHasUpdates = true;
                std::string error;
                if(!pUpdateBase->update(db, error)){
                    Log::err(TAG, "Error on install update " + error);
                    return false;
                }

                QSqlQuery insert_query(db);
                insert_query.prepare("INSERT INTO updates (version, description, datetime_update) "
                      "VALUES (:version, :description, NOW())");
                insert_query.bindValue(":version", QString(pUpdateBase->version().c_str()));
                insert_query.bindValue(":description", QString(pUpdateBase->description().c_str()));
                if(!insert_query.exec()){
                    Log::err(TAG, "Could not insert row to updates: " + insert_query.lastError().text().toStdString());
                    return false;
                }
            }
        }
    }
    return true;
}
