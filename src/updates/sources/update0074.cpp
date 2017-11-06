#include <update0074.h>

Update0074::Update0074(){
    TAG = "Update0074";
}

QString Update0074::from_version(){
	return "u0073";
}

QString Update0074::version(){
	return "u0074";
}

QString Update0074::description(){
	return "Added feedback.from";
}

bool Update0074::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE `feedback` ADD COLUMN `from` VARCHAR(255) DEFAULT '';");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
	return true;
}
