#include <updates_init_database.h>

REGISTRY_STORAGE_UPDATE(UpdatesInitDatabase)

UpdatesInitDatabase::UpdatesInitDatabase()
    : StorageUpdateBase("", "u0099", "Init database u0099") {
    {
        StorageCreateTable *email_delivery = createTable("email_delivery");
        email_delivery->addColumn("id").number().autoIncrement().primaryKey().notNull();
        email_delivery->addColumn("to_email").string(255).notNull();
        email_delivery->addColumn("subject").string(255).notNull();
        email_delivery->addColumn("message").text().notNull();
        email_delivery->addColumn("priority").string(255).notNull();
        email_delivery->addColumn("status").string(255).notNull();
        email_delivery->addColumn("dt").datetime().notNull();
    }

    {
        StorageCreateTable *feedback = createTable("feedback");
        feedback->addColumn("id").number().autoIncrement().primaryKey().notNull();
        feedback->addColumn("dt").datetime().notNull();
        feedback->addColumn("type").string(255).notNull().defaultValue("''");
        feedback->addColumn("text").string(255).notNull();
        feedback->addColumn("userid").number().notNull().defaultValue("0");
        feedback->addColumn("from").string(255).notNull().defaultValue("''");
    }

    {
        StorageCreateTable *feedback_msg = createTable("feedback_msg");
        feedback_msg->addColumn("id").number().autoIncrement().primaryKey().notNull();
        feedback_msg->addColumn("dt").datetime().notNull();
        feedback_msg->addColumn("type").string(255).notNull().defaultValue("''");
        feedback_msg->addColumn("text").string(255).notNull();
        feedback_msg->addColumn("feedbackid").number().notNull().defaultValue("0");
        feedback_msg->addColumn("userid").number().notNull().defaultValue("0");
    }

    {
        StorageCreateTable *games = createTable("games");
        games->addColumn("id").number().autoIncrement().primaryKey().notNull();
        games->addColumn("uuid").string(255).notNull().enableUniqueIndex("idx_game_uuid");
        games->addColumn("title").string(255).notNull();
        games->addColumn("type_game").string(255).notNull();
        games->addColumn("date_create").datetime().notNull().enableIndex();
        games->addColumn("date_start").datetime().notNull().enableIndex();
        games->addColumn("date_stop").datetime().notNull().enableIndex();
        games->addColumn("date_change").datetime().notNull().enableIndex();
        games->addColumn("owner").number().notNull().enableIndex();
        games->addColumn("date_restart").datetime().notNull().enableIndex();
        games->addColumn("description").text().notNull();
        games->addColumn("organizators").string(255).notNull().defaultValue("''");
        games->addColumn("state").string(255).notNull().defaultValue("'copy'");
        games->addColumn("form").string(255).notNull().defaultValue("'online'");
        games->addColumn("rules").text().notNull();
        games->addColumn("maxscore").number().notNull().defaultValue("0");
    }

    {
        StorageCreateTable *public_events = createTable("public_events");
        public_events->addColumn("id").number().autoIncrement().primaryKey().notNull();
        public_events->addColumn("type").string(255).notNull();
        public_events->addColumn("dt").datetime().notNull();
        public_events->addColumn("message").string(2048).notNull().enableIndex();
    }

    {
        StorageCreateTable *quest = createTable("quest");
        quest->addColumn("idquest").number().autoIncrement().primaryKey().notNull();
        quest->addColumn("uuid").string(255).notNull();
        quest->addColumn("name").string(300).notNull();
        quest->addColumn("text").string(4048).notNull();
        quest->addColumn("answer").text().notNull();
        quest->addColumn("score").number().notNull().defaultValue("0");
        quest->addColumn("author").string(50).notNull();
        quest->addColumn("subject").string(128).notNull();
        quest->addColumn("answer_upper_md5").string(128).notNull();
        quest->addColumn("gameid").number().notNull().defaultValue("0");
        quest->addColumn("state").string(50).notNull();
        quest->addColumn("description_state").string(4048).notNull();
        quest->addColumn("date_change").datetime().notNull();
        quest->addColumn("date_create").datetime().notNull();
        quest->addColumn("userid").number().notNull().defaultValue("0");
        quest->addColumn("count_user_solved").number().notNull().defaultValue("0");
        quest->addColumn("copyright").string(255);
        quest->addColumn("answer_format").string(255).defaultValue("''");
    }

    {
        StorageCreateTable *quests_files = createTable("quests_files");
        quests_files->addColumn("id").number().autoIncrement().primaryKey().notNull();
        quests_files->addColumn("uuid").string(255).notNull();
        quests_files->addColumn("questid").number().notNull();
        quests_files->addColumn("filename").string(255).notNull();
        quests_files->addColumn("size").number().notNull();
        quests_files->addColumn("dt").datetime().notNull();
        quests_files->addColumn("filepath").string(255).notNull();
    }

    {
        StorageCreateTable *quests_hints = createTable("quests_hints");
        quests_hints->addColumn("id").number().autoIncrement().primaryKey().notNull();
        quests_hints->addColumn("questid").number().notNull();
        quests_hints->addColumn("text").string(4048).notNull();
        quests_hints->addColumn("dt").datetime().notNull();
    }

    {
        StorageCreateTable *quests_proposal = createTable("quests_proposal");
        quests_proposal->addColumn("id").number().autoIncrement().primaryKey().notNull();
        quests_proposal->addColumn("userid").number().notNull();
        quests_proposal->addColumn("gameid").number().notNull();
        quests_proposal->addColumn("name").string(255).notNull();
        quests_proposal->addColumn("text").string(4048).notNull();
        quests_proposal->addColumn("answer").string(128).notNull();
        quests_proposal->addColumn("score").number().notNull();
        quests_proposal->addColumn("author").string(128).notNull();
        quests_proposal->addColumn("subject").string(128).notNull();
        quests_proposal->addColumn("copyright").text().notNull();
        quests_proposal->addColumn("answer_format").text().notNull();
        quests_proposal->addColumn("created").datetime().notNull();
        quests_proposal->addColumn("updated").datetime().notNull();
        quests_proposal->addColumn("confirmed").string(128).notNull();
    }

    {
        StorageCreateTable *users_quests_answers = createTable("users_quests_answers");
        users_quests_answers->addColumn("id").number().autoIncrement().primaryKey().notNull();
        users_quests_answers->addColumn("userid").number().notNull();
        users_quests_answers->addColumn("questid").number().notNull();
        users_quests_answers->addColumn("user_answer").string(255).notNull();
        users_quests_answers->addColumn("quest_answer").string(255).notNull();
        users_quests_answers->addColumn("passed").string(10).notNull();
        users_quests_answers->addColumn("dt").datetime().notNull();
        users_quests_answers->addColumn("levenshtein").number().notNull().defaultValue("100");
    }

    {
        StorageCreateTable *users = createTable("users");
        users->addColumn("id").number().autoIncrement().primaryKey().notNull();
        users->addColumn("uuid").string(255).notNull();
        users->addColumn("email").string(255).notNull();
        users->addColumn("pass").string(255).notNull();
        users->addColumn("role").string(255).notNull();
        users->addColumn("nick").string(255).notNull();
        users->addColumn("logo").string(255).notNull();
        users->addColumn("dt_create").datetime().notNull();
        users->addColumn("dt_last_login").datetime().notNull();
        users->addColumn("last_ip").string(255).notNull();
        users->addColumn("status").string(255).notNull();
        users->addColumn("country").string(255).defaultValue("''");
        users->addColumn("region").string(255).defaultValue("''");
        users->addColumn("city").string(255).defaultValue("''");
        users->addColumn("latitude").doubleNumber().defaultValue("0.0");
        users->addColumn("longitude").doubleNumber().defaultValue("0.0");
        users->addColumn("rating").number().defaultValue("0");
        users->addColumn("about").text().notNull();
        users->addColumn("university").string(255).defaultValue("''");
    }

    {
        StorageInsert *pAddDefailtAdmin = insertIntoTable("users");
        pAddDefailtAdmin->bindValue("uuid", "39A551F4-3BF0-A1C8-8686-06A5C510DDA3");
        pAddDefailtAdmin->bindValue("email", "admin");
        pAddDefailtAdmin->bindValue("pass", "06976539736714f7eaaa9409a643855029717a9d"); // admin
        pAddDefailtAdmin->bindValue("role", "admin");
        pAddDefailtAdmin->bindValue("nick", "Admin");
        pAddDefailtAdmin->bindValue("logo", "files/users/0.png");
        pAddDefailtAdmin->bindValue("dt_create", "1970-01-01 00:00:00");
        pAddDefailtAdmin->bindValue("dt_last_login", "2015-04-12 23:49:58");
        pAddDefailtAdmin->bindValue("last_ip", "127.0.0.1");
        pAddDefailtAdmin->bindValue("status", "activated");
        pAddDefailtAdmin->bindValue("about", "");
        pAddDefailtAdmin->bindValue("university", "");
    }
    /*
    // TODO redesign
    // here you can migrate data of correction if not just return true;

    std::vector<StorageInsert> vInserts;

    {
        StorageInsert addDefailtAdmin("users");
        addDefailtAdmin.bindValue("uuid", "39A551F4-3BF0-A1C8-8686-06A5C510DDA3");
        addDefailtAdmin.bindValue("email", "admin");
        addDefailtAdmin.bindValue("pass", "06976539736714f7eaaa9409a643855029717a9d"); // admin
        addDefailtAdmin.bindValue("role", "admin");
        addDefailtAdmin.bindValue("nick", "Admin");
        addDefailtAdmin.bindValue("logo", "files/users/0.png");
        addDefailtAdmin.bindValue("dt_create", "1970-01-01 00:00:00");
        addDefailtAdmin.bindValue("dt_last_login", "2015-04-12 23:49:58");
        addDefailtAdmin.bindValue("last_ip", "127.0.0.1");
        addDefailtAdmin.bindValue("status", "activated");
        addDefailtAdmin.bindValue("about", "");
        addDefailtAdmin.bindValue("university", "");
        vInserts.push_back(addDefailtAdmin);
    }
    
    for (int i = 0; i < vInserts.size(); i++) {
        StorageInsert ins = vInserts[i];
        WSJCppLog::info(TAG, "Inserting row to table " + ins.tableName());
        bool bFound = false;
        // is valid
        for (int x = 0; x < m_vStructChanges.size(); x++) {
            StorageStruct st = m_vStructChanges[x];
            if (st.tableName() == ins.tableName()) {
                bFound = true;
                if (!ins.isValid(st)) {
                        error = "Insert not valid";
                        return false;
                }
            }
        }
        if (!bFound) {
            error = "Not foudn table '" + ins.tableName() + "' for insert...";
            return false;
        }
        if (!pStorage->insertRow(pConn, ins)) {
            error = "could not insert to table '" + ins.tableName() + "' ";
            return false;
        }
        if (!ins.isValid(st)) {
                error = "Insert not valid";
                return false;
        }
        WSJCppLog::ok(TAG, "Inserted.");
    }

    WSJCppLog::info(TAG, "exit");
    */


    {
        StorageCreateTable *users_games = createTable("users_games");
        users_games->addColumn("id").number().autoIncrement().primaryKey().notNull();
        users_games->addColumn("userid").number().notNull();
        users_games->addColumn("gameid").number().notNull();
        users_games->addColumn("score").number().notNull();
        users_games->addColumn("date_change").datetime().notNull();
    }

     
    {
        StorageCreateTable *users_profile = createTable("users_profile");
        users_profile->addColumn("id").number().autoIncrement().primaryKey().notNull();
        users_profile->addColumn("userid").number().notNull();
        users_profile->addColumn("name").string(255).notNull();
        users_profile->addColumn("value").string(255).notNull();
        users_profile->addColumn("date_change").datetime().notNull();
    }

     
    {
        StorageCreateTable *users_quests = createTable("users_quests");
        users_quests->addColumn("userid").number().notNull().enableUniqueIndex("idx_user_quest");
        users_quests->addColumn("questid").number().notNull().enableUniqueIndex("idx_user_quest");
        users_quests->addColumn("dt_passed").datetime().notNull();
    }

    {
        StorageCreateTable *users_tokens = createTable("users_tokens");
        users_tokens->addColumn("id").number().autoIncrement().primaryKey().notNull();
        users_tokens->addColumn("userid").number().notNull();
        users_tokens->addColumn("token").string(255).notNull();
        users_tokens->addColumn("status").string(255).notNull();
        users_tokens->addColumn("data").string(4048).notNull();
        users_tokens->addColumn("start_date").datetime().notNull();
        users_tokens->addColumn("end_date").datetime().notNull();
    }

    {
        StorageCreateTable *users_tokens_invalid = createTable("users_tokens_invalid");
        users_tokens_invalid->addColumn("id").number().autoIncrement().primaryKey().notNull();
        users_tokens_invalid->addColumn("userid").number().notNull();
        users_tokens_invalid->addColumn("token").string(255).notNull();
        users_tokens_invalid->addColumn("status").string(255).notNull();
        users_tokens_invalid->addColumn("data").string(4048).notNull();
        users_tokens_invalid->addColumn("start_date").datetime().notNull();
        users_tokens_invalid->addColumn("end_date").datetime().notNull();
    }

    {
        StorageCreateTable *classbook = createTable("classbook");
        classbook->addColumn("id").number().autoIncrement().primaryKey().notNull();
        classbook->addColumn("parentid").number().notNull();
        classbook->addColumn("ordered").number().notNull();
        classbook->addColumn("uuid").string(128).notNull();
        classbook->addColumn("parentuuid").string(128).notNull();
        classbook->addColumn("name").string(128).notNull();
        classbook->addColumn("content").text().notNull();
        classbook->addColumn("md5_content").string(32).notNull();
        classbook->addColumn("created").datetime().notNull();
        classbook->addColumn("updated").datetime().notNull();
    }

    {
        StorageCreateTable *classbook_proposal = createTable("classbook_proposal");
        classbook_proposal->addColumn("id").number().autoIncrement().primaryKey().notNull();
        classbook_proposal->addColumn("classbookid").number().notNull();
        classbook_proposal->addColumn("uuid").string(128).notNull();
        classbook_proposal->addColumn("lang").string(8).notNull();
        classbook_proposal->addColumn("name").string(128).notNull();
        classbook_proposal->addColumn("content").text().notNull();
        classbook_proposal->addColumn("name_before").string(128).notNull();
        classbook_proposal->addColumn("content_before").text().notNull();
        classbook_proposal->addColumn("md5_content").string(32).notNull();
        classbook_proposal->addColumn("created").datetime().notNull();
    }

    {
        StorageCreateTable *classbook_localization = createTable("classbook_localization");
        classbook_localization->addColumn("id").number().autoIncrement().primaryKey().notNull();
        classbook_localization->addColumn("classbookid").number().notNull();
        classbook_localization->addColumn("uuid").string(128).notNull();
        classbook_localization->addColumn("lang").string(8).notNull();
        classbook_localization->addColumn("name").string(128).notNull();
        classbook_localization->addColumn("content").text().notNull();
        classbook_localization->addColumn("md5_content").string(32).notNull();
        classbook_localization->addColumn("created").datetime().notNull();
        classbook_localization->addColumn("updated").datetime().notNull();
    }

    {
        StorageCreateTable *chatmessages = createTable("chatmessages");
        chatmessages->addColumn("id").number().autoIncrement().primaryKey().notNull();
        chatmessages->addColumn("user").string(128).notNull();
        chatmessages->addColumn("message").string(255).notNull();
        chatmessages->addColumn("dt").datetime().notNull();
    }

    {
        StorageCreateTable *users_captcha = createTable("users_captcha");
        users_captcha->addColumn("id").number().autoIncrement().primaryKey().notNull();
        users_captcha->addColumn("captcha_uuid").string(127).notNull();
        users_captcha->addColumn("captcha_val").string(127).notNull();
        users_captcha->addColumn("dt_expired").datetime().notNull();
    }

    {
        StorageCreateTable *quests_writeups = createTable("quests_writeups");
        quests_writeups->addColumn("id").number().autoIncrement().primaryKey().notNull();
        quests_writeups->addColumn("questid").number().notNull();
        quests_writeups->addColumn("type").string(64).notNull();
        quests_writeups->addColumn("link").string(1024).notNull();
        quests_writeups->addColumn("text").string(4048).notNull();
        quests_writeups->addColumn("dt").datetime().notNull();
    }

    {
        StorageCreateTable *users_offers = createTable("users_offers");
        users_offers->addColumn("id").number().autoIncrement().primaryKey().notNull();
        users_offers->addColumn("userid").number().notNull();
        users_offers->addColumn("data").text().notNull();
        users_offers->addColumn("type").string(255).notNull();
        users_offers->addColumn("scomment").string(1024).notNull();
        users_offers->addColumn("status").string(255).notNull();
        users_offers->addColumn("created").datetime().notNull();
        users_offers->addColumn("updated").datetime().notNull();
    }

    {
        StorageCreateTable *settings = createTable("settings");
        settings->addColumn("id").number().autoIncrement().primaryKey().notNull();
        settings->addColumn("name").string(128).notNull();
        settings->addColumn("value").string(1024).notNull();
        settings->addColumn("group").string(255).notNull().defaultValue("''");
        settings->addColumn("type").string(255).notNull().defaultValue("'string'");
    }

    {
        StorageCreateTable *leaks = createTable("leaks");
        leaks->addColumn("id").number().autoIncrement().primaryKey().notNull();
        leaks->addColumn("uuid").string(255).notNull();
        leaks->addColumn("gameid").number().notNull();
        leaks->addColumn("name").string(255).notNull();
        leaks->addColumn("content").string(4096).notNull();
        leaks->addColumn("score").number().notNull();
        leaks->addColumn("created").datetime().notNull();
        leaks->addColumn("updated").datetime().notNull();
        leaks->addColumn("sold").number().notNull();
    }

    {
        StorageCreateTable *users_leaks = createTable("users_leaks");
        users_leaks->addColumn("id").number().autoIncrement().primaryKey().notNull();
        users_leaks->addColumn("leakid").number().notNull();
        users_leaks->addColumn("userid").number().notNull();
        users_leaks->addColumn("grade").number().notNull();
        users_leaks->addColumn("dt").datetime().notNull();
    }

    {

        StorageCreateTable *leaks_files = createTable("leaks_files");
        leaks_files->addColumn("id").number().autoIncrement().primaryKey().notNull();
        leaks_files->addColumn("uuid").string(255).notNull();
        leaks_files->addColumn("leakid").number().notNull();
        leaks_files->addColumn("filename_orig").string(255).notNull();
        leaks_files->addColumn("md5").string(255).notNull();
        leaks_files->addColumn("size").number().notNull();
    }
}