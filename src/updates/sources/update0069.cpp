#include <update0069.h>
#include <QSqlQuery>

Update0069::Update0069(){
    TAG = "Update0069";
}

QString Update0069::from_version(){
	return "u0068";
}

QString Update0069::version(){
	return "u0069";
}

QString Update0069::description(){
	return "Add columns to users";
}

bool Update0069::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE `users` "
		"ADD COLUMN `country` VARCHAR(255) DEFAULT '',"
		"ADD COLUMN `region` VARCHAR(255) DEFAULT '',"
		"ADD COLUMN `city` VARCHAR(255) DEFAULT '',"
		"ADD COLUMN `latitude` DOUBLE  DEFAULT 0.0,"
		"ADD COLUMN `longitude` DOUBLE  DEFAULT 0.0"
		";");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
	return true;
}
