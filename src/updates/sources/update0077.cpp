#include <update0077.h>

QString Update0077::from_version(){
	return "u0076";
}

QString Update0077::version(){
	return "u0077";
}

QString Update0077::description(){
	return "Removed from_version,name,result,userid from updates";
}

bool Update0077::update(QSqlDatabase &db, QString &error){
	{
		QSqlQuery query(db);
		query.prepare("ALTER TABLE `updates` DROP COLUMN from_version;");
		if(!query.exec()){
			error = query.lastError().text();
			return false;
		}
	}
	
	{
		QSqlQuery query(db);
		query.prepare("ALTER TABLE `updates` DROP COLUMN name;");
		if(!query.exec()){
			error = query.lastError().text();
			return false;
		}
	}
	
	{
		QSqlQuery query(db);
		query.prepare("ALTER TABLE `updates` DROP COLUMN result;");
		if(!query.exec()){
			error = query.lastError().text();
			return false;
		}
	}
	
	{
		QSqlQuery query(db);
		query.prepare("ALTER TABLE `updates` DROP COLUMN userid;");
		if(!query.exec()){
			error = query.lastError().text();
			return false;
		}
	}
	return true;
}
