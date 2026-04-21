/**********************************************************************************
 *                      Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * MIT License
 *
 * Copyright (c) 2011-2026 FreeHackQuest <freehackquest@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Original repository: https://github.com/freehackquest/fhq-server
 *
 ***********************************************************************************/

#include <vector>
#include <iostream>

#include <wsjcpp_storages.h>

int createTestTable0(WsjcppStorage *pStorage) {
  WsjcppStorageCreateTable test_tbl0("test_tbl0");
  test_tbl0.addColumn("id").number().autoIncrement().primaryKey().notNull();
  test_tbl0.addColumn("filed1").string(255).notNull().enableIndex();
  std::vector<std::string> vQueries0 = pStorage->prepareSqlQueries(test_tbl0);
  if (vQueries0.size() != 1) {
    std::cerr << "Problem with createTestTable0" << std::endl;
    return 1;
  }
  std::string sql_expected =
    "CREATE TABLE IF NOT EXISTS `test_tbl0` (\r\n"
    "  `id` INT NOT NULL AUTO_INCREMENT,\r\n"
    "  `filed1` VARCHAR(255) NOT NULL,\r\n"
    "  PRIMARY KEY (id),\r\n"
    "  KEY idx_filed1 (filed1)\r\n"
    ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;"
  ;
  if (vQueries0[0] != sql_expected) {
    std::cerr << "Expected '" << sql_expected << "', but got '" << vQueries0[0] << "'" << std::endl;
    return 1;
  }

  pStorage->addStorageChanges(test_tbl0); // add just runtime storage changes
  return 0;
}

int dropTestTable0(WsjcppStorage *pStorage) {
  WsjcppStorageDropTable test_tbl0_drop("test_tbl0");
  std::vector<std::string> vQueries2 = pStorage->prepareSqlQueries(test_tbl0_drop);
  if (vQueries2.size() != 1) {
    std::cerr << "Problem with dropTestTable0" << std::endl;
    return 1;
  }
  std::string sql_expected = "DROP TABLE IF EXISTS `test_tbl0`;";
  if (vQueries2[0] != sql_expected) {
    std::cerr << "Expected '" << sql_expected << "', but got '" << vQueries2[0] << "'" << std::endl;
    return 1;
  }
  pStorage->addStorageChanges(test_tbl0_drop);
  if (pStorage->existsTable("test_tbl0") != false)
  {
    std::cerr << "Table 'test_tbl0' must be disappeared" << std::endl;
    return 1;
  }
  return 0;
}

int createTestTable1(WsjcppStorage *pStorage) {
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
  if (vQueries1.size() != 1) {
    std::cerr << "Problem with createTestTable1" << std::endl;
    return 1;
  }
  std::string sql_expected =
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
    ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;"
  ;
  if (vQueries1[0] != sql_expected) {
    std::cerr << "Expected '" << sql_expected << "', but got '" << vQueries1[0] << "'" << std::endl;
    return 1;
  }
  pStorage->addStorageChanges(test_tbl1); // add just runtime storage changes
  return 0;
}

int modifyTestTable1(WsjcppStorage *pStorage) {
  WsjcppStorageModifyTable modify_test_tbl1("test_tbl1");
  modify_test_tbl1.dropColumn("filed1");
  modify_test_tbl1.dropColumn("filed2");
  modify_test_tbl1.addColumn("filed10").number().notNull();
  modify_test_tbl1.alterColumn("filed4").string(1500).notNull();
  std::vector<std::string> vQueries1_1 = pStorage->prepareSqlQueries(modify_test_tbl1);
  if (vQueries1_1.size() != 4) {
    std::cerr << "Problem with modifyTestTable1" << std::endl;
    return 1;
  }
  std::string sql_expected = "ALTER TABLE `test_tbl1` DROP COLUMN `filed1`;";
  if (vQueries1_1[0] != sql_expected) {
    std::cerr << "Expected '" << sql_expected << "', but got '" << vQueries1_1[0] << "'" << std::endl;
    return 1;
  }
  sql_expected = "ALTER TABLE `test_tbl1` DROP COLUMN `filed2`;";
  if (vQueries1_1[1] != sql_expected) {
    std::cerr << "Expected '" << sql_expected << "', but got '" << vQueries1_1[1] << "'" << std::endl;
    return 1;
  }
  sql_expected = "ALTER TABLE `test_tbl1` ADD COLUMN `filed10` INT NOT NULL;";
  if (vQueries1_1[2] != sql_expected) {
    std::cerr << "Expected '" << sql_expected << "', but got '" << vQueries1_1[2] << "'" << std::endl;
    return 1;
  }
  sql_expected = "ALTER TABLE `test_tbl1` MODIFY `filed4` VARCHAR(1500) NOT NULL;";
  if (vQueries1_1[3] != sql_expected) {
    std::cerr << "Expected '" << sql_expected << "', but got '" << vQueries1_1[3] << "'" << std::endl;
    return 1;
  }
  pStorage->addStorageChanges(modify_test_tbl1); // add just runtime storage changes
  return 0;
}

