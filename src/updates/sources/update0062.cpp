#include <update0062.h>

QString Update0062::from_version(){
	return "u0061";
}

QString Update0062::version(){
	return "u0062";
}

QString Update0062::description(){
	return "Remove table user_old";
}

bool Update0062::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("DROP TABLE user_old");
		if(!query.exec()){
		error = query.lastError().text();
		return false;
	}
	return true;
}
