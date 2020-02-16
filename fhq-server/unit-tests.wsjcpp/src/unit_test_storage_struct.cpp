
#include "unit_test_storage_struct.h"
#include <core/storages.h>

REGISTRY_UNIT_TEST(UnitTestStorageStruct)

UnitTestStorageStruct::UnitTestStorageStruct() 
    : WSJCppUnitTestBase("UnitTestStorageStruct") {
    // 
}

// ---------------------------------------------------------------------

void UnitTestStorageStruct::init() {
}

// ---------------------------------------------------------------------

bool UnitTestStorageStruct::run() {
    if (!Storages::support("mysql")) {
        WSJCppLog::err(TAG, "Not supported mysql");
        return false;
    }
    Storage *pStorage = Storages::create("mysql");
    bool bTestSuccess = true;
    int nExpectedSize = 0;
    std::string sExpectedQuery = "";

    createTestTable0(bTestSuccess, pStorage);
    dropTestTable0(bTestSuccess, pStorage);
    createTestTable1(bTestSuccess, pStorage);
    modifyTestTable1(bTestSuccess, pStorage);
    checkModifiedTable1(bTestSuccess, pStorage);
    insertTestTable1(bTestSuccess, pStorage);
    createTestTable3(bTestSuccess, pStorage);
    modifyTestTable3(bTestSuccess, pStorage);
    return bTestSuccess;
}

// ---------------------------------------------------------------------

