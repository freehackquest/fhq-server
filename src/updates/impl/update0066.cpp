#include "../headers/update0066.h"

QString Update0066::from_version(){
	return "u0065";
}

QString Update0066::version(){
	return "u0066";
}

QString Update0066::name(){
	return "Drop column quest.for_person";
}

QString Update0066::description(){
	return "Drop column quest.for_person";
}

void Update0066::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE quest DROP COLUMN for_person");
	query.exec();
}
