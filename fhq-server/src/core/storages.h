#ifndef STORAGES_H
#define STORAGES_H

#include <map>
#include <string>
#include <vector>
#include <core/fallen.h>

#include <string>
#include <vector>
#include <map>

// ---------------------------------------------------------------------

enum StorageColumnType {
    DATETIME,
    STRING,
    NUMBER,
    DOUBLE_NUMBER
};

// TODO cast to string

// ---------------------------------------------------------------------

class StorageColumnDef {
    public:
        StorageColumnDef(const std::string &sColumnName);
        StorageColumnDef &autoIncrement();
        StorageColumnDef &notNull();
        StorageColumnDef &string(int nSize);
        StorageColumnDef &text();
        StorageColumnDef &datetime();
        StorageColumnDef &number();
        StorageColumnDef &doubleNumber();
        StorageColumnDef &primaryKey();
        StorageColumnDef &defaultValue(const std::string& sDefault);
        StorageColumnDef &enableIndex();
        StorageColumnDef &enableUniqueIndex(const std::string& sIndexName);

        std::string columnName() const;
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

class StorageColumnDefIndex {

};

// ---------------------------------------------------------------------

enum StorageChangesType {
    NOPE,
    DROP_TABLE,
    MODIFY_TABLE,
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

class StorageCreateTable : public StorageChanges {
    public:
        StorageCreateTable(const std::string &sTableName);
        virtual StorageChangesType getType() const override;
        virtual std::string getStartApply() const override;
        virtual std::string getAppliedSuccess() const override;
        virtual std::string getAppliedFailed() const override;

        StorageColumnDef &addColumn(const std::string &sColumnName);
        const std::vector<StorageColumnDef> &getColumns() const;

    private:
        std::string TAG;
        std::string m_sTableName;
        std::vector<StorageColumnDef> m_vColumns;
};

// ---------------------------------------------------------------------

class StorageModifyTable : public StorageChanges {
    public:
        StorageModifyTable(const std::string &sTableName);
        virtual StorageChangesType getType() const override;
        virtual std::string getStartApply() const override;
        virtual std::string getAppliedSuccess() const override;
        virtual std::string getAppliedFailed() const override;

        StorageColumnDef &addColumn(const std::string &sColumnName);
        StorageColumnDef &alterColumn(const std::string &sColumnName);
        std::string dropColumn(const std::string &sColumnName);
        const std::vector<StorageColumnDef> &getAddColumns() const;
        const std::vector<StorageColumnDef> &getAlterColumns() const;
        const std::vector<std::string> &getDropColumns() const;
        bool isColumnDefined(const std::string &sColumnName, std::string &sError) const;

    private:
        std::string TAG;
        std::string m_sTableName;
        std::vector<StorageColumnDef> m_vAddColumns;
        std::vector<StorageColumnDef> m_vAlterColumns;
        std::vector<std::string> m_vDropColumns;
};

// ---------------------------------------------------------------------

class StorageDropTable : public StorageChanges {
    public:
        StorageDropTable(const std::string &sTableName);
        virtual StorageChangesType getType() const override;
        virtual std::string getStartApply() const override;
        virtual std::string getAppliedSuccess() const override;
        virtual std::string getAppliedFailed() const override;

    private:
        std::string TAG;
        std::string m_sTableName;
};

// ---------------------------------------------------------------------

class StorageColumnValue {
    public:
        StorageColumnValue(const std::string &sColumnName, StorageColumnType nType);
        void setValue(std::string sValue);
        void setValue(int nValue);
        void setValue(double nValue);
        std::string getColumnName();
        StorageColumnType getColumnType();
        std::string getString();
        int getInt();
        double getDouble();

    private:
        std::string m_sColumnName;
        StorageColumnType m_nColumnType;

        std::string m_sStringValue;
        int m_nIntValue;
        double m_nDoubleValue;
};

// ---------------------------------------------------------------------

class StorageTable {
    public:
        StorageTable(const std::string &sTableName);
        StorageTable(StorageCreateTable &createTable);
        std::string getTableName() const;
        const std::vector<StorageColumnDef> &getColumns() const;
        void mergeWith(StorageModifyTable &modifyTable);
        
