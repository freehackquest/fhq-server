#ifndef STORAGE_H
#define STORAGE_H

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

enum StorageStructTableMode {
    DROP,
    ALTER,
    CREATE
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

        virtual std::vector<std::string> prepareSqlQueries(StorageStruct &storageStruct) = 0;
        bool addStruct(StorageStruct &storageStruct);
        bool applyStruct(StorageConnection *pConn, StorageStruct &storageStruct);

        virtual std::vector<std::string> prepareSqlQueries(const StorageInsert &storageInsert) = 0;
        bool insertRow(StorageConnection *pConn, const StorageInsert &storageInsert);
        
        std::map<std::string, StorageStruct> storageStruct();
        
    protected:
        std::string TAG;

    private:
        std::map<std::string, StorageStruct> m_mapStructs;
};

// ---------------------------------------------------------------------

#endif // STORAGE_H
