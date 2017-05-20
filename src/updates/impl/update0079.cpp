#include "../headers/update0079.h"

QString Update0079::from_version(){
	return "u0078";
}

QString Update0079::version(){
	return "u0079";
}

QString Update0079::description(){
	return "Rename table tryanswer to users_quests_answers";
}

void Update0079::update(QSqlDatabase &db){
	{
		QSqlQuery query(db);
		query.prepare("RENAME TABLE tryanswer TO users_quests_answers;");
		query.exec();
	}
}
