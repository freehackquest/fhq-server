#include "../headers/update0076.h"

QString Update0076::from_version(){
	return "u0075";
}

QString Update0076::version(){
	return "u0076";
}

QString Update0076::name(){
	return "Added users.score";
}

QString Update0076::description(){
	return "Added users.score";
}

void Update0076::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE `users` ADD COLUMN `rating` INTEGER DEFAULT 0;");
	query.exec();
	
	QSqlQuery query3(db);
	query3.prepare(
		"  SELECT SUM(q.score) as rating, uq.userid FROM users_quests uq "
		"   INNER JOIN quest q ON uq.questid = q.idquest"
		"  GROUP BY uq.userid"
	);
	query3.exec();

	while (query3.next()) {
		QSqlRecord record = query3.record();
		int userid = record.value("userid").toInt();
		int rating = record.value("rating").toInt();
		
		QSqlQuery query2(db);
		query2.prepare("UPDATE users SET rating = :rating WHERE id = :userid");
		query2.bindValue(":rating", rating);
		query2.bindValue(":userid", userid);
		query2.exec();
	}
}
