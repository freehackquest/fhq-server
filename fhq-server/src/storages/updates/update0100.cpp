#include <update0100.h>

REGISTRY_STORAGE_UPDATE(Update0100)

Update0100::Update0100()
    : StorageUpdateBase("u0099", "u0100", "Update table quests_writeups") {
    
    // fill the array with struct changes
    StorageStruct quests_writeups("quests_writeups", StorageStructTableMode::ALTER);
    quests_writeups.addColumn(StorageStructColumn("approve").number().notNull().defaultValue("1"));
    quests_writeups.addColumn(StorageStructColumn("userid").number().notNull().defaultValue("0"));
    m_vStructChanges.push_back(quests_writeups);

    // will be automaticly applied by update algorithm
}

bool Update0100::custom(Storage *pStorage, StorageConnection *pConn, std::string &error) {
    // here you can migrate data of correction if not just return true;
    return true;
}
