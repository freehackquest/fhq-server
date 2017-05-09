#include "update0064.h"

QString Update0064::from_version(){
	return "u0063";
}

QString Update0064::version(){
	return "u0064";
}

QString Update0064::name(){
	return "Remove personal quests which passed";
}

QString Update0064::description(){
	return "Remove personal quests which passed";
}

void Update0064::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("SELECT idquest FROM quest LEFT JOIN users_quests ON users_quests.questid = quest.idquest WHERE for_person <> 0");
	query.exec();

	while (query.next()) {
		QSqlRecord record = query.record();
		int questid = record.value("idquest").toInt();
		QSqlQuery query2(db);
		query.prepare("DELETE FROM quest WHERE idquest = :questid");
		query.bindValue(":questid", questid);
		query.exec();
	}
}
