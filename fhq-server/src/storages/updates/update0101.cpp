#include <update0101.h>

REGISTRY_STORAGE_UPDATE(Update0101)

Update0101::Update0101()
    : StorageUpdateBase("u0100", "u0101", "Add new table 'users_requests'") {
    
    {
        StorageCreateTable *pUserRequests = createTable("users_requests");
        pUserRequests->addColumn("id").number().autoIncrement().primaryKey().notNull();
        pUserRequests->addColumn("email").string(255).notNull();
        pUserRequests->addColumn("type").string(255).notNull().defaultValue("''");
        pUserRequests->addColumn("code").string(32).notNull();
        pUserRequests->addColumn("dt").datetime().notNull();
        pUserRequests->addColumn("status").string(10).notNull().defaultValue("'prepared'");
        pUserRequests->addColumn("data").string(2048).notNull().enableIndex();
    }
}
