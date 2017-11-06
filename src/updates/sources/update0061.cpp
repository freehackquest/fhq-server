#include <update0061.h>
#include <QSqlQuery>

Update0061::Update0061(){
    TAG = "Update0061";
}

QString Update0061::from_version(){
	return "u0060";
}

QString Update0061::version(){
	return "u0061";
}

QString Update0061::description(){
	return "remove personal quests which not passed";
}

bool Update0061::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("SELECT idquest FROM quest LEFT JOIN users_quests ON users_quests.questid = quest.idquest WHERE for_person <> 0 AND isnull( dt_passed )");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with data selection " + error);
		return false;
	}

	while (query.next()) {
		QSqlRecord record = query.record();
		int questid = record.value("idquest").toInt();
		QSqlQuery query2(db);
		query2.prepare("DELETE FROM quest WHERE idquest = :questid");
		query2.bindValue(":questid", questid);
		if(!query2.exec()){
			error = query2.lastError().text();
            Log::err(TAG, "The problem with deleting data " + error);
			return false;
		}
	}
	return true;
}
