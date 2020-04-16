#include "update_u0100_mhd92m15nb.h"

REGISTRY_WSJCPP_STORAGE_UPDATE(Update_u0100_mhd92m15nb)

Update_u0100_mhd92m15nb::Update_u0100_mhd92m15nb()
    : WsjcppStorageUpdateBase("u0100", "mhd92m15nb", "Creating table useful_links") {
    
    WsjcppStorageCreateTable *useful_links = createTable("useful_links");
    useful_links->addColumn("id").number().autoIncrement().primaryKey().notNull();
    useful_links->addColumn("url").string(2048).notNull().defaultValue("");
    useful_links->addColumn("description").string(2048).notNull().defaultValue("");
    useful_links->addColumn("author").string(127).notNull().defaultValue("");
    useful_links->addColumn("stars").number().notNull().defaultValue("0");
    useful_links->addColumn("dt").datetime().notNull();

    WsjcppStorageCreateTable *useful_links_stars = createTable("useful_links_stars");
    useful_links_stars->addColumn("id").number().autoIncrement().primaryKey().notNull();
    useful_links_stars->addColumn("usefullinkid").number().notNull().defaultValue("0");
    useful_links_stars->addColumn("userid").number().notNull().defaultValue("0");
    useful_links_stars->addColumn("dt").datetime().notNull();
}