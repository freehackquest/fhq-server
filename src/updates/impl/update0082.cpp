#include "../headers/update0082.h"

QString Update0082::from_version(){
	return "u0081";
}

QString Update0082::version(){
	return "u0082";
}

QString Update0082::description(){
	return "New column answer_format in table quest ";
}

void Update0082::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE quest ADD COLUMN `answer_format` VARCHAR(255) DEFAULT '';");
	query.exec();
}
