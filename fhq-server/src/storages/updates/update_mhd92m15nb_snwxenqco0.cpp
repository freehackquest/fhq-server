#include "update_mhd92m15nb_snwxenqco0.h"

REGISTRY_STORAGE_UPDATE(Update_mhd92m15nb_snwxenqco0)

Update_mhd92m15nb_snwxenqco0::Update_mhd92m15nb_snwxenqco0()
    : StorageUpdateBase("mhd92m15nb", "snwxenqco0", "Added tables chats, chats_users, chats_messages") {
    
    StorageCreateTable *chat = createTable("chats");
    chat->addColumn("id").number().autoIncrement().primaryKey().notNull();
    chat->addColumn("uuid").string(127).notNull().defaultValue("");
    chat->addColumn("name").string(127).notNull().defaultValue("");
    chat->addColumn("type").string(127).notNull().defaultValue("");
    chat->addColumn("owner").number().notNull().defaultValue("0");
    chat->addColumn("last_message_id").datetime().notNull();
    chat->addColumn("created").datetime().notNull();
    chat->addColumn("updated").datetime().notNull();

    StorageCreateTable *chats_users = createTable("chats_users");
    chats_users->addColumn("id").number().autoIncrement().primaryKey().notNull();
    chats_users->addColumn("chatid").number().notNull().defaultValue("0");
    chats_users->addColumn("userid").number().notNull().defaultValue("0");
    chats_users->addColumn("chat_role").string(127).notNull().defaultValue(""); // TODO enums: invited/blocked/admin/user
    chats_users->addColumn("visited").datetime().notNull();

    StorageCreateTable *chats_messages = createTable("chats_messages");
    chats_messages->addColumn("id").number().autoIncrement().primaryKey().notNull();
    chats_messages->addColumn("chatid").number().notNull().defaultValue("0");
    chats_messages->addColumn("userid").number().notNull().defaultValue("0");
    chats_messages->addColumn("message").string(1024).notNull().defaultValue("");
    chats_messages->addColumn("status").string(127).notNull().defaultValue("");
    chats_messages->addColumn("created").datetime().notNull();
    chats_messages->addColumn("updated").datetime().notNull();
}
