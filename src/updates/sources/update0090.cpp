#include <update0090.h>

Update0090::Update0090(){
    TAG = "Update0090";
}

QString Update0090::from_version(){
    return "u0089";
}

QString Update0090::version(){
    return "u0090";
}

QString Update0090::description(){
    return "Added column type to table settings";
}

bool Update0090::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
    query.prepare("ALTER TABLE users ADD COLUMN `university` VARCHAR(255) DEFAULT ''");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
    return true;
}
