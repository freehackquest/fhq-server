#include <update0088.h>

Update0088::Update0088(){
	TAG = "Update0088";
}
	
QString Update0088::from_version(){
	return "u0087";
}

QString Update0088::version(){
	return "u0088";
}

QString Update0088::description(){
	return "Added column group to table settings";
}

bool Update0088::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE settings ADD COLUMN `group` VARCHAR(255) DEFAULT ''");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
	return true;
}
