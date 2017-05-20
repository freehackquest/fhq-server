#include "../headers/update0063.h"

QString Update0063::from_version(){
	return "u0062";
}

QString Update0063::version(){
	return "u0063";
}

QString Update0063::description(){
	return "Remove table userquest";
}

void Update0063::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("DROP TABLE userquest");
	query.exec();
}
