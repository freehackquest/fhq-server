#include <update0085.h>

Update0085::Update0085(){
    TAG = "Update0085";
}

QString Update0085::from_version(){
	return "u0084";
}

QString Update0085::version(){
	return "u0085";
}

QString Update0085::description(){
	return "Renamed column quest_uuid to uuid";
}

bool Update0085::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE quest CHANGE COLUMN `quest_uuid` `uuid` varchar(255) NOT NULL;");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
	return true;
}
