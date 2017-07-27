#include "../headers/update0086.h"

QString Update0086::from_version(){
	return "u0085";
}

QString Update0086::version(){
	return "u0086";
}

QString Update0086::description(){
	return "Removed column idauthor from table quest";
}

void Update0086::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE quest DROP COLUMN `idauthor`");
	query.exec();
}
