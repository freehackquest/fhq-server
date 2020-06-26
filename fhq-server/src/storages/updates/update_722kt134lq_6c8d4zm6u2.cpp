#include "update_722kt134lq_6c8d4zm6u2.h"

REGISTRY_WSJCPP_STORAGE_UPDATE(Update_722kt134lq_6c8d4zm6u2)

Update_722kt134lq_6c8d4zm6u2::Update_722kt134lq_6c8d4zm6u2()
    : WsjcppStorageUpdateBase("722kt134lq", "6c8d4zm6u2", "TODO") {
    
    WsjcppStorageModifyTable *useful_links_tags = modifyTable("useful_links_tags");
    useful_links_tags->dropColumn("tags");
    useful_links_tags->dropColumn("counter");

    WsjcppStorageModifyTable *useful_links = modifyTable("useful_links");
    useful_links->addColumn("tags").string(1024).notNull().defaultValue("");
    useful_links->dropColumn("stars");

    // fill the array with struct changes
}

