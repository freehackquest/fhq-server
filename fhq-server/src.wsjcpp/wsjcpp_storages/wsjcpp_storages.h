#ifndef WSJCPP_STORAGES_H
#define WSJCPP_STORAGES_H

#include <map>
#include <string>
#include <vector>
#include <wsjcpp_core.h>

#include <string>
#include <vector>
#include <map>

// ---------------------------------------------------------------------

enum WsjcppStorageColumnType {
    WSJCPP_STORAGE_COLUMN_TYPE_DATETIME,
    WSJCPP_STORAGE_COLUMN_TYPE_STRING,
    WSJCPP_STORAGE_COLUMN_TYPE_NUMBER,
    WSJCPP_STORAGE_COLUMN_TYPE_DOUBLE_NUMBER
};

// TODO cast to string

// ---------------------------------------------------------------------

class WsjcppStorageColumnDef {
    public:
        WsjcppStorageColumnDef(const std::string &sColumnName);
        WsjcppStorageColumnDef &autoIncrement();
        WsjcppStorageColumnDef &notNull();
        WsjcppStorageColumnDef &string(int nSize);
        WsjcppStorageColumnDef &text();
        WsjcppStorageColumnDef &datetime();
        WsjcppStorageColumnDef &number();
        WsjcppStorageColumnDef &bigNumber();
        WsjcppStorageColumnDef &doubleNumber();
        WsjcppStorageColumnDef &primaryKey();
        WsjcppStorageColumnDef &defaultValue(const std::string& sDefault);
        WsjcppStorageColumnDef &enableIndex();
        WsjcppStorageColumnDef &enableUniqueIndex(const std::string& sIndexName);

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

class WsjcppStorageColumnDefIndex {

};

// ---------------------------------------------------------------------

enum WsjcppStorageChangesType {
    WSJCPP_STORAGE_CHANGES_TYPE_NOPE,
    WSJCPP_STORAGE_CHANGES_TYPE_DROP_TABLE,
    WSJCPP_STORAGE_CHANGES_TYPE_MODIFY_TABLE,
    WSJCPP_STORAGE_CHANGES_TYPE_CREATE_TABLE,
    WSJCPP_STORAGE_CHANGES_TYPE_INSERT_ROW,
    WSJCPP_STORAGE_CHANGES_TYPE_DELETE_ROW,
    WSJCPP_STORAGE_CHANGES_TYPE_UPDATE_ROW,
    WSJCPP_STORAGE_CHANGES_TYPE_SELECT_ROWS
};

// ---------------------------------------------------------------------

class WsjcppStorageChanges {
    public:
        WsjcppStorageChanges(const std::string &sTableName);
        std::string getTableName() const;
        virtual WsjcppStorageChangesType getType() const;
        virtual std::string getStartApply() const;
        virtual std::string getAppliedSuccess() const;
        virtual std::string getAppliedFailed() const;

    protected:
        std::string TAG;
        std::string m_sTableName;
};

// ---------------------------------------------------------------------

class WsjcppStorageCreateTable : public WsjcppStorageChanges {
    public:
        WsjcppStorageCreateTable(const std::string &sTableName);
        virtual WsjcppStorageChangesType getType() const override;
        virtual std::string getStartApply() const override;
        virtual std::string getAppliedSuccess() const override;
        virtual std::string getAppliedFailed() const override;

        WsjcppStorageColumnDef &addColumn(const std::string &sColumnName);
        const std::vector<WsjcppStorageColumnDef> &getColumns() const;

    private:
        std::string TAG;
        std::vector<WsjcppStorageColumnDef> m_vColumns;
};

// ---------------------------------------------------------------------

class WsjcppStorageModifyTable : public WsjcppStorageChanges {
    public:
        WsjcppStorageModifyTable(const std::string &sTableName);
        virtual WsjcppStorageChangesType getType() const override;
        virtual std::string getStartApply() const override;
        virtual std::string getAppliedSuccess() const override;
        virtual std::string getAppliedFailed() const override;

