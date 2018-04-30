#include <database0060.h>

Database0060::Database0060()
    : UpdateBase("", "u0060", "Init database 0060") {

}

bool Database0060::update(QSqlDatabase &db, std::string &error){
	
	{
		// Table structure for table `email_delivery`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `email_delivery` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `to_email` varchar(255) NOT NULL,"
			"  `subject` varchar(255) NOT NULL,"
			"  `message` text NOT NULL,"
			"  `priority` varchar(255) NOT NULL,"
			"  `status` varchar(255) NOT NULL,"
			"  `dt` datetime NOT NULL,"
			"  PRIMARY KEY (`id`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
	
	{
		// Table structure for table `feedback`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `feedback` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `dt` datetime DEFAULT NULL,"
			"  `type` varchar(255) DEFAULT '',"
			"  `text` text,"
			"  `userid` int(11) DEFAULT '0',"
			"  PRIMARY KEY (`id`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
 

	{
		// Table structure for table `feedback_msg`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `feedback_msg` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `dt` datetime NOT NULL,"
			"  `text` text,"
			"  `feedbackid` int(11) DEFAULT '0',"
			"  `userid` int(11) DEFAULT '0',"
			"  PRIMARY KEY (`id`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
 

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
	
	{
		// Table structure for table `quest`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `quest` ("
			"  `idquest` int(10) unsigned NOT NULL AUTO_INCREMENT,"
			"  `name` varchar(300) NOT NULL,"
			"  `text` varchar(4048) NOT NULL,"
			"  `answer` text NOT NULL,"
			"  `score` int(10) unsigned NOT NULL,"
			"  `min_score` int(10) NOT NULL DEFAULT '0',"
			"  `for_person` bigint(10) unsigned NOT NULL DEFAULT '0',"
			"  `idauthor` bigint(20) DEFAULT '0',"
			"  `author` varchar(50) DEFAULT NULL,"
			"  `subject` varchar(128) DEFAULT NULL,"
			"  `answer_upper_md5` varchar(255) DEFAULT NULL,"
			"  `gameid` int(10) unsigned DEFAULT NULL,"
			"  `state` varchar(50) DEFAULT NULL,"
			"  `description_state` varchar(4048) DEFAULT NULL,"
			"  `quest_uuid` varchar(255) DEFAULT NULL,"
			"  `date_change` datetime DEFAULT NULL,"
			"  `date_create` datetime DEFAULT NULL,"
			"  `userid` bigint(20) DEFAULT '0',"
			"  `count_user_solved` bigint(20) DEFAULT '0',"
			"  PRIMARY KEY (`idquest`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
 

	{
		// Table structure for table `quests_files`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `quests_files` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `uuid` varchar(255) NOT NULL,"
			"  `questid` int(11) NOT NULL,"
			"  `filename` varchar(255) NOT NULL,"
			"  `size` int(11) NOT NULL,"
			"  `dt` datetime NOT NULL,"
			"  `filepath` varchar(255) DEFAULT '',"
			"  PRIMARY KEY (`id`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
 

	{
		// Table structure for table `tryanswer`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `tryanswer` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `iduser` int(11) NOT NULL,"
			"  `idquest` int(11) NOT NULL,"
			"  `answer_try` text NOT NULL,"
			"  `answer_real` text NOT NULL,"
			"  `passed` varchar(10) NOT NULL,"
			"  `datetime_try` datetime NOT NULL,"
			"  `levenshtein` int(11) DEFAULT '100',"
			"  PRIMARY KEY (`id`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
 

	{
		// Table structure for table `tryanswer_backup`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `tryanswer_backup` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `iduser` int(11) NOT NULL,"
			"  `idquest` int(11) NOT NULL,"
			"  `answer_try` text NOT NULL,"
			"  `answer_real` text NOT NULL,"
			"  `passed` varchar(10) NOT NULL,"
			"  `datetime_try` datetime NOT NULL,"
			"  `levenshtein` int(11) DEFAULT '100',"
			"  PRIMARY KEY (`id`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
 

	{
		// Table structure for table `updates`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `updates` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `from_version` varchar(255) DEFAULT NULL,"
			"  `version` varchar(255) DEFAULT NULL,"
			"  `name` varchar(255) DEFAULT NULL,"
			"  `description` text,"
			"  `result` varchar(255) DEFAULT NULL,"
			"  `userid` int(11) DEFAULT NULL,"
			"  `datetime_update` datetime DEFAULT NULL,"
			"  PRIMARY KEY (`id`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}


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

	{
		// Table structure for table `users`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `users` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `uuid` varchar(255) NOT NULL,"
			"  `email` varchar(255) NOT NULL,"
			"  `pass` varchar(255) NOT NULL,"
			"  `role` varchar(255) NOT NULL,"
			"  `nick` varchar(255) NOT NULL,"
			"  `logo` varchar(255) NOT NULL,"
			"  `dt_create` datetime NOT NULL,"
			"  `dt_last_login` datetime NOT NULL,"
			"  `last_ip` varchar(255) NOT NULL,"
			"  `status` varchar(255) NOT NULL,"
			"  PRIMARY KEY (`id`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
 
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
 
	{
		// Table structure for table `users_games`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `users_games` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `userid` int(11) DEFAULT NULL,"
			"  `gameid` int(11) DEFAULT NULL,"
			"  `score` int(11) DEFAULT NULL,"
			"  `date_change` datetime DEFAULT NULL,"
			"  PRIMARY KEY (`id`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
 

	{
		// Table structure for table `users_ips`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `users_ips` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `userid` int(11) DEFAULT NULL,"
			"  `ip` varchar(255) DEFAULT NULL,"
			"  `country` varchar(255) DEFAULT NULL,"
			"  `city` varchar(255) DEFAULT NULL,"
			"  `client` varchar(255) DEFAULT NULL,"
			"  `date_sign_in` datetime DEFAULT NULL,"
			"  `browser` varchar(1024) DEFAULT NULL,"
			"  PRIMARY KEY (`id`),"
			"  KEY `ip` (`ip`),"
			"  KEY `country` (`country`),"
			"  KEY `city` (`city`),"
			"  KEY `client` (`client`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
 

	{
		// Table structure for table `users_profile`
		QSqlQuery query(db);
		query.prepare(""
			"CREATE TABLE IF NOT EXISTS `users_profile` ("
			"  `id` int(11) NOT NULL AUTO_INCREMENT,"
			"  `userid` int(11) DEFAULT NULL,"
			"  `name` varchar(255) DEFAULT NULL,"
			"  `value` varchar(255) DEFAULT NULL,"
			"  `date_change` datetime DEFAULT NULL,"
			"  PRIMARY KEY (`id`)"
			") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}

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
		if(!query.exec()){
			error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
			return false;
		}
	}
 

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
}
