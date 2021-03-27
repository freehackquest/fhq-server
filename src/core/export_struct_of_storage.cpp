#include "export_struct_of_storage.h"
#include <iostream>
#include <wsjcpp_storages.h>

class FakeVersion {
    public:
        FakeVersion() {
            m_sLastVersion = "";
        };

        std::string getVersion() {
            return m_sLastVersion;
        };

        void setVersion(std::string sLastVersion) {
            m_sLastVersion = sLastVersion;
        };

    private:
        std::string m_sLastVersion;
};

// ----------------------------------------------------------------------

class FakeStorageConnection : public WsjcppStorageConnection {
    public:
        FakeStorageConnection(FakeVersion *pVersion) {
            m_pVersion = pVersion;
        };
        virtual bool executeQuery(const std::string &sQuery) {
            return true;
        }; 
        virtual std::string lastDatabaseVersion() {
            return m_pVersion->getVersion();
        };
        virtual std::vector<std::string> getInstalledVersions() override {
            std::vector<std::string> vVersions;
            return vVersions;
        }
        virtual bool insertUpdateInfo(const std::string &sVersion, const std::string &sDescription) {
            m_pVersion->setVersion(sVersion);
            return true;
        };

    private:
        FakeVersion *m_pVersion;
};

// ----------------------------------------------------------------------

class FakeStorage : public WsjcppStorage {
    public:
        FakeStorage() {
            m_pVersion = new FakeVersion();
        };
        virtual bool applyConfigFromFile(const std::string &sFilePath) {
            return true;
        };
        virtual WsjcppStorageConnection *connect() {
            return new FakeStorageConnection(m_pVersion);
        };
        virtual void clean() {
            // nothing
        };
        virtual std::string prepareStringValue(const std::string &sValue) {
            return sValue;
        };
        virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageInsert &storageInsert) {
            return std::vector<std::string>();
        };
        virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageCreateTable &storageCreateTable) {
            return std::vector<std::string>();
        };
        virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageModifyTable &storageModifyTable) {
            return std::vector<std::string>();
        };
        virtual std::vector<std::string> prepareSqlQueries(const WsjcppStorageDropTable &storageDropTable) {
            return std::vector<std::string>();
        };

    private:
        FakeVersion *m_pVersion;
};

// ----------------------------------------------------------------------

void ExportStructOfStorage::print() {
    FakeStorage *pStorage = new FakeStorage();
    WsjcppStorageUpdates::apply2(pStorage);

    std::cout << std::endl << std::endl << " * Tables: " << std::endl;
    std::map<std::string, WsjcppStorageTable> tables = pStorage->getTables();
    std::map<std::string, WsjcppStorageTable>::iterator it = tables.begin();
    while (it != tables.end()) {
        std::string sTableName = it->first;
        WsjcppStorageTable st = it->second;
        std::cout << " + Table '" << st.getTableName() << "' " << std::endl;
        std::cout << " |---* Columns: " << std::endl;
        std::vector<WsjcppStorageColumnDef> vColumns = st.getColumns();
        for (int i = 0; i < vColumns.size(); i++) {
            WsjcppStorageColumnDef c = vColumns[i];
            std::cout << " |   | - '" 
                << c.columnName() << "' as " << c.columnType() << " (" << c.columnTypeSize() << ") " 
                // TODO c.columnDescription()
                << std::endl;
        }
        std::cout << " | " << std::endl;
        it++;
    }
}
