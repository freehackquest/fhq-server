#include <update0081.h>

Update0081::Update0081(){
    TAG = "Update0081";
}

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

bool Update0081::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `iduser` `userid` INTEGER NOT NULL;");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
	
	QSqlQuery query1(db);
	query1.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `idquest` `questid` INTEGER NOT NULL;");
	if(!query1.exec()){
		error = query1.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}

	QSqlQuery query2(db);
	query2.prepare("ALTER TABLE users_quests_answers ADD COLUMN `user_answer` VARCHAR(255) NOT NULL;");
	if(!query2.exec()){
		error = query2.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
	
	QSqlQuery query3(db);
	query3.prepare("UPDATE users_quests_answers SET user_answer = LEFT(answer_try, 255);");
	if(!query3.exec()){
		error = query3.lastError().text();
        Log::err(TAG, "The problem with updating the data " + error);
		return false;
	}

	QSqlQuery query4(db);
	query4.prepare("ALTER TABLE users_quests_answers DROP COLUMN `answer_try`");
	if(!query4.exec()){
		error = query4.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
	
	
	QSqlQuery query5(db);
	query5.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `answer_real` `quest_answer` VARCHAR(255) NOT NULL;");
	if(!query5.exec()){
		error = query5.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
	
	
	QSqlQuery query6(db);
	query6.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `datetime_try` `dt` DATETIME NOT NULL;");
	if(!query6.exec()){
		error = query6.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
		return false;
	}
	return true;
}
