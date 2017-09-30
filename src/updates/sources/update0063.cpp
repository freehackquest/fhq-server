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

bool Update0063::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("DROP TABLE userquest");
	if(!query.exec()){
		error = query.lastError().text();
		return false;
	}
	return true;
}
