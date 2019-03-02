#include <update0097.h>

Update0097::Update0097()
    : UpdateBase("u0096", "u0097", "Add new table quests_proposal"){
}

bool Update0097::update(QSqlDatabase &db, std::string &error){
	QSqlQuery query(db);
	query.prepare(""
		"CREATE TABLE IF NOT EXISTS `quests_proposal` ("
		"  `id` int(11) NOT NULL AUTO_INCREMENT,"
		"  `userid` int(11) NOT NULL,"
		"  `gameid` int(11) NOT NULL,"
		"  `name` varchar(255) NOT NULL,"
		"  `text` varchar(4048) NOT NULL,"
		"  `answer` varchar(128) NOT NULL,"
		"  `score` int(11) NOT NULL,"
		"  `author` varchar(128) NOT NULL,"
		"  `subject` varchar(128) NOT NULL,"
		"  `copyright` text NOT NULL,"
		"  `answer_format` text NOT NULL,"
		"  `created` datetime NOT NULL,"
		"  `updated` datetime NOT NULL,"
		"  `confirmed` varchar(128) NOT NULL,"
		"  PRIMARY KEY (`id`)"
		") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
	if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with creating a table " + error);
		return false;
	}
    return true;
}

