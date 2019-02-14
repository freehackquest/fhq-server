
#include "unit_test_storage_struct.h"
#include <storages.h>

REGISTRY_UNIT_TEST(UnitTestStorageStruct)

UnitTestStorageStruct::UnitTestStorageStruct() 
    : UnitTestBase("UnitTestStorageStruct") {
    // 
}

// ---------------------------------------------------------------------

void UnitTestStorageStruct::init() {
}

// ---------------------------------------------------------------------

bool UnitTestStorageStruct::run() {
    Storage *pStorage = Storages::create("mysql");
    bool bTestSuccess = true;
    int nExpectedSize = 0;
    std::string sExpectedQuery = "";

    StorageStruct test_tbl1("test_tbl1", StorageStructTableMode::CREATE);
	
    test_tbl1.addColumn(StorageStructColumn("id")
		.number().autoIncrement().primaryKey().notNull());

	test_tbl1.addColumn(StorageStructColumn("filed1")
		.string(255).notNull());

	test_tbl1.addColumn(StorageStructColumn("filed2")
		.text().notNull());

	test_tbl1.addColumn(StorageStructColumn("filed3")
		.datetime().notNull());

    std::vector<std::string> vQueries1 = pStorage->prepareSqlQueries(test_tbl1);
    if (!compareN("vQueries1", vQueries1.size(), 1)) {
        bTestSuccess = false;
    } else {
        bTestSuccess = compareS(bTestSuccess, vQueries1[0], 
            "CREATE TABLE IF NOT EXISTS test_tbl1 (\r\n"
			"  id INT NOT NULL AUTO_INCREMENT,\r\n"
			"  filed1 VARCHAR(255) NOT NULL,\r\n"
			"  filed2 TEXT NOT NULL,\r\n"
			"  filed3 DATETIME NOT NULL,\r\n"
			"  PRIMARY KEY (id)\r\n"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    }

    StorageStruct test_tbl2("test_tbl2", StorageStructTableMode::DROP);

    std::vector<std::string> vQueries2 = pStorage->prepareSqlQueries(test_tbl2);
    if (!compareN("vQueries2", vQueries2.size(), 1)) {
        bTestSuccess = false;
    } else {
        bTestSuccess = compareS(bTestSuccess, vQueries2[0], "DROP TABLE IF EXISTS test_tbl2;");
    }

    // Test Later table
    StorageStruct test_tbl3("test_tbl3", StorageStructTableMode::ALTER);
    test_tbl3.dropColumn("filed2");
    test_tbl3.dropColumn("filed3");
    test_tbl3.addColumn(StorageStructColumn("filed5")
		.string(123).notNull());
    test_tbl3.alterColumn(StorageStructColumn("filed1")
		.string(124).notNull());
    std::vector<std::string> vQueries3 = pStorage->prepareSqlQueries(test_tbl3);
    if (!compareN("vQueries3", vQueries3.size(), 4)) {
        bTestSuccess = false;
    } else {
        bTestSuccess = compareS(bTestSuccess, vQueries3[0], "ALTER TABLE test_tbl3 DROP COLUMN filed2;");
        bTestSuccess = compareS(bTestSuccess, vQueries3[1], "ALTER TABLE test_tbl3 DROP COLUMN filed3;");
        bTestSuccess = compareS(bTestSuccess, vQueries3[2], "ALTER TABLE test_tbl3 ADD COLUMN filed5 VARCHAR(123) NOT NULL;");
        bTestSuccess = compareS(bTestSuccess, vQueries3[3], "ALTER TABLE test_tbl3 MODIFY filed1 VARCHAR(124) NOT NULL;");
    }

    return bTestSuccess;
}

// ---------------------------------------------------------------------

bool UnitTestStorageStruct::compareS(bool bTestSuccess, const std::string &sValue, const std::string &sExpected) {
    if (sValue != sExpected) {
        Log::err(TAG, "Expeceted '" + sExpected + "', but got '" + sValue + "'");
        bTestSuccess = false;
    }
    return bTestSuccess;
}

// ---------------------------------------------------------------------

bool UnitTestStorageStruct::compareN(const std::string &sPoint, int nValue, int nExpected) {
    if (nValue != nExpected) {
        Log::err(TAG, " {" + sPoint + "} Expeceted '" + std::to_string(nValue) + "', but got '" + std::to_string(nExpected) + "'");
        return false;
    }
    return true;
}