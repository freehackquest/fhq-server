#include <update0066.h>

Update0066::Update0066(){
    TAG = "Update0066";
}

QString Update0066::from_version(){
	return "u0065";
}

QString Update0066::version(){
	return "u0066";
}

QString Update0066::description(){
	return "Drop column quest.for_person";
}

bool Update0066::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE quest DROP COLUMN for_person");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
	return true;
}
