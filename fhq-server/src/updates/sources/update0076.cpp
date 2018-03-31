#include <update0076.h>

Update0076::Update0076(){
    TAG = "Update0076";
}

QString Update0076::from_version(){
	return "u0075";
}

QString Update0076::version(){
	return "u0076";
}

QString Update0076::description(){
	return "Added users.score";
}

bool Update0076::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE `users` ADD COLUMN `rating` INTEGER DEFAULT 0;");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
	
	QSqlQuery query3(db);
	query3.prepare(
		"  SELECT SUM(q.score) as rating, uq.userid FROM users_quests uq "
		"   INNER JOIN quest q ON uq.questid = q.idquest"
		"  GROUP BY uq.userid"
	);
	if(!query3.exec()){
		error = query3.lastError().text();
        Log::err(TAG, "The problem with data selection " + error);
		return false;
	}

	while (query3.next()) {
		QSqlRecord record = query3.record();
		int userid = record.value("userid").toInt();
		int rating = record.value("rating").toInt();
		
		QSqlQuery query2(db);
		query2.prepare("UPDATE users SET rating = :rating WHERE id = :userid");
		query2.bindValue(":rating", rating);
		query2.bindValue(":userid", userid);
		if(!query2.exec()){
			error = query2.lastError().text();
            Log::err(TAG, "The problem with updating the data " + error);
			return false;
		}
	}
	return true;
}