void UnitTestStorageStruct::createTestTable0(bool &bTestSuccess, Storage *pStorage) {
    StorageCreateTable test_tbl0("test_tbl0");
    test_tbl0.addColumn("id").number().autoIncrement().primaryKey().notNull();
    test_tbl0.addColumn("filed1").string(255).notNull().enableIndex();
    std::vector<std::string> vQueries0 = pStorage->prepareSqlQueries(test_tbl0);
    if (compareN(bTestSuccess, "createTable0", vQueries0.size(), 1)) {
        compareS(bTestSuccess, "createTable0", vQueries0[0], 
            "CREATE TABLE IF NOT EXISTS `test_tbl0` (\r\n"
            "  `id` INT NOT NULL AUTO_INCREMENT,\r\n"
            "  `filed1` VARCHAR(255) NOT NULL,\r\n"
            "  PRIMARY KEY (id),\r\n"
            "  KEY idx_filed1 (filed1)\r\n"
            ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    }
    pStorage->addStorageChanges(test_tbl0); // add just runtime storage changes
}

// ---------------------------------------------------------------------

void UnitTestStorageStruct::dropTestTable0(bool &bTestSuccess, Storage *pStorage) {
    StorageDropTable test_tbl0_drop("test_tbl0");
    std::vector<std::string> vQueries2 = pStorage->prepareSqlQueries(test_tbl0_drop);
    if (compareN(bTestSuccess, "dropTestTable0", vQueries2.size(), 1)) {
        compareS(bTestSuccess, "dropTestTable0", vQueries2[0], "DROP TABLE IF EXISTS `test_tbl0`;");
    }
    pStorage->addStorageChanges(test_tbl0_drop); 
    if (pStorage->existsTable("test_tbl0")) {
        WSJCppLog::err(TAG, "Table 'test_tbl0' must be disappeared");
        bTestSuccess = false;
    }
}

// ---------------------------------------------------------------------

void UnitTestStorageStruct::createTestTable1(bool &bTestSuccess, Storage *pStorage) {
    StorageCreateTable test_tbl1("test_tbl1");
    test_tbl1.addColumn("id").number().autoIncrement().primaryKey().notNull();
    test_tbl1.addColumn("filed1").string(255).notNull().enableIndex();
    test_tbl1.addColumn("filed2").text().notNull();
    test_tbl1.addColumn("filed3").datetime().notNull();
    test_tbl1.addColumn("filed4").string(2000).notNull().enableIndex();
    test_tbl1.addColumn("filed5").number().notNull().enableUniqueIndex("idx_f5_and_f7");
    test_tbl1.addColumn("filed6").number().notNull().enableUniqueIndex("idx_f6_and_f8");
    test_tbl1.addColumn("filed7").number().notNull().enableUniqueIndex("idx_f5_and_f7");
    test_tbl1.addColumn("filed8").number().notNull().enableUniqueIndex("idx_f6_and_f8");
    test_tbl1.addColumn("filed9").doubleNumber().defaultValue("0.0");
    std::vector<std::string> vQueries1 = pStorage->prepareSqlQueries(test_tbl1);
    if (compareN(bTestSuccess, "createTestTable1", vQueries1.size(), 1)) {
        compareS(bTestSuccess, "createTestTable1", vQueries1[0], 
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
    pStorage->addStorageChanges(test_tbl1); // add just runtime storage changes
}

// ---------------------------------------------------------------------

void UnitTestStorageStruct::modifyTestTable1(bool &bTestSuccess, Storage *pStorage) {
    StorageModifyTable modify_test_tbl1("test_tbl1");
    modify_test_tbl1.dropColumn("filed1");
    modify_test_tbl1.dropColumn("filed2");
    modify_test_tbl1.addColumn("filed10").number().notNull();
    modify_test_tbl1.alterColumn("filed4").string(1500).notNull();
    std::vector<std::string> vQueries1_1 = pStorage->prepareSqlQueries(modify_test_tbl1);
    if (compareN(bTestSuccess, "modifyTestTable1", vQueries1_1.size(), 4)) {
        compareS(bTestSuccess, "modifyTestTable1", vQueries1_1[0], "ALTER TABLE `test_tbl1` DROP COLUMN `filed1`;");
        compareS(bTestSuccess, "modifyTestTable1", vQueries1_1[1], "ALTER TABLE `test_tbl1` DROP COLUMN `filed2`;");
        compareS(bTestSuccess, "modifyTestTable1", vQueries1_1[2], "ALTER TABLE `test_tbl1` ADD COLUMN `filed10` INT NOT NULL;");
        compareS(bTestSuccess, "modifyTestTable1", vQueries1_1[3], "ALTER TABLE `test_tbl1` MODIFY `filed4` VARCHAR(1500) NOT NULL;");
    }
    pStorage->addStorageChanges(modify_test_tbl1); // add just runtime storage changes
}

// ---------------------------------------------------------------------

void UnitTestStorageStruct::checkModifiedTable1(bool &bTestSuccess, Storage *pStorage) {
    StorageTable tableDef = pStorage->getTableDef("test_tbl1");
    std::vector<StorageColumnDef> vColumns = tableDef.getColumns();
    if (!compareN(bTestSuccess, "checkModifiedTable1", vColumns.size(), 9)) {
        return;
    }
    std::vector<StorageColumnDef> vExpectedColumns;
    vExpectedColumns.push_back(StorageColumnDef("id").number().autoIncrement().primaryKey().notNull());
    vExpectedColumns.push_back(StorageColumnDef("filed3").datetime().notNull());
    vExpectedColumns.push_back(StorageColumnDef("filed4").string(1500).notNull().enableIndex());
    vExpectedColumns.push_back(StorageColumnDef("filed5").number().notNull().enableUniqueIndex("idx_f5_and_f7"));
    vExpectedColumns.push_back(StorageColumnDef("filed6").number().notNull().enableUniqueIndex("idx_f6_and_f8"));
    vExpectedColumns.push_back(StorageColumnDef("filed7").number().notNull().enableUniqueIndex("idx_f5_and_f7"));
    vExpectedColumns.push_back(StorageColumnDef("filed8").number().notNull().enableUniqueIndex("idx_f6_and_f8"));
    vExpectedColumns.push_back(StorageColumnDef("filed9").doubleNumber().defaultValue("0.0"));
    vExpectedColumns.push_back(StorageColumnDef("filed10").number().notNull());


    for (int i = 0; i < vColumns.size(); i++) {
        StorageColumnDef c = vColumns[i];
        StorageColumnDef expectedC = vExpectedColumns[i];
        compareB(bTestSuccess, "isAutoIncrement for " + c.columnName(), c.isAutoIncrement(), expectedC.isAutoIncrement());
        compareB(bTestSuccess, "isPrimaryKey for " + c.columnName(), c.isPrimaryKey(), expectedC.isPrimaryKey());
        compareB(bTestSuccess, "isNotNull for " + c.columnName(), c.isNotNull(), expectedC.isNotNull());
        compareB(bTestSuccess, "isEnableIndex for " + c.columnName(), c.isEnableIndex(), expectedC.isEnableIndex());
        compareB(bTestSuccess, "isEnableUniqueIndex for " + c.columnName(), c.isEnableUniqueIndex(), expectedC.isEnableUniqueIndex());
        compareB(bTestSuccess, "isDefaultValue for " + c.columnName(), c.isDefaultValue(), expectedC.isDefaultValue());
        compareS(bTestSuccess, "nameUniqueIndex for " + c.columnName(), c.nameOfUniqueIndex(), expectedC.nameOfUniqueIndex());
        compareS(bTestSuccess, "columnName for " + c.columnName(), c.columnName(), expectedC.columnName());
        compareS(bTestSuccess, "columnType for " + c.columnName(), c.columnType(), expectedC.columnType());
        compareS(bTestSuccess, "columnDefaultValue for " + c.columnName(), c.columnDefaultValue(), expectedC.columnDefaultValue());

        // columnTypeSize
        // compareB(bTestSuccess, "primarykey for " + c.columnName(), c.isPrimaryKey(), expectedC.isPrimaryKey());
    }

    // StorageTable *pTableDef = pStorage->getTables()["test_tbl1"];

    // TEST merge structs
    /*if (!test_tbl1.mergeWith(modify_test_tbl1)) {
        WSJCppLog::err(TAG, "Problem with merge structs");
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
    }*/
}

// ---------------------------------------------------------------------

void UnitTestStorageStruct::insertTestTable1(bool &bTestSuccess, Storage *pStorage) {
    StorageTable tableDef = pStorage->getTableDef("test_tbl1");

    StorageInsert tbl1_ins("test_tbl1");
    tbl1_ins.bindValue("filed3", "so\"me"); // TODO datetime
    tbl1_ins.bindValue("filed4", "some");
    tbl1_ins.bindValue("filed5", 123);
    tbl1_ins.bindValue("filed6", 321);
    tbl1_ins.bindValue("filed7", 456);
    tbl1_ins.bindValue("filed8", 654);
    tbl1_ins.bindValue("filed9", 555.0);
    tbl1_ins.bindValue("filed10", 789);
    if (!tbl1_ins.isValid(tableDef)) {
        bTestSuccess = false;
    }
    std::vector<std::string> vQueries1_ins = pStorage->prepareSqlQueries(tbl1_ins);
    if (compareN(bTestSuccess, "insertTestTable1", vQueries1_ins.size(), 1)) {
        compareS(bTestSuccess, "insertTestTable1", vQueries1_ins[0], "INSERT INTO test_tbl1"
            "(filed3, filed4, filed5, filed6, filed7, filed8, filed9, filed10) "
            "VALUES(\"so\\\"me\", \"some\", 123, 321, 456, 654, 555.000000, 789);");
    }
}

// ---------------------------------------------------------------------


void UnitTestStorageStruct::createTestTable3(bool &bTestSuccess, Storage *pStorage) {
    StorageCreateTable test_tbl3("test_tbl3");
    test_tbl3.addColumn("id").number().autoIncrement().primaryKey().notNull();
    test_tbl3.addColumn("field1").string(123).notNull();
    test_tbl3.addColumn("field2").string(223).notNull();
    test_tbl3.addColumn("field3").string(323).notNull();
    std::vector<std::string> vQueries1 = pStorage->prepareSqlQueries(test_tbl3);
    if (compareN(bTestSuccess, "createTestTable3", vQueries1.size(), 1)) {
        compareS(bTestSuccess, "createTestTable3", vQueries1[0], 
            "CREATE TABLE IF NOT EXISTS `test_tbl3` (\r\n"
            "  `id` INT NOT NULL AUTO_INCREMENT,\r\n"
            "  `field1` VARCHAR(123) NOT NULL,\r\n"
            "  `field2` VARCHAR(223) NOT NULL,\r\n"
            "  `field3` VARCHAR(323) NOT NULL,\r\n"
            "  PRIMARY KEY (id)\r\n"
            ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    }
    pStorage->addStorageChanges(test_tbl3); // add just runtime storage changes
}

// ---------------------------------------------------------------------

void UnitTestStorageStruct::modifyTestTable3(bool &bTestSuccess, Storage *pStorage) {
    std::string fn = "modifyTestTable3";
    StorageModifyTable test_tbl3("test_tbl3");
    test_tbl3.dropColumn("field2");
    test_tbl3.dropColumn("field3");
    test_tbl3.addColumn("field5").string(123).notNull();
    test_tbl3.alterColumn("field1").string(124).notNull();
    std::vector<std::string> vQueries3 = pStorage->prepareSqlQueries(test_tbl3);
    if (compareN(bTestSuccess, fn, vQueries3.size(), 4)) {
        compareS(bTestSuccess, fn, vQueries3[0], "ALTER TABLE `test_tbl3` DROP COLUMN `field2`;");
        compareS(bTestSuccess, fn, vQueries3[1], "ALTER TABLE `test_tbl3` DROP COLUMN `field3`;");
        compareS(bTestSuccess, fn, vQueries3[2], "ALTER TABLE `test_tbl3` ADD COLUMN `field5` VARCHAR(123) NOT NULL;");
        compareS(bTestSuccess, fn, vQueries3[3], "ALTER TABLE `test_tbl3` MODIFY `field1` VARCHAR(124) NOT NULL;");
    }
    pStorage->addStorageChanges(test_tbl3);
}