int checkModifiedTable1(WsjcppStorage *pStorage) {
    WsjcppStorageTable tableDef = pStorage->getTableDef("test_tbl1");
    std::vector<WsjcppStorageColumnDef> vColumns = tableDef.getColumns();
    if (vColumns.size() != 9) {
      std::cerr << "Problem with checkModifiedTable1" << std::endl;
      return 1;
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
      if (c.isAutoIncrement() != expectedC.isAutoIncrement()) {
        std::cerr << "Problem with isAutoIncrement for " << c.columnName() << std::endl;
        return 1;
      }
      if (c.isAutoIncrement() != expectedC.isAutoIncrement()) {
        std::cerr << "Problem with isAutoIncrement for " << c.columnName() << std::endl;
        return 1;
      }
      if (c.isPrimaryKey() != expectedC.isPrimaryKey()) {
        std::cerr << "Problem with isPrimaryKey for " << c.columnName() << std::endl;
        return 1;
      }
      if (c.isNotNull() != expectedC.isNotNull()) {
        std::cerr << "Problem with isNotNull for " << c.columnName() << std::endl;
        return 1;
      }
      if (c.isEnableIndex() != expectedC.isEnableIndex()) {
        std::cerr << "Problem with isEnableIndex for " << c.columnName() << std::endl;
        return 1;
      }
      if (c.isEnableUniqueIndex() != expectedC.isEnableUniqueIndex()) {
        std::cerr << "Problem with isEnableUniqueIndex for " << c.columnName() << std::endl;
        return 1;
      }
      if (c.isDefaultValue() != expectedC.isDefaultValue()) {
        std::cerr << "Problem with isDefaultValue for " << c.columnName() << std::endl;
        return 1;
      }
      if (c.nameOfUniqueIndex() != expectedC.nameOfUniqueIndex()) {
        std::cerr << "Problem with nameUniqueIndex for " << c.columnName() << std::endl;
        return 1;
      }
      if (c.columnName() != expectedC.columnName()) {
        std::cerr << "Problem with columnName for " << c.columnName() << std::endl;
        return 1;
      }
      if (c.columnType() != expectedC.columnType()) {
        std::cerr << "Problem with columnType for " << c.columnName() << std::endl;
        return 1;
      }
      if (c.columnDefaultValue() != expectedC.columnDefaultValue()) {
        std::cerr << "Problem with columnDefaultValue for " << c.columnName() << std::endl;
        return 1;
      }

      // columnTypeSize
      // compare("primarykey for " + c.columnName(), c.isPrimaryKey(), expectedC.isPrimaryKey());
    }

  // StorageTable *pTableDef = pStorage->getTables()["test_tbl1"];

  // TODO
  // // TEST merge structs
  // if (!test_tbl1.mergeWith(modify_test_tbl1)) {
  //     WsjcppLog::err(TAG, "Problem with merge structs");
  // }
  // std::vector<std::string> vQueries1_2 = pStorage->prepareSqlQueries(test_tbl1);
  // if (!compareN("vQueries1_2", vQueries1_2.size(), 1)) {
  // } else {
  //     compare(vQueries1_2[0],
  //       "CREATE TABLE IF NOT EXISTS `test_tbl1` (\r\n"
  //       "  `id` INT NOT NULL AUTO_INCREMENT,\r\n"
  //       "  `filed3` DATETIME NOT NULL,\r\n"
  //       "  `filed4` VARCHAR(1500) NOT NULL,\r\n"
  //       "  `filed5` INT NOT NULL,\r\n"
  //       "  `filed6` INT NOT NULL,\r\n"
  //       "  `filed7` INT NOT NULL,\r\n"
  //       "  `filed8` INT NOT NULL,\r\n"
  //       "  `filed9` DOUBLE DEFAULT 0.0,\r\n"
  //       "  `filed10` INT NOT NULL,\r\n"
  //       "  PRIMARY KEY (id),\r\n"
  //       "  KEY idx_filed4 (filed4(255)),\r\n"
  //       "  UNIQUE KEY idx_f5_and_f7 (filed5,filed7),\r\n"
  //       "  UNIQUE KEY idx_f6_and_f8 (filed6,filed8)\r\n"
  //       ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
  // }
  return 0;
}

