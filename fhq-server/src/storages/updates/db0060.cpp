#include <db0060.h>

StorageStruct0060::StorageStruct0060()
    : StorageUpdateBase("", "u0060", "Init database 0060") {

}

bool StorageStruct0060::apply(Storage *pStorage, std::string &error){

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

	/*


	{
		// Table structure for table `games`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `games` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `uuid` varchar(255) DEFAULT NULL,"
			"  `title` varchar(255) DEFAULT NULL,"
			"  `logo` varchar(255) DEFAULT NULL,"
			"  `type_game` varchar(255) DEFAULT NULL,"
			"  `date_create` datetime DEFAULT NULL,"
			"  `date_start` datetime DEFAULT NULL,"
			"  `date_stop` datetime DEFAULT NULL,"
			"  `date_change` datetime DEFAULT NULL,"
			"  `owner` int(11) DEFAULT NULL,"
			"  `date_restart` datetime DEFAULT NULL,"
			"  `description` text,"
			"  `organizators` varchar(255) DEFAULT '',"
			"  `state` varchar(255) DEFAULT 'copy',"
			"  `form` varchar(255) DEFAULT 'online',"
			"  `rules` text,"
			"  `maxscore` int(11) DEFAULT '0',"
			"  PRIMARY KEY (`id`),"
			"  UNIQUE KEY `uuid_game` (`uuid`),"
			"  UNIQUE KEY `uuid_game_2` (`uuid`),"
			"  KEY `date_create` (`date_create`),"
			"  KEY `date_start` (`date_start`),"
			"  KEY `date_stop` (`date_stop`),"
			"  KEY `date_change` (`date_change`),"
			"  KEY `owner` (`owner`),"
			"  KEY `date_restart` (`date_restart`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
 

	{
		// Table structure for table `public_events`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `public_events` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `type` varchar(255) NOT NULL,"
			"  `dt` datetime NOT NULL,"
			"  `message` varchar(2048) DEFAULT '',"
			"  PRIMARY KEY (`id`),"
			"  KEY `message` (`message`(255))"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
	*/
	
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
	tryanswer.addColumn(StorageStructColumn("levenshtein").number().notNull());
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

/*
	{
		// Table structure for table `userquest`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `userquest` ("
			"  `iduser` int(10) NOT NULL,"
			"  `idquest` int(10) NOT NULL,"
			"  `stopdate` datetime NOT NULL,"
			"  `startdate` datetime NOT NULL,"
			"  UNIQUE KEY `iduser` (`iduser`,`idquest`)"
			") ENGINE=InnoDB DEFAULT CHARSET=utf8;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
*/

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

/*
	{
		QSqlQuery query(db);
		query.prepare(
			"INSERT INTO `users` (`uuid`, `email`, `pass`, `role`, `nick`, `logo`, `dt_create`, `dt_last_login`, `last_ip`, `status`) VALUES"
            "('39A551F4-3BF0-A1C8-8686-06A5C510DDA3', 'admin', '06976539736714f7eaaa9409a643855029717a9d', 'admin', 'Admin', 'files/users/0.png', '1970-01-01 00:00:00', '2015-04-12 23:49:58', '127.0.0.1', 'activated')"
		);
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with data insertion " + error);
			return false;
		}
	}
 */

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

	/*StorageStruct users_quests("users_quests", StorageStructTableMode::CREATE);
	users_quests.addColumn(StorageStructColumn("userid").number().notNull());
	users_quests.addColumn(StorageStructColumn("questid").number().notNull());
	users_quests.addColumn(StorageStructColumn("name").string(255).notNull());
	users_quests.addColumn(StorageStructColumn("value").string(255).notNull());
	users_quests.addColumn(StorageStructColumn("date_change").datetime().notNull());
	if (!pStorage->applyStruct(pConn, users_quests)) {
		error = "could not create table 'users_quests'";
		return false;
	}*/

/*
	{
		// Table structure for table `users_quests`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `users_quests` ("
			"  `userid` int(11) DEFAULT NULL,"
			"  `questid` int(11) DEFAULT NULL,"
			"  `dt_passed` datetime NOT NULL,"
			"  UNIQUE KEY `userid` (`userid`,`questid`)"
			") ENGINE=InnoDB DEFAULT CHARSET=utf8;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
 */

/*

	{
		// Table structure for table `users_tokens`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `users_tokens` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `userid` int(11) NOT NULL,"
			"  `token` varchar(255) NOT NULL,"
			"  `status` varchar(255) NOT NULL,"
			"  `data` varchar(4048) NOT NULL,"
			"  `start_date` datetime NOT NULL,"
			"  `end_date` datetime NOT NULL,"
			"  PRIMARY KEY (`id`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if (!query.exec()) {
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
 */

/*

	{
		// Table structure for table `users_tokens_invalid`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `users_tokens_invalid` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `userid` int(11) NOT NULL,"
			"  `token` varchar(255) NOT NULL,"
			"  `status` varchar(255) NOT NULL,"
			"  `data` varchar(4048) NOT NULL,"
			"  `start_date` datetime NOT NULL,"
			"  `end_date` datetime NOT NULL,"
			"  PRIMARY KEY (`id`)"
			") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
	return true;
	*/
	delete pConn;
	Log::info(TAG, "exit");
	return false;
}
