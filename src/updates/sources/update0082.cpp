#include <update0082.h>

QString Update0082::from_version(){
	return "u0081";
}

QString Update0082::version(){
	return "u0082";
}

QString Update0082::description(){
	return "New column answer_format in table quest ";
}

bool Update0082::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE quest ADD COLUMN `answer_format` VARCHAR(255) DEFAULT '';");
	if(!query.exec()){
		error = query.lastError().text();
		return false;
	}
	return true;
}
