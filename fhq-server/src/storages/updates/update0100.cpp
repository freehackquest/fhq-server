#include <update0100.h>

REGISTRY_STORAGE_UPDATE(Update0100)

Update0100::Update0100()
    : StorageUpdateBase("u0099", "u0100", "Update table quests_writeups") {
    
    // fill the array with struct changes
    StorageModifyTable *quests_writeups = modifyTable("quests_writeups");
    quests_writeups->addColumn("approve").number().notNull().defaultValue("1");
    quests_writeups->addColumn("userid").number().notNull().defaultValue("0");
}
