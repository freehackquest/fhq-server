#include "update0067.h"
#include <QSqlQuery>


QString Update0067::from_version(){
	return "u0066";
}

QString Update0067::version(){
	return "u0067";
}

QString Update0067::name(){
	return "Added column copyright to quest";
}

QString Update0067::description(){
	return "Added column copyright to quest";
}

void Update0067::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE quest ADD COLUMN copyright VARCHAR(255);");
	query.exec();
}
