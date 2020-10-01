
#include <wsjcpp_unit_tests.h>
#include <wsjcpp_storages.h>

class UnitTestStorageStruct : public WsjcppUnitTestBase {
    public:
        UnitTestStorageStruct();
        virtual bool doBeforeTest();
        virtual void executeTest();
        virtual bool doAfterTest();

    private:

        void createTestTable0(WsjcppStorage *pStorage);
        void dropTestTable0(WsjcppStorage *pStorage);
        void createTestTable1(WsjcppStorage *pStorage);
        void modifyTestTable1(WsjcppStorage *pStorage);
        void checkModifiedTable1(WsjcppStorage *pStorage);
        void insertTestTable1(WsjcppStorage *pStorage);
        void createTestTable3(WsjcppStorage *pStorage);
        void modifyTestTable3(WsjcppStorage *pStorage);

};

REGISTRY_WSJCPP_UNIT_TEST(UnitTestStorageStruct)

UnitTestStorageStruct::UnitTestStorageStruct() 
    : WsjcppUnitTestBase("UnitTestStorageStruct") {
    // 
}

// ---------------------------------------------------------------------

bool UnitTestStorageStruct::doBeforeTest() {
    // nothing
    return true;
}

// ---------------------------------------------------------------------

void UnitTestStorageStruct::executeTest() {
    if (!compare("Not supported mysql", WsjcppStorages::support("mysql"), true)) {
        return;
    }
    
    WsjcppStorage *pStorage = WsjcppStorages::create("mysql");
    int nExpectedSize = 0;
    std::string sExpectedQuery = "";

    createTestTable0(pStorage);
    dropTestTable0(pStorage);
    createTestTable1(pStorage);
    modifyTestTable1(pStorage);
    checkModifiedTable1(pStorage);
    insertTestTable1(pStorage);
    createTestTable3(pStorage);
    modifyTestTable3(pStorage);
}

// ---------------------------------------------------------------------

bool UnitTestStorageStruct::doAfterTest() {
    // nothing
    return true;
}

// ---------------------------------------------------------------------

