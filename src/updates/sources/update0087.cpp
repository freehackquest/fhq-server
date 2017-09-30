#include "../headers/update0087.h"
#include <log.h>

Update0087::Update0087(){
	TAG = "Update0087";
}
	
QString Update0087::from_version(){
	return "u0086";
}

QString Update0087::version(){
	return "u0087";
}

QString Update0087::description(){
	return "Added column about to table users";
}

bool Update0087::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE users ADD COLUMN `about` TEXT NOT NULL");
	if(!query.exec()){
		error = query.lastError().text();
		Log::info(TAG, "Problem with install update " + error);
		return false;
	}
	return true;
}
