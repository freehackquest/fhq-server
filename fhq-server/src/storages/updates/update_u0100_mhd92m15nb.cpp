#include "update_u0100_mhd92m15nb.h"

REGISTRY_STORAGE_UPDATE(Update_u0100_mhd92m15nb)

Update_u0100_mhd92m15nb::Update_u0100_mhd92m15nb()
    : StorageUpdateBase("u0100", "mhd92m15nb", "Creating table useful_links") {
    
    // fill the array with struct changes
    StorageStruct useful_links("useful_links", StorageStructTableMode::CREATE);
    useful_links.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
    useful_links.addColumn(StorageStructColumn("url").string(2048).notNull().defaultValue(""));
    useful_links.addColumn(StorageStructColumn("description").string(2048).notNull().defaultValue(""));
    useful_links.addColumn(StorageStructColumn("author").string(127).notNull().defaultValue(""));
    useful_links.addColumn(StorageStructColumn("stars").number().notNull().defaultValue("0"));
    useful_links.addColumn(StorageStructColumn("dt").datetime().notNull());
    add(useful_links);

    StorageStruct useful_links_stars("useful_links_stars", StorageStructTableMode::CREATE);
    useful_links_stars.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
    useful_links_stars.addColumn(StorageStructColumn("usefullinkid").number().notNull().defaultValue("0"));
    useful_links_stars.addColumn(StorageStructColumn("userid").number().notNull().defaultValue("0"));
    useful_links_stars.addColumn(StorageStructColumn("dt").datetime().notNull());
    add(useful_links_stars);
    
    // fill the array with struct changes
}

bool Update_u0100_mhd92m15nb::custom(Storage *pStorage, StorageConnection *pConn, std::string &error) {
    // here you can migrate data of correction if not just return true;
    return true;
}
