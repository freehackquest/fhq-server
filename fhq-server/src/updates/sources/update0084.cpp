#include <update0084.h>

Update0084::Update0084(){
    TAG = "Update0084";
}

QString Update0084::from_version(){
	return "u0083";
}

QString Update0084::version(){
	return "u0084";
}

QString Update0084::description(){
	return "Add table settings";
}

bool Update0084::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare(
		"CREATE TABLE IF NOT EXISTS `settings` ("
		"	  `id` int(11) NOT NULL AUTO_INCREMENT,"
		"	  `name` varchar(128) NOT NULL,"
		"	  `value` varchar(255) NOT NULL,"
		"	  PRIMARY KEY (`id`)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with creating a table " + error);
		return false;
	}
	return true;
}
