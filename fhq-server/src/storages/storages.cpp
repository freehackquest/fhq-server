#include <storages.h>
#include <utils_logger.h>
#include <algorithm>

// ---------------------------------------------------------------------

std::map<std::string, IFabricStorage*> *g_pFabricStorages = NULL;

// ---------------------------------------------------------------------

void Storages::initGlobalVariables(){
    if(g_pFabricStorages == NULL){
        // Log::info(std::string(), "Create employees map");
        g_pFabricStorages = new std::map<std::string, IFabricStorage*>();
    }
}

// ---------------------------------------------------------------------

void Storages::add(const std::string &sType, IFabricStorage* pStorage){
    Storages::initGlobalVariables();
    const std::string TAG = "addStorage";
    if (g_pFabricStorages->count(sType) > 0) {
        Log::err(TAG, sType + " - storage already registered");
    } else {
        g_pFabricStorages->insert(std::pair<std::string, IFabricStorage*>(sType,pStorage));
        // Log::info(sType, "Registered");
    }
}

// ---------------------------------------------------------------------

std::vector<std::string> Storages::list() {
    Storages::initGlobalVariables();
    std::vector<std::string> vResult;
    std::map<std::string, IFabricStorage*>::iterator it;
    for (it = g_pFabricStorages->begin(); it != g_pFabricStorages->end(); it++) {
        vResult.push_back(it->first);
    }
    return vResult;
}

// ---------------------------------------------------------------------

bool Storages::support(const std::string &sType) {
    Storages::initGlobalVariables();
    return g_pFabricStorages->count(sType) != 0;
}

// ---------------------------------------------------------------------

Storage* Storages::create(const std::string &sType) {
    Storages::initGlobalVariables();
    std::string TAG = "findStorage";
    IFabricStorage *pFabricStorage = nullptr;
    if(g_pFabricStorages->count(sType)){
        pFabricStorage = g_pFabricStorages->at(sType);
        return pFabricStorage->create();
    }
    return nullptr;
}

// ---------------------------------------------------------------------

std::vector<StorageUpdateBase*> *g_pStorageUpdates = nullptr;

// ---------------------------------------------------------------------
// class StorageUpdateBase

StorageUpdateBase::StorageUpdateBase(const std::string &sFromVersion, const std::string &sVersion, const std::string &sDescription){
    TAG = "Update_" + sVersion;
    m_sFromVersion = sFromVersion;
    m_sVersion = sVersion;
    m_sDescription = sDescription;
    StorageUpdates::initGlobalVariables();
    g_pStorageUpdates->push_back(this);
}

// ---------------------------------------------------------------------

const std::string &StorageUpdateBase::from_version(){
    return m_sFromVersion;
}

// ---------------------------------------------------------------------

const std::string &StorageUpdateBase::version(){
    return m_sVersion;
}

// ---------------------------------------------------------------------

const std::string &StorageUpdateBase::description(){
    return m_sDescription;
}



// ---------------------------------------------------------------------

void StorageUpdates::initGlobalVariables(){
    if(g_pStorageUpdates == NULL){
        // Log::info(std::string(), "Create list updates");
        g_pStorageUpdates = new std::vector<StorageUpdateBase*>();
    }
}

// ---------------------------------------------------------------------

bool StorageUpdates::apply(Storage *pStorage){
    StorageUpdates::initGlobalVariables();
    std::string TAG = "StorageUpdates::apply";

    std::string sLastVersion = pStorage->lastVersion();

    Log::info(TAG, "Last Version -> " + sLastVersion);

    bool bHasUpdates = true;
    while (bHasUpdates) {
        bHasUpdates = false;
        for(int i = 0; i < g_pStorageUpdates->size(); i++){
            StorageUpdateBase* pUpdate = g_pStorageUpdates->at(i);
            if (sLastVersion == pUpdate->from_version()) {
                Log::info(TAG, "Installing update " + pUpdate->from_version() + " -> " + pUpdate->version() + ": " + pUpdate->description());
                sLastVersion = pUpdate->version();
                bHasUpdates = true;
                std::string error;
                if (!pUpdate->apply(pStorage, error)) {
                    Log::err(TAG, "Error on install update " + error);
                    return false;
                }
                pStorage->insertUpdateInfo(pUpdate->version(), pUpdate->description());
            }
        }
    }

    return true;
}
