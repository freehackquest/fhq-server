#include <update0078.h>

Update0078::Update0078(){
    TAG = "Update0078";
}

QString Update0078::from_version(){
	return "u0077";
}

QString Update0078::version(){
	return "u0078";
}

QString Update0078::description(){
	return "Removed quest.min_score";
}

bool Update0078::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE `quest` DROP COLUMN min_score;");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
	return true;
}
