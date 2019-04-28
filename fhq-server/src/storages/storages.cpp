#include <storages.h>
#include <utils_logger.h>
#include <algorithm>

// ---------------------------------------------------------------------

std::map<std::string, IFabricStorage*> *g_pFabricStorages = NULL;

// ---------------------------------------------------------------------

void Storages::initGlobalVariables() {
    if (g_pFabricStorages == NULL) {
        // Log::info(std::string(), "Create employees map");
        g_pFabricStorages = new std::map<std::string, IFabricStorage*>();
    }
}

// ---------------------------------------------------------------------

void Storages::add(const std::string &sType, IFabricStorage* pStorage) {
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
    if (g_pFabricStorages->count(sType)) {
        pFabricStorage = g_pFabricStorages->at(sType);
        return pFabricStorage->create();
    }
    return nullptr;
}

// ---------------------------------------------------------------------

std::vector<StorageUpdateBase*> *g_pStorageUpdates = nullptr;

// ---------------------------------------------------------------------
// class StorageUpdateBase

StorageUpdateBase::StorageUpdateBase(const std::string &sFromVersion, const std::string &sVersion, const std::string &sDescription) {
    TAG = "Update_" + sVersion;
    m_sFromVersion = sFromVersion;
    m_sVersion = sVersion;
    m_sDescription = sDescription;
    StorageUpdates::initGlobalVariables();
    g_pStorageUpdates->push_back(this);
}

// ---------------------------------------------------------------------

const std::string &StorageUpdateBase::from_version() {
    return m_sFromVersion;
}

// ---------------------------------------------------------------------

const std::string &StorageUpdateBase::version() {
    return m_sVersion;
}

// ---------------------------------------------------------------------

const std::string &StorageUpdateBase::description() {
    return m_sDescription;
}

// ---------------------------------------------------------------------

const std::vector<StorageStruct> &StorageUpdateBase::listOfStructChanges() {
    return m_vStructChanges;
}

// ---------------------------------------------------------------------

void StorageUpdateBase::add(StorageStruct st) {
    m_vStructChanges.push_back(st);
}

// ---------------------------------------------------------------------

void StorageUpdates::initGlobalVariables() {
    if (g_pStorageUpdates == NULL) {
        // Log::info(std::string(), "Create list updates");
        g_pStorageUpdates = new std::vector<StorageUpdateBase*>();
    }
}

// ---------------------------------------------------------------------

StorageUpdateBase* StorageUpdates::findUpdateFromVersion(const std::string &sFromVersion) {
    std::string TAG = "StorageUpdates::findUpdateFromVersion";
    StorageUpdateBase* pRet = nullptr;
    for (int i = 0; i < g_pStorageUpdates->size(); i++) {
        StorageUpdateBase* pUpdate = g_pStorageUpdates->at(i);
        if (sFromVersion == pUpdate->from_version()) {
            if (pRet == nullptr) {
                pRet = pUpdate;
            } else {
                Log::warn(TAG, "Already defined update with from_version " + sFromVersion);
            }
        }
    }
    return pRet;
}

// ---------------------------------------------------------------------

bool StorageUpdates::apply(Storage *pStorage) {
    StorageUpdates::initGlobalVariables();
    std::string TAG = "StorageUpdates::apply";

    StorageConnection *pConn = pStorage->connect();
    if (pConn == nullptr) {
        Log::err(TAG, "Could not connect to database");
        return false;
    } else {
        Log::ok(TAG, "Successfully connection to database");
    }

    std::string sFirstVersion = "";
    std::string sLastVersion = pConn->lastDatabaseVersion();
    Log::info(TAG, "Last Version -> '" + sLastVersion + "'");

    // restore struct of storage before sLastVersion
    bool bHasUpdates = true;
    while (bHasUpdates) {
        bHasUpdates = false;
        StorageUpdateBase* pUpdate = StorageUpdates::findUpdateFromVersion(sFirstVersion);
        if (pUpdate != nullptr) {
            if (sFirstVersion != sLastVersion) {
                sFirstVersion = pUpdate->version();
                Log::info(TAG, "Add struct from '" + pUpdate->from_version() + "' -> '" + pUpdate->version() + "'");
                bHasUpdates = true;
                std::string error = "";
                std::vector<StorageStruct> vStructChanges = pUpdate->listOfStructChanges();
                for (int i = 0; i < vStructChanges.size(); i++) {
                    StorageStruct st = vStructChanges[i];
                    if (!pStorage->addStruct(st)) {
                        error = " Problem with add struct '" + st.tableName() + "'";
                        Log::err(TAG, error);
                        delete pConn;
                        return false;
                    }
                }
            }
        }
    }

    // apply new updates
    bHasUpdates = true;
    while (bHasUpdates) {
        bHasUpdates = false;
        StorageUpdateBase* pUpdate = StorageUpdates::findUpdateFromVersion(sLastVersion);
        if (pUpdate != nullptr) {
            Log::info(TAG, "Installing update '" + pUpdate->from_version() + "' -> '" + pUpdate->version() + "': " + pUpdate->description());
            sLastVersion = pUpdate->version();
            bHasUpdates = true;
            std::string error = "";
            std::vector<StorageStruct> vStructChanges = pUpdate->listOfStructChanges();

            // Apply struct
            for (int i = 0; i < vStructChanges.size(); i++) {
                StorageStruct st = vStructChanges[i];
                if (!pStorage->applyStruct(pConn, st)) {
                    error = "Problem with table '" + st.tableName() + "' in version " + pUpdate->version();
                    delete pConn;
                    return false;
                }
            }

            if (!pUpdate->custom(pStorage, pConn, error)) {
                Log::err(TAG, "Error on install custom update: \n\t -> " + error);
                delete pConn;
                return false;
            }
            pConn->insertUpdateInfo(pUpdate->version(), pUpdate->description());
        }
    }

    return true;
}
