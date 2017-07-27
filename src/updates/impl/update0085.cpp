#include "../headers/update0085.h"

QString Update0085::from_version(){
	return "u0084";
}

QString Update0085::version(){
	return "u0085";
}

QString Update0085::description(){
	return "Renamed column quest_uuid to uuid";
}

void Update0085::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("ALTER TABLE quest CHANGE COLUMN `quest_uuid` `uuid` varchar(255) NOT NULL;");
	query.exec();
}
