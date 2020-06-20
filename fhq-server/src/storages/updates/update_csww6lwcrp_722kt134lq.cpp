#include "update_csww6lwcrp_722kt134lq.h"

REGISTRY_WSJCPP_STORAGE_UPDATE(Update_csww6lwcrp_722kt134lq)

Update_csww6lwcrp_722kt134lq::Update_csww6lwcrp_722kt134lq()
    : WsjcppStorageUpdateBase("csww6lwcrp", "722kt134lq", "useful_links - tags, favorites, comments") {
    
    // useful_links_user_favorites
    WsjcppStorageCreateTable *useful_links_user_favorites = createTable("useful_links_user_favorites");
    useful_links_user_favorites->addColumn("id").number().autoIncrement().primaryKey().notNull();
    useful_links_user_favorites->addColumn("usefullinkid").number().notNull().defaultValue("0");
    useful_links_user_favorites->addColumn("userid").number().notNull().defaultValue("0");
    useful_links_user_favorites->addColumn("dt").datetime().notNull();

    // useful_links_comments
    WsjcppStorageCreateTable *useful_links_comments = createTable("useful_links_comments");
    useful_links_comments->addColumn("id").number().autoIncrement().primaryKey().notNull();
    useful_links_comments->addColumn("usefullinkid").number().notNull().defaultValue("0");
    useful_links_comments->addColumn("userid").number().notNull().defaultValue("0");
    useful_links_comments->addColumn("comment").string(1024).notNull().defaultValue("");
    useful_links_comments->addColumn("dt").datetime().notNull();


    // useful_links_comments
    WsjcppStorageCreateTable *useful_links_tags = createTable("useful_links_tags");
    useful_links_tags->addColumn("id").number().autoIncrement().primaryKey().notNull();
    useful_links_tags->addColumn("usefullinkid").number().notNull().defaultValue("0");
    useful_links_tags->addColumn("tagvalue").string(32).notNull().defaultValue("");
    useful_links_tags->addColumn("counter").number().notNull().defaultValue("0");
    
    dropTable("useful_links_stars");

    WsjcppStorageModifyTable *useful_links = modifyTable("useful_links");
    useful_links->addColumn("user_favorites").number().notNull().defaultValue("0");
    useful_links->addColumn("user_clicks").number().notNull().defaultValue("0");
    useful_links->addColumn("user_comments").number().notNull().defaultValue("0");
    useful_links_tags->addColumn("tags").string(1024).notNull().defaultValue("");
}