void UnitTestStorageStruct::createTestTable0(WsjcppStorage *pStorage) {
    WsjcppStorageCreateTable test_tbl0("test_tbl0");
    test_tbl0.addColumn("id").number().autoIncrement().primaryKey().notNull();
    test_tbl0.addColumn("filed1").string(255).notNull().enableIndex();
    std::vector<std::string> vQueries0 = pStorage->prepareSqlQueries(test_tbl0);
    if (compare("createTable0", vQueries0.size(), 1)) {
        compare("createTable0", vQueries0[0], 
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

void UnitTestStorageStruct::dropTestTable0(WsjcppStorage *pStorage) {
    WsjcppStorageDropTable test_tbl0_drop("test_tbl0");
    std::vector<std::string> vQueries2 = pStorage->prepareSqlQueries(test_tbl0_drop);
    if (compare("dropTestTable0", vQueries2.size(), 1)) {
        compare("dropTestTable0", vQueries2[0], "DROP TABLE IF EXISTS `test_tbl0`;");
    }
    pStorage->addStorageChanges(test_tbl0_drop);
    compare("Table 'test_tbl0' must be disappeared", pStorage->existsTable("test_tbl0"), false);
}

// ---------------------------------------------------------------------

void UnitTestStorageStruct::createTestTable1(WsjcppStorage *pStorage) {
    WsjcppStorageCreateTable test_tbl1("test_tbl1");
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
    if (compare("createTestTable1", vQueries1.size(), 1)) {
        compare("createTestTable1", vQueries1[0], 
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

void UnitTestStorageStruct::modifyTestTable1(WsjcppStorage *pStorage) {
    WsjcppStorageModifyTable modify_test_tbl1("test_tbl1");
    modify_test_tbl1.dropColumn("filed1");
    modify_test_tbl1.dropColumn("filed2");
    modify_test_tbl1.addColumn("filed10").number().notNull();
    modify_test_tbl1.alterColumn("filed4").string(1500).notNull();
    std::vector<std::string> vQueries1_1 = pStorage->prepareSqlQueries(modify_test_tbl1);
    if (compare("modifyTestTable1", vQueries1_1.size(), 4)) {
        compare("modifyTestTable1", vQueries1_1[0], "ALTER TABLE `test_tbl1` DROP COLUMN `filed1`;");
        compare("modifyTestTable1", vQueries1_1[1], "ALTER TABLE `test_tbl1` DROP COLUMN `filed2`;");
        compare("modifyTestTable1", vQueries1_1[2], "ALTER TABLE `test_tbl1` ADD COLUMN `filed10` INT NOT NULL;");
        compare("modifyTestTable1", vQueries1_1[3], "ALTER TABLE `test_tbl1` MODIFY `filed4` VARCHAR(1500) NOT NULL;");
    }
    pStorage->addStorageChanges(modify_test_tbl1); // add just runtime storage changes
}

// ---------------------------------------------------------------------

void UnitTestStorageStruct::checkModifiedTable1(WsjcppStorage *pStorage) {
    WsjcppStorageTable tableDef = pStorage->getTableDef("test_tbl1");
    std::vector<WsjcppStorageColumnDef> vColumns = tableDef.getColumns();
    if (!compare("checkModifiedTable1", vColumns.size(), 9)) {
        return;
    }
    std::vector<WsjcppStorageColumnDef> vExpectedColumns;
    vExpectedColumns.push_back(WsjcppStorageColumnDef("id").number().autoIncrement().primaryKey().notNull());
    vExpectedColumns.push_back(WsjcppStorageColumnDef("filed3").datetime().notNull());
    vExpectedColumns.push_back(WsjcppStorageColumnDef("filed4").string(1500).notNull().enableIndex());
    vExpectedColumns.push_back(WsjcppStorageColumnDef("filed5").number().notNull().enableUniqueIndex("idx_f5_and_f7"));
    vExpectedColumns.push_back(WsjcppStorageColumnDef("filed6").number().notNull().enableUniqueIndex("idx_f6_and_f8"));
    vExpectedColumns.push_back(WsjcppStorageColumnDef("filed7").number().notNull().enableUniqueIndex("idx_f5_and_f7"));
    vExpectedColumns.push_back(WsjcppStorageColumnDef("filed8").number().notNull().enableUniqueIndex("idx_f6_and_f8"));
    vExpectedColumns.push_back(WsjcppStorageColumnDef("filed9").doubleNumber().defaultValue("0.0"));
    vExpectedColumns.push_back(WsjcppStorageColumnDef("filed10").number().notNull());


    for (int i = 0; i < vColumns.size(); i++) {
        WsjcppStorageColumnDef c = vColumns[i];
        WsjcppStorageColumnDef expectedC = vExpectedColumns[i];
        compare("isAutoIncrement for " + c.columnName(), c.isAutoIncrement(), expectedC.isAutoIncrement());
        compare("isPrimaryKey for " + c.columnName(), c.isPrimaryKey(), expectedC.isPrimaryKey());
        compare("isNotNull for " + c.columnName(), c.isNotNull(), expectedC.isNotNull());
        compare("isEnableIndex for " + c.columnName(), c.isEnableIndex(), expectedC.isEnableIndex());
        compare("isEnableUniqueIndex for " + c.columnName(), c.isEnableUniqueIndex(), expectedC.isEnableUniqueIndex());
        compare("isDefaultValue for " + c.columnName(), c.isDefaultValue(), expectedC.isDefaultValue());
        compare("nameUniqueIndex for " + c.columnName(), c.nameOfUniqueIndex(), expectedC.nameOfUniqueIndex());
        compare("columnName for " + c.columnName(), c.columnName(), expectedC.columnName());
        compare("columnType for " + c.columnName(), c.columnType(), expectedC.columnType());
        compare("columnDefaultValue for " + c.columnName(), c.columnDefaultValue(), expectedC.columnDefaultValue());

        // columnTypeSize
        // compare("primarykey for " + c.columnName(), c.isPrimaryKey(), expectedC.isPrimaryKey());
    }

    // StorageTable *pTableDef = pStorage->getTables()["test_tbl1"];

    // TEST merge structs
    /*if (!test_tbl1.mergeWith(modify_test_tbl1)) {
        WsjcppLog::err(TAG, "Problem with merge structs");
    }

    std::vector<std::string> vQueries1_2 = pStorage->prepareSqlQueries(test_tbl1);
    if (!compareN("vQueries1_2", vQueries1_2.size(), 1)) {
    } else {
        compare(vQueries1_2[0],
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

void UnitTestStorageStruct::insertTestTable1(WsjcppStorage *pStorage) {
    WsjcppStorageTable tableDef = pStorage->getTableDef("test_tbl1");

    WsjcppStorageInsert tbl1_ins("test_tbl1");
    tbl1_ins.bindValue("filed3", "so\"me"); // TODO datetime
    tbl1_ins.bindValue("filed4", "some");
    tbl1_ins.bindValue("filed5", 123);
    tbl1_ins.bindValue("filed6", 321);
    tbl1_ins.bindValue("filed7", 456);
    tbl1_ins.bindValue("filed8", 654);
    tbl1_ins.bindValue("filed9", 555.0);
    tbl1_ins.bindValue("filed10", 789);
    compare("tbl1_ins", tbl1_ins.isValid(tableDef), true);
    std::vector<std::string> vQueries1_ins = pStorage->prepareSqlQueries(tbl1_ins);
    if (compare("insertTestTable1", vQueries1_ins.size(), 1)) {
        compare("insertTestTable1", vQueries1_ins[0], "INSERT INTO test_tbl1"
            "(filed3, filed4, filed5, filed6, filed7, filed8, filed9, filed10) "
            "VALUES(\"so\\\"me\", \"some\", 123, 321, 456, 654, 555.000000, 789);");
    }
}

// ---------------------------------------------------------------------


void UnitTestStorageStruct::createTestTable3(WsjcppStorage *pStorage) {
    WsjcppStorageCreateTable test_tbl3("test_tbl3");
    test_tbl3.addColumn("id").number().autoIncrement().primaryKey().notNull();
    test_tbl3.addColumn("field1").string(123).notNull();
    test_tbl3.addColumn("field2").string(223).notNull();
    test_tbl3.addColumn("field3").string(323).notNull();
    std::vector<std::string> vQueries1 = pStorage->prepareSqlQueries(test_tbl3);
    if (compare("createTestTable3", vQueries1.size(), 1)) {
        compare("createTestTable3", vQueries1[0], 
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

void UnitTestStorageStruct::modifyTestTable3(WsjcppStorage *pStorage) {
    std::string fn = "modifyTestTable3";
    WsjcppStorageModifyTable test_tbl3("test_tbl3");
    test_tbl3.dropColumn("field2");
    test_tbl3.dropColumn("field3");
    test_tbl3.addColumn("field5").string(123).notNull();
    test_tbl3.alterColumn("field1").string(124).notNull();
    std::vector<std::string> vQueries3 = pStorage->prepareSqlQueries(test_tbl3);
    if (compare(fn, vQueries3.size(), 4)) {
        compare(fn, vQueries3[0], "ALTER TABLE `test_tbl3` DROP COLUMN `field2`;");
        compare(fn, vQueries3[1], "ALTER TABLE `test_tbl3` DROP COLUMN `field3`;");
        compare(fn, vQueries3[2], "ALTER TABLE `test_tbl3` ADD COLUMN `field5` VARCHAR(123) NOT NULL;");
        compare(fn, vQueries3[3], "ALTER TABLE `test_tbl3` MODIFY `field1` VARCHAR(124) NOT NULL;");
    }
    pStorage->addStorageChanges(test_tbl3);
}