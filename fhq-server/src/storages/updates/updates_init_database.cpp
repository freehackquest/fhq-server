#include <updates_init_database.h>

UpdatesInitDatabase::UpdatesInitDatabase()
    : StorageUpdateBase("", "u0063", "Init database 0063") {

}

bool UpdatesInitDatabase::apply(Storage *pStorage, std::string &error){

	StorageConnection *pConn = pStorage->connect();
	if (pConn == nullptr) {
		Log::info(TAG, "Wrong connection");
		return false;
	}

	StorageStruct email_delivery("email_delivery", StorageStructTableMode::CREATE);
	email_delivery.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
	email_delivery.addColumn(StorageStructColumn("to_email").string(255).notNull());
	email_delivery.addColumn(StorageStructColumn("subject").string(255).notNull());
	email_delivery.addColumn(StorageStructColumn("message").text().notNull());
	email_delivery.addColumn(StorageStructColumn("priority").string(255).notNull());
	email_delivery.addColumn(StorageStructColumn("status").string(255).notNull());
	email_delivery.addColumn(StorageStructColumn("dt").datetime().notNull());
	if (!pStorage->applyStruct(pConn, email_delivery)) {
		error = "Could not create table 'email_delivery'";
		return false;
	}

	StorageStruct feedback("feedback", StorageStructTableMode::CREATE);
	feedback.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
	feedback.addColumn(StorageStructColumn("dt").datetime().notNull());
	feedback.addColumn(StorageStructColumn("type").string(255).notNull().defaultValue("''"));
	feedback.addColumn(StorageStructColumn("text").string(255).notNull());
	feedback.addColumn(StorageStructColumn("userid").number().notNull().defaultValue("0"));
	if (!pStorage->applyStruct(pConn, feedback)) {
		error = "could not create table 'feedback'";
		return false;
	}

	StorageStruct feedback_msg("feedback_msg", StorageStructTableMode::CREATE);
	feedback_msg.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
	feedback_msg.addColumn(StorageStructColumn("dt").datetime().notNull());
	feedback_msg.addColumn(StorageStructColumn("type").string(255).notNull().defaultValue("''"));
	feedback_msg.addColumn(StorageStructColumn("text").string(255).notNull());
	feedback_msg.addColumn(StorageStructColumn("feedbackid").number().notNull().defaultValue("0"));
	feedback_msg.addColumn(StorageStructColumn("userid").number().notNull().defaultValue("0"));
	if (!pStorage->applyStruct(pConn, feedback_msg)) {
		error = "could not create table 'feedback_msg'";
		return false;
	}


	StorageStruct games("games", StorageStructTableMode::CREATE);
	games.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
	games.addColumn(StorageStructColumn("uuid").string(255).notNull().enableUniqueIndex("idx_game_uuid"));
	games.addColumn(StorageStructColumn("title").string(255).notNull());
	games.addColumn(StorageStructColumn("logo").string(255).notNull());
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

	if (!pStorage->applyStruct(pConn, games)) {
		error = "could not create table 'games'";
		return false;
	}

	StorageStruct public_events("public_events", StorageStructTableMode::CREATE);
	public_events.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
	public_events.addColumn(StorageStructColumn("type").string(255).notNull());
	public_events.addColumn(StorageStructColumn("dt").datetime().notNull());
	public_events.addColumn(StorageStructColumn("message").string(2048).notNull().enableIndex());
	if (!pStorage->applyStruct(pConn, public_events)) {
		error = "could not create table 'public_events'";
		return false;
	}

	StorageStruct quest("quest", StorageStructTableMode::CREATE);
	quest.addColumn(StorageStructColumn("idquest").number().autoIncrement().primaryKey().notNull());
	quest.addColumn(StorageStructColumn("name").string(300).notNull());
	quest.addColumn(StorageStructColumn("text").string(4048).notNull());
	quest.addColumn(StorageStructColumn("answer").text().notNull());
	quest.addColumn(StorageStructColumn("score").number().notNull().defaultValue("0"));
	quest.addColumn(StorageStructColumn("min_score").number().notNull().defaultValue("0"));
	quest.addColumn(StorageStructColumn("for_person").number().notNull().defaultValue("0"));
	quest.addColumn(StorageStructColumn("idauthor").number().notNull().defaultValue("0"));
	quest.addColumn(StorageStructColumn("author").string(50).notNull());
	quest.addColumn(StorageStructColumn("subject").string(128).notNull());
	quest.addColumn(StorageStructColumn("answer_upper_md5").string(128).notNull());
	quest.addColumn(StorageStructColumn("gameid").number().notNull().defaultValue("0"));
	quest.addColumn(StorageStructColumn("state").string(50).notNull());
	quest.addColumn(StorageStructColumn("description_state").string(4048).notNull());
	quest.addColumn(StorageStructColumn("quest_uuid").string(255).notNull());
	quest.addColumn(StorageStructColumn("date_change").datetime().notNull());
	quest.addColumn(StorageStructColumn("date_create").datetime().notNull());
	quest.addColumn(StorageStructColumn("userid").number().notNull().defaultValue("0"));
	quest.addColumn(StorageStructColumn("count_user_solved").number().notNull().defaultValue("0"));
	if (!pStorage->applyStruct(pConn, quest)) {
		error = "could not create table 'quest'";
		return false;
	}

	StorageStruct quests_files("quests_files", StorageStructTableMode::CREATE);
	quests_files.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
	quests_files.addColumn(StorageStructColumn("uuid").string(255).notNull());
	quests_files.addColumn(StorageStructColumn("questid").number().notNull());
	quests_files.addColumn(StorageStructColumn("filename").string(255).notNull());
	quests_files.addColumn(StorageStructColumn("size").number().notNull());
	quests_files.addColumn(StorageStructColumn("dt").datetime().notNull());
	quests_files.addColumn(StorageStructColumn("filepath").string(255).notNull());
	if (!pStorage->applyStruct(pConn, quests_files)) {
		error = "could not create table 'quests_files'";
		return false;
	}

	StorageStruct tryanswer("tryanswer", StorageStructTableMode::CREATE);
	tryanswer.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
	tryanswer.addColumn(StorageStructColumn("iduser").number().notNull());
	tryanswer.addColumn(StorageStructColumn("idquest").number().notNull());
	tryanswer.addColumn(StorageStructColumn("answer_try").text().notNull());
	tryanswer.addColumn(StorageStructColumn("answer_real").text().notNull());
	tryanswer.addColumn(StorageStructColumn("passed").string(10).notNull());
	tryanswer.addColumn(StorageStructColumn("datetime_try").datetime().notNull());
	tryanswer.addColumn(StorageStructColumn("levenshtein").number().notNull().defaultValue("100"));
	if (!pStorage->applyStruct(pConn, tryanswer)) {
		error = "could not create table 'tryanswer'";
		return false;
	}

	StorageStruct tryanswer_backup("tryanswer_backup", StorageStructTableMode::CREATE);
	tryanswer_backup.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
	tryanswer_backup.addColumn(StorageStructColumn("iduser").number().notNull());
	tryanswer_backup.addColumn(StorageStructColumn("idquest").number().notNull());
	tryanswer_backup.addColumn(StorageStructColumn("answer_try").text().notNull());
	tryanswer_backup.addColumn(StorageStructColumn("answer_real").text().notNull());
	tryanswer_backup.addColumn(StorageStructColumn("passed").string(10).notNull());
	tryanswer_backup.addColumn(StorageStructColumn("datetime_try").datetime().notNull());
	tryanswer_backup.addColumn(StorageStructColumn("levenshtein").number().notNull().defaultValue("100"));
	if (!pStorage->applyStruct(pConn, tryanswer_backup)) {
		error = "could not create table 'tryanswer_backup'";
		return false;
	}

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
	if (!pStorage->applyStruct(pConn, users)) {
		error = "could not create table 'users'";
		return false;
	}

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

	if (!addDefailtAdmin.isValid(users)) {
        error = "Insert not valid";
		return false;
    }

	if (!pStorage->insertRow(pConn, addDefailtAdmin)) {
		error = "could not insert default user admin";
		return false;
	}

	// TODO redesign
	/*if (!pConn->executeQuery("INSERT INTO users(uuid, email, pass, role, nick, logo, dt_create, dt_last_login, last_ip, status) "
			" VALUES ("
            " '39A551F4-3BF0-A1C8-8686-06A5C510DDA3',"
			" 'admin', '06976539736714f7eaaa9409a643855029717a9d', 'admin', 'Admin', 'files/users/0.png', '1970-01-01 00:00:00', '2015-04-12 23:49:58', '127.0.0.1', 'activated')")) {
		error = "could not insert default 'admin' to 'users'";
		return false;
	}*/

	StorageStruct users_games("users_games", StorageStructTableMode::CREATE);
	users_games.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
	users_games.addColumn(StorageStructColumn("userid").number().notNull());
	users_games.addColumn(StorageStructColumn("gameid").number().notNull());
	users_games.addColumn(StorageStructColumn("score").number().notNull());
	users_games.addColumn(StorageStructColumn("date_change").datetime().notNull());
	if (!pStorage->applyStruct(pConn, users_games)) {
		error = "could not create table 'users_games'";
		return false;
	}

	StorageStruct users_ips("users_ips", StorageStructTableMode::CREATE);
	users_ips.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
	users_ips.addColumn(StorageStructColumn("userid").number().notNull());
	users_ips.addColumn(StorageStructColumn("ip").string(255).notNull().enableIndex());
	users_ips.addColumn(StorageStructColumn("country").string(255).notNull().enableIndex());
	users_ips.addColumn(StorageStructColumn("city").string(255).notNull().enableIndex());
	users_ips.addColumn(StorageStructColumn("client").string(255).notNull().enableIndex());
	users_ips.addColumn(StorageStructColumn("date_sign_in").datetime().notNull());
	users_ips.addColumn(StorageStructColumn("browser").string(1024).notNull());
	users_ips.addColumn(StorageStructColumn("gameid").number().notNull());
	users_ips.addColumn(StorageStructColumn("score").number().notNull());
	
	if (!pStorage->applyStruct(pConn, users_ips)) {
		error = "could not create table 'users_ips'";
		return false;
	}

	StorageStruct users_profile("users_profile", StorageStructTableMode::CREATE);
	users_profile.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
	users_profile.addColumn(StorageStructColumn("userid").number().notNull());
	users_profile.addColumn(StorageStructColumn("name").string(255).notNull());
	users_profile.addColumn(StorageStructColumn("value").string(255).notNull());
	users_profile.addColumn(StorageStructColumn("date_change").datetime().notNull());
	if (!pStorage->applyStruct(pConn, users_profile)) {
		error = "could not create table 'users_ips'";
		return false;
	}

	StorageStruct users_quests("users_quests", StorageStructTableMode::CREATE);
	users_quests.addColumn(StorageStructColumn("userid").number().notNull().enableUniqueIndex("idx_user_quest"));
	users_quests.addColumn(StorageStructColumn("questid").number().notNull().enableUniqueIndex("idx_user_quest"));
	users_quests.addColumn(StorageStructColumn("dt_passed").datetime().notNull());
	if (!pStorage->applyStruct(pConn, users_quests)) {
		error = "could not create table 'users_quests'";
		return false;
	}

	StorageStruct users_tokens("users_tokens", StorageStructTableMode::CREATE);
	users_tokens.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
	users_tokens.addColumn(StorageStructColumn("userid").number().notNull());
	users_tokens.addColumn(StorageStructColumn("token").string(255).notNull());
	users_tokens.addColumn(StorageStructColumn("status").string(255).notNull());
	users_tokens.addColumn(StorageStructColumn("data").string(4048).notNull());
	users_tokens.addColumn(StorageStructColumn("start_date").datetime().notNull());
	users_tokens.addColumn(StorageStructColumn("end_date").datetime().notNull());
	if (!pStorage->applyStruct(pConn, users_tokens)) {
		error = "could not create table 'users_tokens'";
		return false;
	}

	StorageStruct users_tokens_invalid("users_tokens_invalid", StorageStructTableMode::CREATE);
	users_tokens_invalid.addColumn(StorageStructColumn("id").number().autoIncrement().primaryKey().notNull());
	users_tokens_invalid.addColumn(StorageStructColumn("userid").number().notNull());
	users_tokens_invalid.addColumn(StorageStructColumn("token").string(255).notNull());
	users_tokens_invalid.addColumn(StorageStructColumn("status").string(255).notNull());
	users_tokens_invalid.addColumn(StorageStructColumn("data").string(4048).notNull());
	users_tokens_invalid.addColumn(StorageStructColumn("start_date").datetime().notNull());
	users_tokens_invalid.addColumn(StorageStructColumn("end_date").datetime().notNull());
	if (!pStorage->applyStruct(pConn, users_tokens_invalid)) {
		error = "could not create table 'users_tokens'";
		return false;
	}

	delete pConn;
	Log::info(TAG, "exit");
	return true;
}
