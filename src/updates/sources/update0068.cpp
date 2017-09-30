#include "../headers/update0068.h"
#include <QSqlQuery>


QString Update0068::from_version(){
	return "u0067";
}

QString Update0068::version(){
	return "u0068";
}

QString Update0068::description(){
	return "Added table hints";
}

bool Update0068::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare(
		"CREATE TABLE IF NOT EXISTS `quests_hints` ("
		"	  `id` int(11) NOT NULL AUTO_INCREMENT,"
		"	  `questid` int(11) NOT NULL,"
		"	  `text` varchar(4048) NOT NULL,"
		"	  `dt` datetime NOT NULL,"
		"	  PRIMARY KEY (`id`)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
	if(!query.exec()){
		error = query.lastError().text();
		return false;
	}
	return true;
}
