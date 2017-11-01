#include <update0086.h>

Update0086::Update0086(){
    TAG = "Update0086";
}

QString Update0086::from_version(){
	return "u0085";
}

QString Update0086::version(){
	return "u0086";
}

QString Update0086::description(){
	return "Removed column idauthor from table quest";
}

bool Update0086::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE quest DROP COLUMN `idauthor`");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
	return true;
}
