#include "../headers/update0062.h"

QString Update0062::from_version(){
	return "u0061";
}

QString Update0062::version(){
	return "u0062";
}

QString Update0062::description(){
	return "Remove table user_old";
}

void Update0062::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("DROP TABLE user_old");
	query.exec();
}
