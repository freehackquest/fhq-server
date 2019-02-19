
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
		.string(255).notNull().enableIndex());

	test_tbl1.addColumn(StorageStructColumn("filed2")
		.text().notNull());

	test_tbl1.addColumn(StorageStructColumn("filed3")
		.datetime().notNull());

	test_tbl1.addColumn(StorageStructColumn("filed4")
		.string(2000).notNull().enableIndex());

    test_tbl1.addColumn(StorageStructColumn("filed5")
		.number().notNull().enableUniqueIndex("idx_f5_and_f7"));

    test_tbl1.addColumn(StorageStructColumn("filed6")
		.number().notNull().enableUniqueIndex("idx_f6_and_f8"));

    test_tbl1.addColumn(StorageStructColumn("filed7")
		.number().notNull().enableUniqueIndex("idx_f5_and_f7"));

    test_tbl1.addColumn(StorageStructColumn("filed8")
		.number().notNull().enableUniqueIndex("idx_f6_and_f8"));

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
            "  filed4 VARCHAR(2000) NOT NULL,\r\n"
            "  filed5 INT NOT NULL,\r\n"
            "  filed6 INT NOT NULL,\r\n"
            "  filed7 INT NOT NULL,\r\n"
            "  filed8 INT NOT NULL,\r\n"
			"  PRIMARY KEY (id),\r\n"
            "  KEY idx_filed1 (filed1),\r\n"
            "  KEY idx_filed4 (filed4(255)),\r\n"
            "  UNIQUE KEY idx_f5_and_f7 (filed5,filed7),\r\n"
            "  UNIQUE KEY idx_f6_and_f8 (filed6,filed8)\r\n"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    }


    StorageStruct modify_test_tbl1("test_tbl1", StorageStructTableMode::ALTER);
    modify_test_tbl1.dropColumn("filed1");
    modify_test_tbl1.dropColumn("filed2");
    modify_test_tbl1.addColumn(StorageStructColumn("filed9").number().notNull());
    modify_test_tbl1.alterColumn(StorageStructColumn("filed4").string(1500).notNull());

    std::vector<std::string> vQueries1_1 = pStorage->prepareSqlQueries(modify_test_tbl1);
    if (!compareN("vQueries1_1", vQueries1_1.size(), 4)) {
        bTestSuccess = false;
    } else {
        bTestSuccess = compareS(bTestSuccess, vQueries1_1[0], "ALTER TABLE test_tbl1 DROP COLUMN filed1;");
        bTestSuccess = compareS(bTestSuccess, vQueries1_1[1], "ALTER TABLE test_tbl1 DROP COLUMN filed2;");
        bTestSuccess = compareS(bTestSuccess, vQueries1_1[2], "ALTER TABLE test_tbl1 ADD COLUMN filed9 INT NOT NULL;");
        bTestSuccess = compareS(bTestSuccess, vQueries1_1[3], "ALTER TABLE test_tbl1 MODIFY filed4 VARCHAR(1500) NOT NULL;");
    }

    // TEST merge structs
    if (!test_tbl1.mergeWith(modify_test_tbl1)) {
        Log::err(TAG, "Problem with merge structs");
        bTestSuccess = false;
    }

    std::vector<std::string> vQueries1_2 = pStorage->prepareSqlQueries(test_tbl1);
    if (!compareN("vQueries1_2", vQueries1_2.size(), 1)) {
        bTestSuccess = false;
    } else {
        bTestSuccess = compareS(bTestSuccess, vQueries1_2[0],
            "CREATE TABLE IF NOT EXISTS test_tbl1 (\r\n"
			"  id INT NOT NULL AUTO_INCREMENT,\r\n"
			"  filed3 DATETIME NOT NULL,\r\n"
            "  filed4 VARCHAR(1500) NOT NULL,\r\n"
            "  filed5 INT NOT NULL,\r\n"
            "  filed6 INT NOT NULL,\r\n"
            "  filed7 INT NOT NULL,\r\n"
            "  filed8 INT NOT NULL,\r\n"
            "  filed9 INT NOT NULL,\r\n"
			"  PRIMARY KEY (id),\r\n"
            "  KEY idx_filed4 (filed4(255)),\r\n"
            "  UNIQUE KEY idx_f5_and_f7 (filed5,filed7),\r\n"
            "  UNIQUE KEY idx_f6_and_f8 (filed6,filed8)\r\n"
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
        Log::err(TAG, " {" + sPoint + "} Expeceted '" + std::to_string(nExpected) + "', but got '" + std::to_string(nValue) + "'");
        return false;
    }
    return true;
}