        WsjcppStorageColumnDef &addColumn(const std::string &sColumnName);
        WsjcppStorageColumnDef &alterColumn(const std::string &sColumnName);
        std::string dropColumn(const std::string &sColumnName);
        const std::vector<WsjcppStorageColumnDef> &getAddColumns() const;
        const std::vector<WsjcppStorageColumnDef> &getAlterColumns() const;
        const std::vector<std::string> &getDropColumns() const;
        bool isColumnDefined(const std::string &sColumnName, std::string &sError) const;

    private:
        std::string TAG;
        std::vector<WsjcppStorageColumnDef> m_vAddColumns;
        std::vector<WsjcppStorageColumnDef> m_vAlterColumns;
        std::vector<std::string> m_vDropColumns;
};

// ---------------------------------------------------------------------

class WsjcppStorageDropTable : public WsjcppStorageChanges {
    public:
        WsjcppStorageDropTable(const std::string &sTableName);
        virtual WsjcppStorageChangesType getType() const override;
        virtual std::string getStartApply() const override;
        virtual std::string getAppliedSuccess() const override;
        virtual std::string getAppliedFailed() const override;

    private:
        std::string TAG;
};

// ---------------------------------------------------------------------

class WsjcppStorageColumnValue {
    public:
        WsjcppStorageColumnValue(const std::string &sColumnName, WsjcppStorageColumnType nType);
        void setValue(std::string sValue);
        void setValue(int nValue);
        void setValue(double nValue);
        std::string getColumnName();
        WsjcppStorageColumnType getColumnType();
        std::string getString();
        int getInt();
        double getDouble();

    private:
        std::string m_sColumnName;
        WsjcppStorageColumnType m_nColumnType;

        std::string m_sStringValue;
        int m_nIntValue;
        double m_nDoubleValue;
};

// ---------------------------------------------------------------------

class WsjcppStorageTable {
    public:
        WsjcppStorageTable(const std::string &sTableName);
        WsjcppStorageTable(WsjcppStorageCreateTable &createTable);
        std::string getTableName() const;
        const std::vector<WsjcppStorageColumnDef> &getColumns() const;
        void mergeWith(WsjcppStorageModifyTable &modifyTable);
        
    private:
        std::string TAG;
        std::string m_sTableName;
        std::vector<WsjcppStorageColumnDef> m_vColumns;
};

// ---------------------------------------------------------------------

class WsjcppStorageInsert : public WsjcppStorageChanges {
    public:
        WsjcppStorageInsert(const std::string &sTableName);
        virtual WsjcppStorageChangesType getType() const override;
        virtual std::string getStartApply() const override;
        virtual std::string getAppliedSuccess() const override;
        virtual std::string getAppliedFailed() const override;

        void bindValue(const std::string &sColumnName, const std::string &sValue);
        void bindValue(const std::string &sColumnName, int nValue);
        void bindValue(const std::string &sColumnName, double nValue);
        std::vector<WsjcppStorageColumnValue> values() const;
        bool isValid(const WsjcppStorageTable &storageTable) const;

    private:
        bool exists(const std::string &sColumnName);
        std::string TAG;
        std::string m_sTableName;
        std::vector<WsjcppStorageColumnValue> m_vValues;
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

class WsjcppStorageConnection {
    public:
        WsjcppStorageConnection();
        virtual ~WsjcppStorageConnection();
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

class WsjcppStorage {
    public:
        WsjcppStorage();
        // WsjcppStorage(const std::string &sType);
        static std::string type() { return "unknown"; };
        virtual bool applyConfigFromFile(const std::string &sFilePath) = 0;
        virtual WsjcppStorageConnection *connect() = 0;
        virtual void clean() = 0;
        virtual std::string prepareStringValue(const std::string &sValue) = 0;

        // virtual std::vector<std::string> prepareSqlQueries(StorageStruct &storageStruct) = 0; // TODO deprecated

        bool executeStorageChanges(WsjcppStorageConnection *pConn, WsjcppStorageChanges &storageChanges);
        bool addStorageChanges(WsjcppStorageChanges &storageChanges);

        virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageInsert &storageInsert) = 0;
        virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageCreateTable &storageCreateTable) = 0;
        virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageModifyTable &storageModifyTable) = 0;
        virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageDropTable &storageDropTable) = 0;
        
        bool insertRow(WsjcppStorageConnection *pConn, const WsjcppStorageInsert &storageInsert);
        const std::map<std::string, WsjcppStorageTable> &getTables();
        bool existsTable(const std::string &sTableName);
        const WsjcppStorageTable &getTableDef(const std::string &sTableName);

    protected:
        std::string TAG;

    private:
        std::map<std::string, WsjcppStorageTable> m_mapTables;
};

// ---------------------------------------------------------------------

class IFabricWsjcppStorage {
    public:
        virtual WsjcppStorage *create() = 0;
};

// ---------------------------------------------------------------------

extern std::map<std::string, IFabricWsjcppStorage*> *g_pFabricWsjcppStorages;

class WsjcppStorages {
    public:
        static void initGlobalVariables();
        static void add(const std::string &sType, IFabricWsjcppStorage* pStorage);
        static std::vector<std::string> list();
        static bool support(const std::string &sType);
        static WsjcppStorage* create(const std::string &sType);
};

// ---------------------------------------------------------------------

template<class T>
class FabricWsjcppStorage : IFabricWsjcppStorage {
    public:
        FabricWsjcppStorage() {
            WsjcppStorages::add(T::type(), this);
        };
        virtual WsjcppStorage *create() {
            return new T();
        };
};

// ---------------------------------------------------------------------
// RegistryStorage
#define REGISTRY_WSJCPP_STORAGE( classname ) \
    static FabricWsjcppStorage<classname> * pRegistryFabric ## classname = new FabricWsjcppStorage<classname>(); \

// ---------------------------------------------------------------------

class WsjcppStorageUpdateBase {

    public:
        WsjcppStorageUpdateBase(const std::string &sFromVersion, const std::string &sVersion, const std::string &sDescription);
        const std::string &from_version();
        const std::string &version();
        const std::string &description();
        const std::vector<WsjcppStorageChanges *> &getChanges();
        void setWeight(int nWeight);
        int getWeight();
    protected:
        std::string TAG;
        WsjcppStorageCreateTable *createTable(std::string sTableName);
        WsjcppStorageModifyTable *modifyTable(std::string sTableName);
        WsjcppStorageDropTable *dropTable(std::string sTableName);
        WsjcppStorageInsert *insertIntoTable(std::string sTableName);
        
    private:
        void checkTableName(std::string sTableName);
        std::vector<WsjcppStorageChanges *> m_vStorageChanges;
        std::string m_sFromVersion;
        std::string m_sVersion;
        std::string m_sDescription;
        int m_nWeight;
};

// ---------------------------------------------------------------------

extern std::vector<WsjcppStorageUpdateBase *> *g_pWsjcppStorageUpdates;

class WsjcppStorageUpdates {
    private:
        static int calculateWeight(int nWeight, const std::string &sVersion);
        static void sortByWeight(std::vector<WsjcppStorageUpdateBase*> &vUpdates);
        static std::vector<WsjcppStorageUpdateBase*> findUpdatesFromVersion(const std::string &sVersion);
        static void pushUpdatesFromVersion(std::vector<WsjcppStorageUpdateBase*> &vUpdates, const std::string &sVersion);
        static void applyAllStorageChanges(WsjcppStorage *pStorage, WsjcppStorageUpdateBase *pUpdate);
        static void executeAllStorageChanges(WsjcppStorage *pStorage, WsjcppStorageConnection *pConn, WsjcppStorageUpdateBase *pUpdate);

    public:
        static std::vector<WsjcppStorageUpdateBase*> getSortedStorageUpdates();
        static void initGlobalVariables();
        static WsjcppStorageUpdateBase* findUpdateFromVersion(const std::string &sFromVersion);
        static WsjcppStorageUpdateBase* findUpdateVersion(const std::string &sVersion);
        // static bool apply(Storage *pStorage);
        static bool apply2(WsjcppStorage *pStorage);
};

// ---------------------------------------------------------------------

// RegistryCmdHandler
#define REGISTRY_WSJCPP_STORAGE_UPDATE( classname ) \
    static classname * pRegistry ## classname = new classname();


#endif // WSJCPP_STORAGES_H