    private:
        std::string TAG;
        std::string m_sTableName;
        std::vector<StorageColumnDef> m_vColumns;
};

// ---------------------------------------------------------------------

class StorageInsert : public StorageChanges {
    public:
        StorageInsert(const std::string &sTableName);
        virtual StorageChangesType getType() const override;
        virtual std::string getStartApply() const override;
        virtual std::string getAppliedSuccess() const override;
        virtual std::string getAppliedFailed() const override;

        void bindValue(const std::string &sColumnName, const std::string &sValue);
        void bindValue(const std::string &sColumnName, int nValue);
        void bindValue(const std::string &sColumnName, double nValue);
        std::vector<StorageColumnValue> values() const;
        bool isValid(const StorageTable &storageTable) const;

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

class StorageConnection {
    public:
        StorageConnection();
        virtual ~StorageConnection();
        virtual bool executeQuery(const std::string &sQuery) = 0; // TODO redesign in future
        virtual std::string lastDatabaseVersion() = 0;
        virtual std::vector<std::string> getInstalledVersions() = 0;
        virtual bool insertUpdateInfo(const std::string &sVersion, const std::string &sDescription) = 0;
        
        long created();
        long getConnectionDurationInSeconds();

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

        // virtual std::vector<std::string> prepareSqlQueries(StorageStruct &storageStruct) = 0; // TODO deprecated

        bool executeStorageChanges(StorageConnection *pConn, StorageChanges &storageChanges);
        bool addStorageChanges(StorageChanges &storageChanges);

        virtual std::vector<std::string> prepareSqlQueries(const StorageInsert &storageInsert) = 0;
        virtual std::vector<std::string> prepareSqlQueries(const StorageCreateTable &storageCreateTable) = 0;
        virtual std::vector<std::string> prepareSqlQueries(const StorageModifyTable &storageModifyTable) = 0;
        virtual std::vector<std::string> prepareSqlQueries(const StorageDropTable &storageDropTable) = 0;
        
        bool insertRow(StorageConnection *pConn, const StorageInsert &storageInsert);
        const std::map<std::string, StorageTable> &getTables();
        bool existsTable(const std::string &sTableName);
        const StorageTable &getTableDef(const std::string &sTableName);

    protected:
        std::string TAG;

    private:
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
        const std::vector<StorageChanges *> &getChanges();
        void setWeight(int nWeight);
        int getWeight();
    protected:
        std::string TAG;
        StorageCreateTable *createTable(std::string sTableName);
        StorageModifyTable *modifyTable(std::string sTableName);
        StorageDropTable *dropTable(std::string sTableName);
        StorageInsert *insertIntoTable(std::string sTableName);
        
    private:
        void checkTableName(std::string sTableName);
        std::vector<StorageChanges *> m_vStorageChanges;
        std::string m_sFromVersion;
        std::string m_sVersion;
        std::string m_sDescription;
        int m_nWeight;
};

// ---------------------------------------------------------------------

extern std::vector<StorageUpdateBase*> *g_pStorageUpdates;

class StorageUpdates {
    private:
        static int calculateWeight(int nWeight, const std::string &sVersion);
        static void sortByWeight(std::vector<StorageUpdateBase*> &vUpdates);
        static std::vector<StorageUpdateBase*> findUpdatesFromVersion(const std::string &sVersion);
        static void pushUpdatesFromVersion(std::vector<StorageUpdateBase*> &vUpdates, const std::string &sVersion);
        static void applyAllStorageChanges(Storage *pStorage, StorageUpdateBase *pUpdate);
        static void executeAllStorageChanges(Storage *pStorage, StorageConnection *pConn, StorageUpdateBase *pUpdate);

    public:
        static std::vector<StorageUpdateBase*> getSortedStorageUpdates();
        static void initGlobalVariables();
        static StorageUpdateBase* findUpdateFromVersion(const std::string &sFromVersion);
        static StorageUpdateBase* findUpdateVersion(const std::string &sVersion);
        // static bool apply(Storage *pStorage);
        static bool apply2(Storage *pStorage);
};

// ---------------------------------------------------------------------

// RegistryCmdHandler
#define REGISTRY_STORAGE_UPDATE( classname ) \
    static classname * pRegistry ## classname = new classname();


#endif // STORAGES_H
