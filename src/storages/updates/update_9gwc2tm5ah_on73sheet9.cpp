#include "update_9gwc2tm5ah_on73sheet9.h"

REGISTRY_WSJCPP_STORAGE_UPDATE(Update_9gwc2tm5ah_on73sheet9)

Update_9gwc2tm5ah_on73sheet9::Update_9gwc2tm5ah_on73sheet9()
    : WsjcppStorageUpdateBase("9gwc2tm5ah", "on73sheet9", "Adde new columns to quests_files") {

  WsjcppStorageModifyTable *quests_files = modifyTable("quests_files");
  quests_files->addColumn("md5").string(32).notNull().defaultValue("");
  quests_files->addColumn("downloads_counter").number().notNull().defaultValue("0");
}