int insertTestTable1(WsjcppStorage *pStorage) {
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
  if (!tbl1_ins.isValid(tableDef)) {
    std::cerr << "Problem with insertTestTable1" << std::endl;
    return 1;
  }
  std::vector<std::string> vQueries1_ins = pStorage->prepareSqlQueries(tbl1_ins);
  if (vQueries1_ins.size() != 1) {
    std::cerr << "Problem with insertTestTable1" << std::endl;
    return 1;
  }
  std::string sql_expected =
    "INSERT INTO test_tbl1"
    "(filed3, filed4, filed5, filed6, filed7, filed8, filed9, filed10) "
    "VALUES(\"so\\\"me\", \"some\", 123, 321, 456, 654, 555.000000, 789);"
  ;
  if (vQueries1_ins[0] != sql_expected) {
    std::cerr << "Expected '" << sql_expected << "', but got '" << vQueries1_ins[0] << "'" << std::endl;
    return 1;
  }
  return 0;
}

int createTestTable3(WsjcppStorage *pStorage) {
  WsjcppStorageCreateTable test_tbl3("test_tbl3");
  test_tbl3.addColumn("id").number().autoIncrement().primaryKey().notNull();
  test_tbl3.addColumn("field1").string(123).notNull();
  test_tbl3.addColumn("field2").string(223).notNull();
  test_tbl3.addColumn("field3").string(323).notNull();
  std::vector<std::string> vQueries1 = pStorage->prepareSqlQueries(test_tbl3);
  if (vQueries1.size() != 1) {
    std::cerr << "Problem with createTestTable3" << std::endl;
    return 1;
  }
  std::string sql_expected =
    "CREATE TABLE IF NOT EXISTS `test_tbl3` (\r\n"
    "  `id` INT NOT NULL AUTO_INCREMENT,\r\n"
    "  `field1` VARCHAR(123) NOT NULL,\r\n"
    "  `field2` VARCHAR(223) NOT NULL,\r\n"
    "  `field3` VARCHAR(323) NOT NULL,\r\n"
    "  PRIMARY KEY (id)\r\n"
    ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;"
  ;
  std::string sql_got = vQueries1[0];
  if (sql_got != sql_expected) {
    std::cerr << "Expected '" << sql_expected << "', but got '" << sql_got << "'" << std::endl;
    return 1;
  }
  pStorage->addStorageChanges(test_tbl3); // add just runtime storage changes
  return 0;
}

int modifyTestTable3(WsjcppStorage *pStorage) {
  std::string fn = "modifyTestTable3";
  WsjcppStorageModifyTable test_tbl3("test_tbl3");
  test_tbl3.dropColumn("field2");
  test_tbl3.dropColumn("field3");
  test_tbl3.addColumn("field5").string(123).notNull();
  test_tbl3.alterColumn("field1").string(124).notNull();
  std::vector<std::string> vQueries3 = pStorage->prepareSqlQueries(test_tbl3);
  if (vQueries3.size() != 4) {
    std::cerr << "Problem with modifyTestTable3" << std::endl;
    return 1;
  }
  std::string sql_expected = "ALTER TABLE `test_tbl3` DROP COLUMN `field2`;";
  std::string sql_got = vQueries3[0];
  if (sql_got != sql_expected) {
    std::cerr << "Expected '" << sql_expected << "', but got '" << sql_got << "'" << std::endl;
    return 1;
  }
  sql_expected = "ALTER TABLE `test_tbl3` DROP COLUMN `field3`;";
  sql_got = vQueries3[1];
  if (sql_got != sql_expected) {
    std::cerr << "Expected '" << sql_expected << "', but got '" << sql_got << "'" << std::endl;
    return 1;
  }
  sql_expected = "ALTER TABLE `test_tbl3` ADD COLUMN `field5` VARCHAR(123) NOT NULL;";
  sql_got = vQueries3[2];
  if (sql_got != sql_expected) {
    std::cerr << "Expected '" << sql_expected << "', but got '" << sql_got << "'" << std::endl;
    return 1;
  }
  sql_expected = "ALTER TABLE `test_tbl3` MODIFY `field1` VARCHAR(124) NOT NULL;";
  sql_got = vQueries3[3];
  if (sql_got != sql_expected) {
    std::cerr << "Expected '" << sql_expected << "', but got '" << sql_got << "'" << std::endl;
    return 1;
  }
  pStorage->addStorageChanges(test_tbl3);
  return 0;
}

int main() {
  if (!WsjcppStorages::support("mysql")) {
    std::cerr << "Not supported mysql" << std::endl;
    return -1;
  }

  WsjcppStorage *pStorage = WsjcppStorages::create("mysql");

  int result = 0;
  result += createTestTable0(pStorage);
  result += dropTestTable0(pStorage);
  result += createTestTable1(pStorage);
  result += modifyTestTable1(pStorage);
  result += checkModifiedTable1(pStorage);
  result += insertTestTable1(pStorage);
  result += createTestTable3(pStorage);
  result += modifyTestTable3(pStorage);

  return result;
}
