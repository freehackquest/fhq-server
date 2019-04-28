
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

    StorageCreateTable test_tbl0("test_tbl0");
    test_tbl0.addColumn("id").number().autoIncrement().primaryKey().notNull();
    test_tbl0.addColumn("filed1").string(255).notNull().enableIndex();

    std::vector<std::string> vQueries0 = pStorage->prepareSqlQueries(test_tbl0);
    if (!compareN("vQueries0", vQueries0.size(), 1)) {
        bTestSuccess = false;
    } else {
        bTestSuccess = compareS(bTestSuccess, vQueries0[0], 
            "CREATE TABLE IF NOT EXISTS `test_tbl0` (\r\n"
			"  `id` INT NOT NULL AUTO_INCREMENT,\r\n"
			"  `filed1` VARCHAR(255) NOT NULL,\r\n"
			"  PRIMARY KEY (id),\r\n"
            "  KEY idx_filed1 (filed1)\r\n"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    }

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

    test_tbl1.addColumn(StorageStructColumn("filed9")
		.doubleNumber().defaultValue("0.0"));

    std::vector<std::string> vQueries1 = pStorage->prepareSqlQueries(test_tbl1);
    if (!compareN("vQueries1", vQueries1.size(), 1)) {
        bTestSuccess = false;
    } else {
        bTestSuccess = compareS(bTestSuccess, vQueries1[0], 
            "CREATE TABLE IF NOT EXISTS `test_tbl1` (\r\n"
			"  `id` INT NOT NULL AUTO_INCREMENT,\r\n"
			"  `filed1` VARCHAR(255) NOT NULL,\r\n"
			"  `filed2` TEXT NOT NULL,\r\n"
			"  `filed3` DATETIME NOT NULL,\r\n"
            "  `filed4` VARCHAR(2000) NOT NULL,\r\n"
            "  `filed5` INT NOT NULL,\r\n"
            "  `filed6` INT NOT NULL,\r\n"
            "  `filed7` INT NOT NULL,\r\n"
            "  `filed8` INT NOT NULL,\r\n"
            "  `filed9` DOUBLE DEFAULT 0.0,\r\n"
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
    modify_test_tbl1.addColumn(StorageStructColumn("filed10").number().notNull());
    modify_test_tbl1.alterColumn(StorageStructColumn("filed4").string(1500).notNull());

    std::vector<std::string> vQueries1_1 = pStorage->prepareSqlQueries(modify_test_tbl1);
    if (!compareN("vQueries1_1", vQueries1_1.size(), 4)) {
        bTestSuccess = false;
    } else {
        bTestSuccess = compareS(bTestSuccess, vQueries1_1[0], "ALTER TABLE `test_tbl1` DROP COLUMN `filed1`;");
        bTestSuccess = compareS(bTestSuccess, vQueries1_1[1], "ALTER TABLE `test_tbl1` DROP COLUMN `filed2`;");
        bTestSuccess = compareS(bTestSuccess, vQueries1_1[2], "ALTER TABLE `test_tbl1` ADD COLUMN `filed10` INT NOT NULL;");
        bTestSuccess = compareS(bTestSuccess, vQueries1_1[3], "ALTER TABLE `test_tbl1` MODIFY `filed4` VARCHAR(1500) NOT NULL;");
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
            "CREATE TABLE IF NOT EXISTS `test_tbl1` (\r\n"
			"  `id` INT NOT NULL AUTO_INCREMENT,\r\n"
			"  `filed3` DATETIME NOT NULL,\r\n"
            "  `filed4` VARCHAR(1500) NOT NULL,\r\n"
            "  `filed5` INT NOT NULL,\r\n"
            "  `filed6` INT NOT NULL,\r\n"
            "  `filed7` INT NOT NULL,\r\n"
            "  `filed8` INT NOT NULL,\r\n"
            "  `filed9` DOUBLE DEFAULT 0.0,\r\n"
            "  `filed10` INT NOT NULL,\r\n"
			"  PRIMARY KEY (id),\r\n"
            "  KEY idx_filed4 (filed4(255)),\r\n"
            "  UNIQUE KEY idx_f5_and_f7 (filed5,filed7),\r\n"
            "  UNIQUE KEY idx_f6_and_f8 (filed6,filed8)\r\n"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    }

    StorageInsert tbl1_ins("test_tbl1");
    tbl1_ins.bindValue("filed3", "so\"me"); // TODO datetime
    tbl1_ins.bindValue("filed4", "some");
    tbl1_ins.bindValue("filed5", 123);
    tbl1_ins.bindValue("filed6", 321);
    tbl1_ins.bindValue("filed7", 456);
    tbl1_ins.bindValue("filed8", 654);
    tbl1_ins.bindValue("filed9", 555.0);
    tbl1_ins.bindValue("filed10", 789);
    if (!tbl1_ins.isValid(test_tbl1)) {
        bTestSuccess = false;
    }
    std::vector<std::string> vQueries1_ins = pStorage->prepareSqlQueries(tbl1_ins);
    if (!compareN("vQueries1_ins", vQueries1_ins.size(), 1)) {
        bTestSuccess = false;
    } else {
        bTestSuccess = compareS(bTestSuccess, vQueries1_ins[0], "INSERT INTO test_tbl1"
            "(filed3, filed4, filed5, filed6, filed7, filed8, filed9, filed10) "
            "VALUES(\"so\\\"me\", \"some\", 123, 321, 456, 654, 555.000000, 789);");
    }

    StorageStruct test_tbl2("test_tbl2", StorageStructTableMode::DROP);

    std::vector<std::string> vQueries2 = pStorage->prepareSqlQueries(test_tbl2);
    if (!compareN("vQueries2", vQueries2.size(), 1)) {
        bTestSuccess = false;
    } else {
        bTestSuccess = compareS(bTestSuccess, vQueries2[0], "DROP TABLE IF EXISTS `test_tbl2`;");
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
        bTestSuccess = compareS(bTestSuccess, vQueries3[0], "ALTER TABLE `test_tbl3` DROP COLUMN `filed2`;");
        bTestSuccess = compareS(bTestSuccess, vQueries3[1], "ALTER TABLE `test_tbl3` DROP COLUMN `filed3`;");
        bTestSuccess = compareS(bTestSuccess, vQueries3[2], "ALTER TABLE `test_tbl3` ADD COLUMN `filed5` VARCHAR(123) NOT NULL;");
        bTestSuccess = compareS(bTestSuccess, vQueries3[3], "ALTER TABLE `test_tbl3` MODIFY `filed1` VARCHAR(124) NOT NULL;");
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
