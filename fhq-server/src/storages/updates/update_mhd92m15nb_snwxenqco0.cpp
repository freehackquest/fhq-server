#include "update_mhd92m15nb_snwxenqco0.h"

REGISTRY_STORAGE_UPDATE(Update_mhd92m15nb_snwxenqco0)

Update_mhd92m15nb_snwxenqco0::Update_mhd92m15nb_snwxenqco0()
    : StorageUpdateBase("mhd92m15nb", "snwxenqco0", "TODO") {
    
    StorageCreateTable *chat = createTable("chat");
    chat->addColumn("id").number().autoIncrement().primaryKey().notNull();
    chat->addColumn("type").string(127).notNull().defaultValue("");
    chat->addColumn("owner").string(127).notNull().defaultValue("");
    chat->addColumn("dt_last_message").datetime().notNull();

    StorageCreateTable *chat_users = createTable("chat_users");
    chat_users->addColumn("id").number().autoIncrement().primaryKey().notNull();
    chat_users->addColumn("chat").number().notNull().defaultValue("0");
    chat_users->addColumn("user").string(127).notNull().defaultValue("");
    chat_users->addColumn("chat_role").string(127).notNull().defaultValue("");
    chat_users->addColumn("last_date").datetime().notNull();

    StorageModifyTable *chatmessages = modifyTable("chatmessages");
    chatmessages->addColumn("status").string(127).notNull().defaultValue("");
}
