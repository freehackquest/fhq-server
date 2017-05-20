#include "../headers/update0081.h"

QString Update0081::from_version(){
	return "u0080";
}

QString Update0081::version(){
	return "u0081";
}

QString Update0081::description(){
	return "Rename columns in users_quests_answers. "
		"iduser => userid, "
		"idquest => questid, "
		"datetime_try => dt, "
		"answer_try => user_answer, "
		"answer_real => quest_answer";
}

void Update0081::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `iduser` `userid` INTEGER NOT NULL;");
	query.exec();
	
	QSqlQuery query1(db);
	query1.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `idquest` `questid` INTEGER NOT NULL;");
	query1.exec();

	QSqlQuery query2(db);
	query2.prepare("ALTER TABLE users_quests_answers ADD COLUMN `user_answer` VARCHAR(255) NOT NULL;");
	query2.exec();
	
	QSqlQuery query3(db);
	query3.prepare("UPDATE users_quests_answers SET user_answer = LEFT(answer_try, 255);");
	query3.exec();

	QSqlQuery query4(db);
	query4.prepare("ALTER TABLE users_quests_answers DROP COLUMN `answer_try`");
	query4.exec();
	
	QSqlQuery query5(db);
	query5.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `answer_real` `quest_answer` VARCHAR(255) NOT NULL;");
	query5.exec();
	
	QSqlQuery query6(db);
	query6.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `datetime_try` `dt` DATETIME NOT NULL;");
	query6.exec();
}
