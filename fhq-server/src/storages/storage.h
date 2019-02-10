#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <vector>

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
        StorageStructColumn &primaryKey();
        
        bool isAutoIncrement();

    private:
        std::string m_sColumnName;
        std::string m_sType;
        int m_nTypeSize;

        bool m_bAutoIncrement;
        bool m_bNotNull;
        bool m_bPrimaryKey;
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
        std::string tableName();
        StorageStructTableMode mode();
        void addColumn(StorageStructColumn &column);
        void alterColumn(const std::string &sColumnName);
        void dropColumn(const std::string &sColumnName);
    private:
        std::string m_sTableName;
        std::vector<StorageStructColumn> m_vAddColumns;
        StorageStructTableMode m_nMode;
};

// ---------------------------------------------------------------------

class Storage {
    public:
        // Storage(const std::string &sType);
        static std::string type() { return "unknown"; };
        virtual bool applyConfigFromFile(const std::string &sFilePath) = 0;
        virtual void clean() = 0;
        virtual std::string lastVersion() = 0;
        virtual bool insertUpdateInfo(const std::string &sVersion, const std::string &sDescription) = 0;
        virtual bool applyStruct(StorageStruct &storageStruct) = 0;
};

#endif // STORAGE_H