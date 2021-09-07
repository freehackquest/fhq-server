
#include "argument_processor_database.h"
#include <wsjcpp_core.h>
#include <employ_database.h>

// ---------------------------------------------------------------------
// ArgumentProcessorApi

ArgumentProcessorDatabase::ArgumentProcessorDatabase() 
: WsjcppArgumentProcessor({"database", "db"}, "Database", "All what depened to database processing") {
    TAG = "ArgumentProcessorDatabase";
    registryProcessor(new ArgumentProcessorDatabaseTestConnection());
}

int ArgumentProcessorDatabase::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::err(TAG, "Not implemented");
    return -10;
}

// ---------------------------------------------------------------------
// ArgumentProcessorApi

ArgumentProcessorDatabaseTestConnection::ArgumentProcessorDatabaseTestConnection() 
: WsjcppArgumentProcessor({"test-connection", "test"}, "Test connection to database", "Test connection to database") {
    TAG = "ArgumentProcessorDatabaseTestConnection";
}

int ArgumentProcessorDatabaseTestConnection::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    std::cout << "\n * Check Database Connection\n\n";
    if (!WsjcppEmployees::init({})) {
        WsjcppLog::err(TAG, "Could not init database module");
        return -1;
    }
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase *db = pDatabase->database();
    if (!db->open()) {
        WsjcppLog::err(TAG, "Could not connect to database, please check config");
        return -1;
    }
    std::cout << "\n * Success\n\n";
    return 0;
}