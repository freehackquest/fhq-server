#ifndef STORAGES_H
#define STORAGES_H

#include <map>
#include <string>
#include <vector>
#include <utils_logger.h>

#include <string>
#include <vector>
#include <map>

// ---------------------------------------------------------------------

enum StorageStructColumnType {
    DATETIME,
    STRING,
    NUMBER,
    DOUBLE_NUMBER
};

// TODO cast to string

// ---------------------------------------------------------------------

class StorageStructColumn {
    public:
        StorageStructColumn(const std::string &sColumnName);
        StorageStructColumn &autoIncrement();
        StorageStructColumn &notNull();
        StorageStructColumn &string(int nSize);
        StorageStructColumn &text();
        StorageStructColumn &datetime();
        StorageStructColumn &number();
        StorageStructColumn &doubleNumber();
        StorageStructColumn &primaryKey();
        StorageStructColumn &defaultValue(const std::string& sDefault);
        StorageStructColumn &enableIndex();
        StorageStructColumn &enableUniqueIndex(const std::string& sIndexName);

        std::string columnName();
        std::string columnType();
        bool isDefaultValue();
        std::string columnDefaultValue();

        int columnTypeSize();
        bool isAutoIncrement();
        bool isPrimaryKey();
        bool isNotNull();
        bool isEnableIndex();
        bool isEnableUniqueIndex();
        std::string nameOfUniqueIndex();

    private:
        std::string TAG;
        std::string m_sColumnName;
        std::string m_sType;
        std::string m_sDefaultValue;
        int m_nTypeSize;

        bool m_bAutoIncrement;
        bool m_bNotNull;
        bool m_bPrimaryKey;
        bool m_bDefaultValue;
        bool m_bEnableIndex;
        bool m_bEnableUniqueIndex;
        std::string m_sNameOfUniqueIndex;
};

// ---------------------------------------------------------------------

// TODO redesign

class StorageStructColumnIndex {

};

// ---------------------------------------------------------------------

enum StorageStructTableMode { // TODO deprecated
    DROP,
    ALTER,
    CREATE
};

// ---------------------------------------------------------------------

enum StorageChangesType {
    NOPE,
    DROP_TABLE,
    ALTER_TABLE,
    CREATE_TABLE,
    INSERT_ROW,
    DELETE_ROW,
    UPDATE_ROW,
    SELECT_ROWS
};

// ---------------------------------------------------------------------

class StorageChanges {
    public:
        StorageChanges(const std::string &sTableName);
        std::string getTableName() const;
        virtual StorageChangesType getType() const;
        virtual std::string getStartApply() const;
        virtual std::string getAppliedSuccess() const;
        virtual std::string getAppliedFailed() const;

    protected:
        std::string TAG;
        std::string m_sTableName;
};

// ---------------------------------------------------------------------

// TODO split to different -> 
// StorageDropTable
// StorageAlterTable

class StorageCreateTable : public StorageChanges {
    public:
        StorageCreateTable(const std::string &sTableName);
        virtual StorageChangesType getType() const override;
        virtual std::string getStartApply() const override;
        virtual std::string getAppliedSuccess() const override;
        virtual std::string getAppliedFailed() const override;

        StorageStructColumn &addColumn(const std::string &sColumnName);
        const std::vector<StorageStructColumn> &getColumns() const;

    private:
        std::string TAG;
        std::string m_sTableName;
        std::vector<StorageStructColumn> m_vColumns;
};

// ---------------------------------------------------------------------

class StorageStruct {
    public:
        StorageStruct(const std::string &sTableName, StorageStructTableMode nMode);
        std::string tableName() const;
        StorageStructTableMode mode() const;
        bool addColumn(StorageStructColumn &column);
        bool alterColumn(StorageStructColumn &column);
        bool dropColumn(const std::string &sColumnName);
        const std::vector<StorageStructColumn> &listAddColumns() const;
        const std::vector<StorageStructColumn> &listAlterColumns() const;
        const std::vector<std::string> &listDropColumns() const;
        bool mergeWith(const StorageStruct &storageStruct);
        // bool mergeWith(const StorageCreateTable &storageStruct);
        // bool mergeWith(const StorageDropTable &storageStruct);
        // bool mergeWith(const StorageAlterTable &storageStruct);

    private:
        std::string TAG;
        std::string m_sTableName;
        StorageStructTableMode m_nMode;
        std::vector<StorageStructColumn> m_vAddColumns;
        std::vector<StorageStructColumn> m_vAlterColumns;
        std::vector<std::string> m_vDropColumns;
        
};

// ---------------------------------------------------------------------

