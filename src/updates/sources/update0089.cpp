#include <update0089.h>

Update0089::Update0089(){
	TAG = "Update0089";
}
	
QString Update0089::from_version(){
	return "u0088";
}

QString Update0089::version(){
	return "u0089";
}

QString Update0089::description(){
	return "Added column type to table settings";
}

bool Update0089::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE settings ADD COLUMN `type` VARCHAR(255) DEFAULT 'string'");
	if(!query.exec()){
		error = query.lastError().text();
		Log::info(TAG, "Problem with install update " + error);
		return false;
	}
	return true;
}
