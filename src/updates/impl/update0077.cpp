#include "../headers/update0077.h"

QString Update0077::from_version(){
	return "u0076";
}

QString Update0077::version(){
	return "u0077";
}

QString Update0077::description(){
	return "Removed from_version,name,result,userid from updates";
}

void Update0077::update(QSqlDatabase &db){
	{
		QSqlQuery query(db);
		query.prepare("ALTER TABLE `updates` DROP COLUMN from_version;");
		query.exec();
	}
	
	{
		QSqlQuery query(db);
		query.prepare("ALTER TABLE `updates` DROP COLUMN name;");
		query.exec();
	}
	
	{
		QSqlQuery query(db);
		query.prepare("ALTER TABLE `updates` DROP COLUMN result;");
		query.exec();
	}
	
	{
		QSqlQuery query(db);
		query.prepare("ALTER TABLE `updates` DROP COLUMN userid;");
		query.exec();
	}
}
