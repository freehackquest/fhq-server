#include "update0070.h"
#include <QSqlQuery>


QString Update0070::from_version(){
	return "u0069";
}

QString Update0070::version(){
	return "u0070";
}

QString Update0070::name(){
	return "Removed table users_ips";
}

QString Update0070::description(){
	return "Removed table users_ips";
}

void Update0070::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("DROP TABLE `users_ips`;");
	query.exec();
}
