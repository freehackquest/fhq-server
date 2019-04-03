#ifndef STORAGES_H
#define STORAGES_H

#include <map>
#include <string>
#include <vector>
#include <storage.h>
#include <utils_logger.h>

// ---------------------------------------------------------------------

class IFabricStorage {
    public:
        virtual Storage *create() = 0;
};

// ---------------------------------------------------------------------

extern std::map<std::string, IFabricStorage*> *g_pFabricStorages;

class Storages {
    public:
        static void initGlobalVariables();
        static void add(const std::string &sType, IFabricStorage* pStorage);
        static std::vector<std::string> list();
        static bool support(const std::string &sType);
        static Storage* create(const std::string &sType);
};

// ---------------------------------------------------------------------

template<class T>
class FabricStorage : IFabricStorage {
    public:
        FabricStorage() {
            Storages::add(T::type(), this);
        };
        virtual Storage *create() {
            return new T();
        };
};

// ---------------------------------------------------------------------
// RegistryStorage
#define REGISTRY_STORAGE( classname ) \
    static FabricStorage<classname> * pRegistryFabric ## classname = new FabricStorage<classname>(); \

// ---------------------------------------------------------------------

class StorageUpdateBase {

    public:
        StorageUpdateBase(const std::string &sFromVersion, const std::string &sVersion, const std::string &sDescription);
        const std::string &from_version();
        const std::string &version();
        const std::string &description();
        const std::vector<StorageStruct> &listOfStructChanges();
        virtual bool custom(Storage *pStorage, StorageConnection *pConn, std::string &error) = 0;

    protected:
        std::string TAG;
        std::vector<StorageStruct> m_vStructChanges;
        
    private:
        std::string m_sFromVersion;
        std::string m_sVersion;
        std::string m_sDescription;
};

// ---------------------------------------------------------------------

extern std::vector<StorageUpdateBase*> *g_pStorageUpdates;

class StorageUpdates {
    public:
        static void initGlobalVariables();
        static StorageUpdateBase* findUpdateFromVersion(const std::string &sFromVersion);
        static bool apply(Storage *pStorage);
};

// ---------------------------------------------------------------------

// RegistryCmdHandler
#define REGISTRY_STORAGE_UPDATE( classname ) \
    static classname * pRegistry ## classname = new classname();


#endif // STORAGES_H
