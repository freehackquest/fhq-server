#include <update0062.h>

Update0062::Update0062(){
    TAG = "Update0062";
}

QString Update0062::from_version(){
	return "u0061";
}

QString Update0062::version(){
	return "u0062";
}

QString Update0062::description(){
	return "Remove table user_old";
}

bool Update0062::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
    query.prepare("DROP TABLE IF EXISTS user_old");
		if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with removing the table " + error);
		return false;
	}
	return true;
}
