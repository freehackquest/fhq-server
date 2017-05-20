#include "../headers/update0080.h"

QString Update0080::from_version(){
	return "u0079";
}

QString Update0080::version(){
	return "u0080";
}

QString Update0080::description(){
	return "Merge table tryanswer_backup to users_quests_answers and drop this";
}

void Update0080::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("INSERT INTO users_quests_answers (iduser, idquest, answer_try, answer_real, passed, levenshtein, datetime_try)"
				"SELECT iduser, idquest, answer_try, answer_real, passed, levenshtein, datetime_try FROM tryanswer_backup;");
	query.exec();
	
	QSqlQuery query2(db);
	query2.prepare("DROP TABLE IF EXISTS tryanswer_backup");
	query2.exec();
	
}
