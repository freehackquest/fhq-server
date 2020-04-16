#include <update0100.h>

REGISTRY_WSJCPP_STORAGE_UPDATE(Update0100)

Update0100::Update0100()
    : WsjcppStorageUpdateBase("u0099", "u0100", "Update table quests_writeups") {
    
    // fill the array with struct changes
    WsjcppStorageModifyTable *quests_writeups = modifyTable("quests_writeups");
    quests_writeups->addColumn("approve").number().notNull().defaultValue("1");
    quests_writeups->addColumn("userid").number().notNull().defaultValue("0");
}
