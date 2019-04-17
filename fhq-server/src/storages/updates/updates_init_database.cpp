#include <updates_init_database.h>

REGISTRY_STORAGE_UPDATE(UpdatesInitDatabase)

UpdatesInitDatabase::UpdatesInitDatabase()
    : StorageUpdateBase("", "u0099", "Init database u0099") {
    {
        StorageStruct email_delivery("email_delivery", StorageStructTableMode::CREATE);
        email_delivery.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        email_delivery.addColumn(StorageStructColumn("to_email").string(255).notNull());
        email_delivery.addColumn(StorageStructColumn("subject").string(255).notNull());
        email_delivery.addColumn(StorageStructColumn("message").text().notNull());
        email_delivery.addColumn(StorageStructColumn("priority").string(255).notNull());
        email_delivery.addColumn(StorageStructColumn("status").string(255).notNull());
        email_delivery.addColumn(StorageStructColumn("dt").datetime().notNull());
        m_vStructChanges.push_back(email_delivery);
    }

    {
        StorageStruct feedback("feedback", StorageStructTableMode::CREATE);
        feedback.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        feedback.addColumn(StorageStructColumn("dt").datetime().notNull());
        feedback.addColumn(StorageStructColumn("type").string(255).notNull().defaultValue("''"));
        feedback.addColumn(StorageStructColumn("text").string(255).notNull());
        feedback.addColumn(StorageStructColumn("userid").number().notNull().defaultValue("0"));
        feedback.addColumn(StorageStructColumn("from").string(255).notNull().defaultValue("''"));
        m_vStructChanges.push_back(feedback);
    }

    {
        StorageStruct feedback_msg("feedback_msg", StorageStructTableMode::CREATE);
        feedback_msg.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        feedback_msg.addColumn(StorageStructColumn("dt").datetime().notNull());
        feedback_msg.addColumn(StorageStructColumn("type").string(255).notNull().defaultValue("''"));
        feedback_msg.addColumn(StorageStructColumn("text").string(255).notNull());
        feedback_msg.addColumn(StorageStructColumn("feedbackid").number().notNull().defaultValue("0"));
        feedback_msg.addColumn(StorageStructColumn("userid").number().notNull().defaultValue("0"));
        m_vStructChanges.push_back(feedback_msg);
    }

    {
        StorageStruct games("games", StorageStructTableMode::CREATE);
        games.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        games.addColumn(StorageStructColumn("uuid").string(255).notNull().enableUniqueIndex("idx_game_uuid"));
        games.addColumn(StorageStructColumn("title").string(255).notNull());
        games.addColumn(StorageStructColumn("type_game").string(255).notNull());
        games.addColumn(StorageStructColumn("date_create").datetime().notNull().enableIndex());
        games.addColumn(StorageStructColumn("date_start").datetime().notNull().enableIndex());
        games.addColumn(StorageStructColumn("date_stop").datetime().notNull().enableIndex());
        games.addColumn(StorageStructColumn("date_change").datetime().notNull().enableIndex());
        games.addColumn(StorageStructColumn("owner").number().notNull().enableIndex());
        games.addColumn(StorageStructColumn("date_restart").datetime().notNull().enableIndex());
        games.addColumn(StorageStructColumn("description").text().notNull());
        games.addColumn(StorageStructColumn("organizators").string(255).notNull().defaultValue("''"));
        games.addColumn(StorageStructColumn("state").string(255).notNull().defaultValue("'copy'"));
        games.addColumn(StorageStructColumn("form").string(255).notNull().defaultValue("'online'"));
        games.addColumn(StorageStructColumn("rules").text().notNull());
        games.addColumn(StorageStructColumn("maxscore").number().notNull().defaultValue("0"));
        m_vStructChanges.push_back(games);
    }

    {
        StorageStruct public_events("public_events", StorageStructTableMode::CREATE);
        public_events.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        public_events.addColumn(StorageStructColumn("type").string(255).notNull());
        public_events.addColumn(StorageStructColumn("dt").datetime().notNull());
        public_events.addColumn(StorageStructColumn("message").string(2048).notNull().enableIndex());
        m_vStructChanges.push_back(public_events);
    }

    {
        StorageStruct quest("quest", StorageStructTableMode::CREATE);
        quest.addColumn(StorageStructColumn("idquest").number().autoIncrement().primaryKey().notNull());
        quest.addColumn(StorageStructColumn("uuid").string(255).notNull());
        quest.addColumn(StorageStructColumn("name").string(300).notNull());
        quest.addColumn(StorageStructColumn("text").string(4048).notNull());
        quest.addColumn(StorageStructColumn("answer").text().notNull());
        quest.addColumn(StorageStructColumn("score").number().notNull().defaultValue("0"));
        quest.addColumn(StorageStructColumn("author").string(50).notNull());
        quest.addColumn(StorageStructColumn("subject").string(128).notNull());
        quest.addColumn(StorageStructColumn("answer_upper_md5").string(128).notNull());
        quest.addColumn(StorageStructColumn("gameid").number().notNull().defaultValue("0"));
        quest.addColumn(StorageStructColumn("state").string(50).notNull());
        quest.addColumn(StorageStructColumn("description_state").string(4048).notNull());
        quest.addColumn(StorageStructColumn("date_change").datetime().notNull());
        quest.addColumn(StorageStructColumn("date_create").datetime().notNull());
        quest.addColumn(StorageStructColumn("userid").number().notNull().defaultValue("0"));
        quest.addColumn(StorageStructColumn("count_user_solved").number().notNull().defaultValue("0"));
        quest.addColumn(StorageStructColumn("copyright").string(255));
        quest.addColumn(StorageStructColumn("answer_format").string(255).defaultValue("''"));
        m_vStructChanges.push_back(quest);
    }

    {
        StorageStruct quests_files("quests_files", StorageStructTableMode::CREATE);
        quests_files.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        quests_files.addColumn(StorageStructColumn("uuid").string(255).notNull());
        quests_files.addColumn(StorageStructColumn("questid").number().notNull());
        quests_files.addColumn(StorageStructColumn("filename").string(255).notNull());
        quests_files.addColumn(StorageStructColumn("size").number().notNull());
        quests_files.addColumn(StorageStructColumn("dt").datetime().notNull());
        quests_files.addColumn(StorageStructColumn("filepath").string(255).notNull());
        m_vStructChanges.push_back(quests_files);
    }

    {
        StorageStruct quests_hints("quests_hints", StorageStructTableMode::CREATE);
        quests_hints.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        quests_hints.addColumn(StorageStructColumn("questid").number().notNull());
        quests_hints.addColumn(StorageStructColumn("text").string(4048).notNull());
        quests_hints.addColumn(StorageStructColumn("dt").datetime().notNull());
        m_vStructChanges.push_back(quests_hints);
    }

    {
        StorageStruct quests_proposal("quests_proposal", StorageStructTableMode::CREATE);
        quests_proposal.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        quests_proposal.addColumn(StorageStructColumn("userid").number().notNull());
        quests_proposal.addColumn(StorageStructColumn("gameid").number().notNull());
        quests_proposal.addColumn(StorageStructColumn("name").string(255).notNull());
        quests_proposal.addColumn(StorageStructColumn("text").string(4048).notNull());
        quests_proposal.addColumn(StorageStructColumn("answer").string(128).notNull());
        quests_proposal.addColumn(StorageStructColumn("score").number().notNull());
        quests_proposal.addColumn(StorageStructColumn("author").string(128).notNull());
        quests_proposal.addColumn(StorageStructColumn("subject").string(128).notNull());
        quests_proposal.addColumn(StorageStructColumn("copyright").text().notNull());
        quests_proposal.addColumn(StorageStructColumn("answer_format").text().notNull());
        quests_proposal.addColumn(StorageStructColumn("created").datetime().notNull());
        quests_proposal.addColumn(StorageStructColumn("updated").datetime().notNull());
        quests_proposal.addColumn(StorageStructColumn("confirmed").string(128).notNull());
        m_vStructChanges.push_back(quests_proposal);
    }

    {
        StorageStruct users_quests_answers("users_quests_answers", StorageStructTableMode::CREATE);
        users_quests_answers.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        users_quests_answers.addColumn(StorageStructColumn("userid").number().notNull());
        users_quests_answers.addColumn(StorageStructColumn("questid").number().notNull());
        users_quests_answers.addColumn(StorageStructColumn("user_answer").string(255).notNull());
        users_quests_answers.addColumn(StorageStructColumn("quest_answer").string(255).notNull());
        users_quests_answers.addColumn(StorageStructColumn("passed").string(10).notNull());
        users_quests_answers.addColumn(StorageStructColumn("dt").datetime().notNull());
        users_quests_answers.addColumn(StorageStructColumn("levenshtein").number().notNull().defaultValue("100"));
        m_vStructChanges.push_back(users_quests_answers);
    }

    {
        StorageStruct users("users", StorageStructTableMode::CREATE);
        users.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        users.addColumn(StorageStructColumn("uuid").string(255).notNull());
        users.addColumn(StorageStructColumn("email").string(255).notNull());
        users.addColumn(StorageStructColumn("pass").string(255).notNull());
        users.addColumn(StorageStructColumn("role").string(255).notNull());
        users.addColumn(StorageStructColumn("nick").string(255).notNull());
        users.addColumn(StorageStructColumn("logo").string(255).notNull());
        users.addColumn(StorageStructColumn("dt_create").datetime().notNull());
        users.addColumn(StorageStructColumn("dt_last_login").datetime().notNull());
        users.addColumn(StorageStructColumn("last_ip").string(255).notNull());
        users.addColumn(StorageStructColumn("status").string(255).notNull());
        users.addColumn(StorageStructColumn("country").string(255).defaultValue("''"));
        users.addColumn(StorageStructColumn("region").string(255).defaultValue("''"));
        users.addColumn(StorageStructColumn("city").string(255).defaultValue("''"));
        users.addColumn(StorageStructColumn("latitude").doubleNumber().defaultValue("0.0"));
        users.addColumn(StorageStructColumn("longitude").doubleNumber().defaultValue("0.0"));
        users.addColumn(StorageStructColumn("rating").number().defaultValue("0"));
        users.addColumn(StorageStructColumn("about").text().notNull());
        users.addColumn(StorageStructColumn("university").string(255).defaultValue("''"));
        m_vStructChanges.push_back(users);
    }

    {
        StorageStruct users_games("users_games", StorageStructTableMode::CREATE);
        users_games.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        users_games.addColumn(StorageStructColumn("userid").number().notNull());
        users_games.addColumn(StorageStructColumn("gameid").number().notNull());
        users_games.addColumn(StorageStructColumn("score").number().notNull());
        users_games.addColumn(StorageStructColumn("date_change").datetime().notNull());
        m_vStructChanges.push_back(users_games);
    }

     
    {
        StorageStruct users_profile("users_profile", StorageStructTableMode::CREATE);
        users_profile.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        users_profile.addColumn(StorageStructColumn("userid").number().notNull());
        users_profile.addColumn(StorageStructColumn("name").string(255).notNull());
        users_profile.addColumn(StorageStructColumn("value").string(255).notNull());
        users_profile.addColumn(StorageStructColumn("date_change").datetime().notNull());
        m_vStructChanges.push_back(users_profile);
    }

     
    {
        StorageStruct users_quests("users_quests", StorageStructTableMode::CREATE);
        users_quests.addColumn(StorageStructColumn("userid").number().notNull().enableUniqueIndex("idx_user_quest"));
        users_quests.addColumn(StorageStructColumn("questid").number().notNull().enableUniqueIndex("idx_user_quest"));
        users_quests.addColumn(StorageStructColumn("dt_passed").datetime().notNull());
        m_vStructChanges.push_back(users_quests);
    }

    {
        StorageStruct users_tokens("users_tokens", StorageStructTableMode::CREATE);
        users_tokens.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        users_tokens.addColumn(StorageStructColumn("userid").number().notNull());
        users_tokens.addColumn(StorageStructColumn("token").string(255).notNull());
        users_tokens.addColumn(StorageStructColumn("status").string(255).notNull());
        users_tokens.addColumn(StorageStructColumn("data").string(4048).notNull());
        users_tokens.addColumn(StorageStructColumn("start_date").datetime().notNull());
        users_tokens.addColumn(StorageStructColumn("end_date").datetime().notNull());
        m_vStructChanges.push_back(users_tokens);
    }

    {
        StorageStruct users_tokens_invalid("users_tokens_invalid", StorageStructTableMode::CREATE);
        users_tokens_invalid.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        users_tokens_invalid.addColumn(StorageStructColumn("userid").number().notNull());
        users_tokens_invalid.addColumn(StorageStructColumn("token").string(255).notNull());
        users_tokens_invalid.addColumn(StorageStructColumn("status").string(255).notNull());
        users_tokens_invalid.addColumn(StorageStructColumn("data").string(4048).notNull());
        users_tokens_invalid.addColumn(StorageStructColumn("start_date").datetime().notNull());
        users_tokens_invalid.addColumn(StorageStructColumn("end_date").datetime().notNull());
        m_vStructChanges.push_back(users_tokens_invalid);
    }

    {
        StorageStruct classbook("classbook", StorageStructTableMode::CREATE);
        classbook.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        classbook.addColumn(StorageStructColumn("parentid").number().notNull());
        classbook.addColumn(StorageStructColumn("classbookid").number().notNull());
        classbook.addColumn(StorageStructColumn("uuid").string(128).notNull());
        classbook.addColumn(StorageStructColumn("parentuuid").string(128).notNull());
        classbook.addColumn(StorageStructColumn("name").string(128).notNull());
        classbook.addColumn(StorageStructColumn("content").text().notNull());
        classbook.addColumn(StorageStructColumn("md5_content").string(32).notNull());
        classbook.addColumn(StorageStructColumn("created").datetime().notNull());
        classbook.addColumn(StorageStructColumn("updated").datetime().notNull());
        m_vStructChanges.push_back(classbook);
    }

    {
        StorageStruct classbook_proposal("classbook_proposal", StorageStructTableMode::CREATE);
        classbook_proposal.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        classbook_proposal.addColumn(StorageStructColumn("classbookid").number().notNull());
        classbook_proposal.addColumn(StorageStructColumn("uuid").string(128).notNull());
        classbook_proposal.addColumn(StorageStructColumn("lang").string(8).notNull());
        classbook_proposal.addColumn(StorageStructColumn("name").string(128).notNull());
        classbook_proposal.addColumn(StorageStructColumn("content").text().notNull());
        classbook_proposal.addColumn(StorageStructColumn("name_before").string(128).notNull());
        classbook_proposal.addColumn(StorageStructColumn("content_before").text().notNull());
        classbook_proposal.addColumn(StorageStructColumn("md5_content").string(32).notNull());
        classbook_proposal.addColumn(StorageStructColumn("created").datetime().notNull());
        m_vStructChanges.push_back(classbook_proposal);
    }

    {
        StorageStruct classbook_localization("classbook_localization", StorageStructTableMode::CREATE);
        classbook_localization.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        classbook_localization.addColumn(StorageStructColumn("classbookid").number().notNull());
        classbook_localization.addColumn(StorageStructColumn("uuid").string(128).notNull());
        classbook_localization.addColumn(StorageStructColumn("lang").string(8).notNull());
        classbook_localization.addColumn(StorageStructColumn("name").string(128).notNull());
        classbook_localization.addColumn(StorageStructColumn("content").text().notNull());
        classbook_localization.addColumn(StorageStructColumn("md5_content").string(32).notNull());
        classbook_localization.addColumn(StorageStructColumn("created").datetime().notNull());
        classbook_localization.addColumn(StorageStructColumn("updated").datetime().notNull());
        m_vStructChanges.push_back(classbook_localization);
    }

    {
        StorageStruct chatmessages("chatmessages", StorageStructTableMode::CREATE);
        chatmessages.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        chatmessages.addColumn(StorageStructColumn("user").string(128).notNull());
        chatmessages.addColumn(StorageStructColumn("message").string(255).notNull());
        chatmessages.addColumn(StorageStructColumn("dt").datetime().notNull());
        m_vStructChanges.push_back(chatmessages);
    }

    {
        StorageStruct users_captcha("users_captcha", StorageStructTableMode::CREATE);
        users_captcha.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        users_captcha.addColumn(StorageStructColumn("captcha_uuid").string(127).notNull());
        users_captcha.addColumn(StorageStructColumn("captcha_val").string(127).notNull());
        users_captcha.addColumn(StorageStructColumn("dt_expired").datetime().notNull());
        m_vStructChanges.push_back(users_captcha);
    }

    {
        StorageStruct quests_writeups("quests_writeups", StorageStructTableMode::CREATE);
        quests_writeups.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        quests_writeups.addColumn(StorageStructColumn("questid").number().notNull());
        quests_writeups.addColumn(StorageStructColumn("type").string(64).notNull());
        quests_writeups.addColumn(StorageStructColumn("link").string(1024).notNull());
        quests_writeups.addColumn(StorageStructColumn("text").string(4048).notNull());
        quests_writeups.addColumn(StorageStructColumn("dt").datetime().notNull());
        m_vStructChanges.push_back(quests_writeups);
    }

    {
        StorageStruct users_offers("users_offers", StorageStructTableMode::CREATE);
        users_offers.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        users_offers.addColumn(StorageStructColumn("userid").number().notNull());
        users_offers.addColumn(StorageStructColumn("data").text().notNull());
        users_offers.addColumn(StorageStructColumn("type").string(255).notNull());
        users_offers.addColumn(StorageStructColumn("scomment").string(1024).notNull());
        users_offers.addColumn(StorageStructColumn("status").string(255).notNull());
        users_offers.addColumn(StorageStructColumn("created").datetime().notNull());
        users_offers.addColumn(StorageStructColumn("updated").datetime().notNull());
        m_vStructChanges.push_back(users_offers);
    }

    {
        StorageStruct settings("settings", StorageStructTableMode::CREATE);
        settings.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        settings.addColumn(StorageStructColumn("name").string(128).notNull());
        settings.addColumn(StorageStructColumn("value").string(1024).notNull());
        settings.addColumn(StorageStructColumn("group").string(255).notNull().defaultValue("''"));
        settings.addColumn(StorageStructColumn("type").string(255).notNull().defaultValue("'string'"));
        m_vStructChanges.push_back(settings);
    }

    {
        StorageStruct leaks("leaks", StorageStructTableMode::CREATE);
        leaks.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        leaks.addColumn(StorageStructColumn("uuid").string(255).notNull());
        leaks.addColumn(StorageStructColumn("gameid").number().notNull());
        leaks.addColumn(StorageStructColumn("name").string(255).notNull());
        leaks.addColumn(StorageStructColumn("content").string(4096).notNull());
        leaks.addColumn(StorageStructColumn("score").number().notNull());
        leaks.addColumn(StorageStructColumn("created").datetime().notNull());
        leaks.addColumn(StorageStructColumn("updated").datetime().notNull());
        leaks.addColumn(StorageStructColumn("sold").number().notNull());
        m_vStructChanges.push_back(leaks);
    }

    {
        StorageStruct users_leaks("users_leaks", StorageStructTableMode::CREATE);
        users_leaks.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        users_leaks.addColumn(StorageStructColumn("leakid").number().notNull());
        users_leaks.addColumn(StorageStructColumn("userid").number().notNull());
        users_leaks.addColumn(StorageStructColumn("grade").number().notNull());
        users_leaks.addColumn(StorageStructColumn("dt").datetime().notNull());
        m_vStructChanges.push_back(users_leaks);
    }

    {
        StorageStruct leaks_files("leaks_files", StorageStructTableMode::CREATE);
        leaks_files.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
        leaks_files.addColumn(StorageStructColumn("uuid").string(255).notNull());
        leaks_files.addColumn(StorageStructColumn("leakid").number().notNull());
        leaks_files.addColumn(StorageStructColumn("filename_orig").string(255).notNull());
        leaks_files.addColumn(StorageStructColumn("md5").string(255).notNull());
        leaks_files.addColumn(StorageStructColumn("size").number().notNull());
        m_vStructChanges.push_back(leaks_files);
    }
}

bool UpdatesInitDatabase::custom(Storage *pStorage, StorageConnection *pConn, std::string &error) {
    
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
        Log::info(TAG, "Inserting row to table " + ins.tableName());
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
        Log::ok(TAG, "Inserted.");
    }

    Log::info(TAG, "exit");
    return true;
}
