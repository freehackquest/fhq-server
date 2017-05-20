#include "../headers/update0075.h"

QString Update0075::from_version(){
	return "u0074";
}

QString Update0075::version(){
	return "u0075";
}

QString Update0075::name(){
	return "Added table quests_writeups";
}

QString Update0075::description(){
	return "Added table quests_writeups";
}

void Update0075::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare(
		"CREATE TABLE IF NOT EXISTS `quests_writeups` ("
		"	  `id` int(11) NOT NULL AUTO_INCREMENT,"
		"	  `questid` int(11) NOT NULL,"
		"	  `type` varchar(64) NOT NULL,"
		"	  `link` varchar(1024) NOT NULL,"
		"	  `text` varchar(4048) NOT NULL,"
		"	  `dt` datetime NOT NULL,"
		"	  PRIMARY KEY (`id`)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
	query.exec();
}
