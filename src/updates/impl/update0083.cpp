#include "../headers/update0083.h"

QString Update0083::from_version(){
	return "u0082";
}

QString Update0083::version(){
	return "u0083";
}

QString Update0083::description(){
	return "Create table users_offers";
}

void Update0083::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare(
		"CREATE TABLE IF NOT EXISTS `users_offers` ("
		"	  `id` int(11) NOT NULL AUTO_INCREMENT,"
		"	  `userid` int(11) NOT NULL,"
		"	  `data` text NOT NULL,"
		"	  `type` varchar(255) NOT NULL,"
		"	  `scomment` varchar(255) NOT NULL,"
		"	  `status` varchar(255) NOT NULL,"
		"	  `created` datetime NOT NULL,"
		"	  `updated` datetime NOT NULL,"
		"	  PRIMARY KEY (`id`)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
	query.exec();
}
