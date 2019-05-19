#include <update0101.h>

REGISTRY_STORAGE_UPDATE(Update0101)

Update0101::Update0101()
    : StorageUpdateBase("u0100", "u0101", "Add new table for requests") {
    
    {
        StorageStruct user_requests("user_requests", StorageStructTableMode::CREATE);
        user_requests.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        user_requests.addColumn(StorageStructColumn("email").string(255).notNull());
        user_requests.addColumn(StorageStructColumn("type").string(255).notNull().defaultValue("''"));
        user_requests.addColumn(StorageStructColumn("code").string(32).notNull());
        user_requests.addColumn(StorageStructColumn("dt").datetime().notNull());
        user_requests.addColumn(StorageStructColumn("status").string(10).notNull().defaultValue("prepared"));
        user_requests.addColumn(StorageStructColumn("data").string(2048).notNull().enableIndex());
        m_vStructChanges.push_back(user_requests);
    }
}

bool Update0101::custom(Storage *pStorage, StorageConnection *pConn, std::string &error) {
    // here you can migrate data of correction if not just return true;
    return true;
}
