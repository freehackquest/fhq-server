#include <update0066.h>

QString Update0066::from_version(){
	return "u0065";
}

QString Update0066::version(){
	return "u0066";
}

QString Update0066::description(){
	return "Drop column quest.for_person";
}

bool Update0066::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE quest DROP COLUMN for_person");
	if(!query.exec()){
		error = query.lastError().text();
		return false;
	}
	return true;
}
