#include "../headers/update0078.h"

QString Update0078::from_version(){
	return "u0077";
}

QString Update0078::version(){
	return "u0078";
}

QString Update0078::description(){
	return "Removed quest.min_score";
}

void Update0078::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE `quest` DROP COLUMN min_score;");
	query.exec();
}
