#include "../headers/database0060.h"
#include <QSqlQuery>

QString Database0060::from_version(){
	return "";
}

QString Database0060::version(){
	return "u0060";
}

QString Database0060::description(){
	return "Init database 0060";
}

void Database0060::update(QSqlDatabase &db){
	
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
		query.exec();
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
		query.exec();
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
		query.exec();
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
		query.exec();
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
		query.exec();
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
		query.exec();
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
		query.exec();
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
		query.exec();
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
		query.exec();
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
		query.exec();
	}


	{
		//  Dumping data for table `updates`
		QSqlQuery query(db);
		query.prepare(
			"INSERT INTO `updates` (`from_version`, `version`, `name`, `description`, `result`, `userid`, `datetime_update`) VALUES"
			"(1, 'u0000', 'u0001', 'test', 'test', 'updated', 220, '2015-01-16 01:55:57'),"
			"(2, 'u0001', 'u0002', 'emails', 'unpack mails from user.username to user.email', 'updated', 220, '2015-01-16 01:56:01'),"
			"(3, 'u0002', 'u0003', 'update table quest', 'state and description_state to "" if null', 'updated', 220, '2015-01-16 01:56:01'),"
			"(4, 'u0003', 'u0004', 'drop user.score', 'drop column score from user ', 'updated', 220, '2015-01-16 01:56:01'),"
			"(5, 'u0004', 'u0005', 'update table users_ips', 'added column browser to users_ips', 'updated', 220, '2015-01-16 01:56:01'),"
			"(6, 'u0005', 'u0006', 'update table user', 'added column status to user', 'updated', 220, '2015-01-16 01:56:01'),"
			"(7, 'u0006', 'u0007', 'update state for user', 'update column status in user', 'updated', 220, '2015-01-16 01:56:01'),"
			"(8, 'u0007', 'u0008', 'added table users_skills', 'update column status in user', 'updated', 220, '2015-01-16 01:56:01'),"
			"(9, 'u0008', 'u0009', 'added table users_achievements', 'added table users_achievements', 'updated', 220, '2015-01-16 01:56:01'),"
			"(10, 'u0009', 'u0010', 'added column team', 'added column team', 'updated', 220, '2015-01-16 01:56:01'),"
			"(11, 'u0010', 'u0011', 'added table users_tokens', 'added table users_tokens', 'updated', 220, '2015-01-16 01:56:01'),"
			"(12, 'u0011', 'u0012', 'added table rules', 'added table rules', 'updated', 220, '2015-01-16 01:56:02'),"
			"(13, 'u0012', 'u0013', 'added table games_rules', 'added table games_rules', 'updated', 220, '2015-01-16 01:56:02'),"
			"(14, 'u0013', 'u0014', 'update logo for users', 'update logo for users', 'updated', 220, '2015-01-16 01:56:02'),"
			"(15, 'u0014', 'u0015', 'added columns', 'added column organizators, state and form to games', 'updated', 220, '2015-01-19 22:29:54'),"
			"(16, 'u0015', 'u0016', 'drop table teams and userteams', 'drop table teams and userteams', 'updated', 220, '2015-03-25 22:56:01'),"
			"(17, 'u0016', 'u0017', 'updated table games', 'updated table games', 'updated', 220, '2015-03-25 22:56:02'),"
			"(18, 'u0017', 'u0018', 'drop table games_rules', 'drop table games_rules', 'updated', 220, '2015-03-25 22:56:02'),"
			"(19, 'u0018', 'u0019', 'updated games', 'updated games', 'updated', 220, '2015-03-25 22:56:02'),"
			"(20, 'u0019', 'u0020', 'removed users_achievements, users_skills', 'removed users_achievements, users_skills', 'updated', 220, '2015-03-27 18:29:36'),"
			"(21, 'u0020', 'u0021', 'generate user uuids', 'generate user uuids', 'updated', 220, '2015-03-27 18:29:40'),"
			"(22, 'u0021', 'u0022', 'new table public_events', 'new table public_events', 'updated', 220, '2015-03-27 18:29:40'),"
			"(23, 'u0022', 'u0023', 'moved news to public events', 'moved news to public events', 'updated', 220, '2015-03-27 18:29:40'),"
			"(24, 'u0023', 'u0024', 'remove table news', 'remove table news', 'updated', 220, '2015-03-27 18:29:47'),"
			"(25, 'u0024', 'u0025', 'new column pass', 'new column pass', 'updated', 220, '2015-03-27 22:59:38'),"
			"(26, 'u0025', 'u0026', 'new table emails', 'new table emails', 'updated', 220, '2015-03-28 09:22:31'),"
			"(27, 'u0026', 'u0027', 'new table emails', 'new table emails', 'updated', 220, '2015-03-28 09:22:31'),"
			"(28, 'u0027', 'u0028', 'new table emails', 'new table emails', 'updated', 220, '2015-03-28 09:22:32'),"
			"(29, 'u0028', 'u0029', 'updated table games', 'updated table games', 'updated', 220, '2015-03-28 14:01:05'),"
			"(30, 'u0029', 'u0030', 'migrate data and removed id_game, tema', 'migrate data and removed id_game, tema', 'updated', 220, '2015-03-30 01:00:58'),"
			"(31, 'u0030', 'u0031', 'migrate data and removed name', 'migrate data and removed name', 'updated', 220, '2015-03-30 01:01:06'),"
			"(32, 'u0031', 'u0032', 'migrate data and removed text_copy', 'migrate data and removed text_copy', 'updated', 220, '2015-03-30 01:01:14'),"
			"(33, 'u0032', 'u0033', 'migrate data and removed answer_copy', 'migrate data and removed answer_copy', 'updated', 220, '2015-03-30 01:01:23'),"
			"(34, 'u0033', 'u0034', 'migrate data in tryanswer, tryanswer_backup', 'migrate data in tryanswer, tryanswer_backup', 'updated', 220, '2015-03-30 01:03:43'),"
			"(35, 'u0034', 'u0035', 'removed user.password', 'removed user.password', 'updated', 220, '2015-03-31 02:02:21'),"
			"(36, 'u0035', 'u0036', 'added table quests_files', 'added table quests_files', 'updated', 220, '2015-03-31 02:02:21'),"
			"(37, 'u0036', 'u0037', 'redesign table feedback', 'redesign table feedback', 'updated', 220, '2015-04-01 01:48:56'),"
			"(38, 'u0037', 'u0038', 'redesign table feedback_msg', 'redesign table feedback_msg', 'updated', 220, '2015-04-01 01:48:56'),"
			"(39, 'u0038', 'u0039', 'removed user.ipserver and user.username', 'removed user.ipserver and user.username', 'updated', 220, '2015-04-04 01:25:02'),"
			"(40, 'u0039', 'u0040', 'drop table advisers', 'drop table advisers', 'updated', 220, '2015-04-04 01:25:02'),"
			"(41, 'u0040', 'u0041', 'drop table flags and flags_live', 'drop table flags and flags_live', 'updated', 220, '2015-04-04 01:25:02'),"
			"(42, 'u0041', 'u0042', 'drop table scoreboard', 'drop table scoreboard', 'updated', 220, '2015-04-04 01:25:02'),"
			"(43, 'u0042', 'u0043', 'drop table services', 'drop table services', 'updated', 220, '2015-04-04 01:25:02'),"
			"(44, 'u0043', 'u0044', 'add quests_files.filepath', 'add quests_files.filepath', 'updated', 220, '2015-04-04 01:25:02'),"
			"(45, 'u0044', 'u0045', 'added table users', 'added table users', 'updated', 0, '2015-04-09 00:13:25'),"
			"(46, 'u0045', 'u0046', 'rename table user => user_old', 'rename table user => user_old', 'updated', 0, '2015-04-09 00:13:25'),"
			"(47, 'u0046', 'u0047', 'added table users', 'added table users', 'updated', 0, '2015-04-09 00:13:30'),"
			"(48, 'u0047', 'u0048', 'change message in events (for indexing and search)', 'change message in events (for indexing and search)', 'updated', 220, '2015-04-22 19:29:15'),"
			"(49, 'u0048', 'u0049', 'add column maxscore', 'add column maxscore', 'updated', 220, '2015-05-16 00:32:28'),"
			"(50, 'u0049', 'u0050', 'add column maxscore', 'add column maxscore', 'updated', 220, '2015-05-16 00:32:28'),"
			"(51, 'u0050', 'u0051', 'add column levenshtein', 'add column levenshtein', 'updated', 220, '2015-05-16 00:32:28'),"
			"(52, 'u0051', 'u0052', 'calculate levenshtein distance', 'calculate levenshtein distance', 'updated', 220, '2015-05-16 00:33:33'),"
			"(53, 'u0052', 'u0053', 'calculate levenshtein distance (backup)', 'calculate levenshtein distance (backup)', 'updated', 220, '2015-05-16 00:35:18'),"
			"(54, 'u0053', 'u0054', 'recalculate user scoreboards', 'recalculate user scoreboards', 'updated', 220, '2015-06-02 01:02:14'),"
			"(55, 'u0054', 'u0055', 'added new table users_quests', 'added new table users_quests', 'updated', 220, '2015-06-07 18:53:23'),"
			"(56, 'u0055', 'u0056', 'added cleanup users_games', 'added cleanup users_games', 'updated', 220, '2015-06-07 18:53:23'),"
			"(57, 'u0056', 'u0057', 'added table users_token_invalid', 'added table users_token_invalid', 'updated', 220, '2015-06-07 20:59:09'),"
			"(58, 'u0057', 'u0058', 'rename games.uuid_game games.uuid', 'rename games.uuid_game games.uuid', 'updated', 220, '2015-06-07 20:59:09'),"
			"(59, 'u0058', 'u0059', 'change userid and questid in users_quests to INT', 'change userid and questid in users_quests to INT', 'updated', 220, '2015-06-07 20:59:09'),"
			"(60, 'u0059', 'u0060', 'moved data from userquest to users_quests', 'moved data from userquest to users_quests', 'updated', 220, '2015-06-07 20:59:46');"
		);
		query.exec();
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
		query.exec();
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
		query.exec();
	}
 
	{
		QSqlQuery query(db);
		query.prepare(
			"INSERT INTO `users` (`uuid`, `email`, `pass`, `role`, `nick`, `logo`, `dt_create`, `dt_last_login`, `last_ip`, `status`) VALUES"
			"('39A551F4-3BF0-A1C8-8686-06A5C510DDA3', 'admin', '00fe92df464389f2da26c14475ad81e2632904fa', 'admin', 'Admin', 'files/users/0.png', '0000-00-00 00:00:00', '2015-04-12 23:49:58', '127.0.0.1', 'activated')"
		);
		query.exec();
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
		query.exec();
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
		query.exec();
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
		query.exec();
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
		query.exec();
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
		query.exec();
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
		query.exec();
	}
}
