#include "../headers/update0061.h"
#include <QSqlQuery>

QString Update0061::from_version(){
	return "u0060";
}

QString Update0061::version(){
	return "u0061";
}

QString Update0061::description(){
	return "remove personal quests which not passed";
}

void Update0061::update(QSqlDatabase &db){
	QSqlQuery query(db);
	query.prepare("SELECT idquest FROM quest LEFT JOIN users_quests ON users_quests.questid = quest.idquest WHERE for_person <> 0 AND isnull( dt_passed )");
	query.exec();

	while (query.next()) {
		QSqlRecord record = query.record();
		int questid = record.value("idquest").toInt();
		QSqlQuery query2(db);
		query2.prepare("DELETE FROM quest WHERE idquest = :questid");
		query2.bindValue(":questid", questid);
		query2.exec();
	}
}