class StorageColumnValue {
    public:
        StorageColumnValue(const std::string &sColumnName, StorageStructColumnType nType);
        void setValue(std::string sValue);
        void setValue(int nValue);
        void setValue(double nValue);
        std::string getColumnName();
        StorageStructColumnType getColumnType();
        std::string getString();
        int getInt();
        double getDouble();

    private:
        std::string m_sColumnName;
        StorageStructColumnType m_nColumnType;

        std::string m_sStringValue;
        int m_nIntValue;
        double m_nDoubleValue;
};

// ---------------------------------------------------------------------

class StorageInsert {
    public:
        StorageInsert(const std::string &sTableName);
        std::string tableName() const;

        void bindValue(const std::string &sColumnName, const std::string &sValue);
        void bindValue(const std::string &sColumnName, int nValue);
        void bindValue(const std::string &sColumnName, double nValue);
        std::vector<StorageColumnValue> values() const;
        bool isValid(const StorageStruct &storageStruct) const;

    private:
        bool exists(const std::string &sColumnName);
        std::string TAG;
        std::string m_sTableName;
        std::vector<StorageColumnValue> m_vValues;
};

// ---------------------------------------------------------------------

/*
class StorageUpdate {
    public:
        StorageUpdate(const StorageStruct &storageStruct);
        std::string tableName() const;

        void bindValue(const std::string &sColumnName, const std::string &sValue);
        void bindValue(const std::string &sColumnName, int nValue);
        void bindValue(const std::string &sColumnName, double nValue);
        std::vector<StorageColumnValue> values() const;
        bool isValid() const;

    private:
        StorageStruct m_storageStruct;
        bool exists(const std::string &sColumnName);
        std::string TAG;
        std::string m_sTableName;
        std::vector<StorageColumnValue> m_vValues;
};
*/

// ---------------------------------------------------------------------

class StorageTable {
    public:
        StorageTable(const std::string &sTableName);
        const std::vector<StorageStructColumn> &getColumns() const;
        void mergeWith(StorageCreateTable &createTable);
        void mergeWith(StorageChanges &storageChanges);

    private:
        std::string TAG;
        std::string m_sTableName;
        std::vector<StorageStructColumn> m_vColumns;
};

// ---------------------------------------------------------------------

class StorageConnection {
    public:
        StorageConnection();
        virtual ~StorageConnection();
        virtual bool executeQuery(const std::string &sQuery) = 0; // TODO redesign in future
        virtual std::string lastDatabaseVersion() = 0;
        virtual bool insertUpdateInfo(const std::string &sVersion, const std::string &sDescription) = 0;
        
        long created();

    protected:
        std::string TAG;

    private:
        long m_nCreated;
};

// ---------------------------------------------------------------------

class Storage {
    public:
        Storage();
        // Storage(const std::string &sType);
        static std::string type() { return "unknown"; };
        virtual bool applyConfigFromFile(const std::string &sFilePath) = 0;
        virtual StorageConnection *connect() = 0;
        virtual void clean() = 0;
        virtual std::string prepareStringValue(const std::string &sValue) = 0;

        virtual std::vector<std::string> prepareSqlQueries(StorageStruct &storageStruct) = 0; // TODO deprecated
        bool addStruct(StorageStruct &storageStruct); // TODO deprecated
        bool applyStruct(StorageConnection *pConn, StorageStruct &storageStruct); // TODO deprecated

        bool executeStorageChanges(StorageConnection *pConn, StorageChanges &storageChanges);
        bool addStorageChanges(StorageChanges &storageChanges);

        virtual std::vector<std::string> prepareSqlQueries(const StorageInsert &storageInsert) = 0;
        virtual std::vector<std::string> prepareSqlQueries(const StorageCreateTable &storageCreateTable) = 0;
        
        bool insertRow(StorageConnection *pConn, const StorageInsert &storageInsert);
        
        const std::map<std::string, StorageStruct> &storageStruct(); // TODO deprecated
        const std::map<std::string, StorageTable> &getTables();

    protected:
        std::string TAG;

    private:
        std::map<std::string, StorageStruct> m_mapStructs;
        std::map<std::string, StorageTable> m_mapTables;
};

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
        const std::vector<StorageStruct> &listOfStructChanges(); // TODO deprecated
        const std::vector<StorageChanges> &getListChanges();
        void add(StorageStruct st); // TODO deprecated
        void addStorageChanges(const StorageChanges &st);
        virtual bool custom(Storage *pStorage, StorageConnection *pConn, std::string &error) = 0;  // TODO deprecated create after and before update

    protected:
        std::string TAG;
        std::vector<StorageStruct> m_vStructChanges; // TODO: move to private
        
    private:
        std::vector<StorageChanges> m_vStorageChanges;
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
