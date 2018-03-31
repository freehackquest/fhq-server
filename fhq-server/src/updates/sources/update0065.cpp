#include <update0065.h>
#include <QUuid>

Update0065::Update0065(){
    TAG = "Update0065";
}

QString Update0065::from_version(){
	return "u0064";
}

QString Update0065::version(){
	return "u0065";
}

QString Update0065::description(){
	return "Update quests uuid";
}

bool Update0065::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("SELECT idquest FROM quest WHERE isnull(quest_uuid)");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with data selection " + error);
		return false;
	}

	while (query.next()) {
		QSqlRecord record = query.record();
		int questid = record.value("idquest").toInt();
		QString questuuid = QUuid::createUuid().toString();
		questuuid = questuuid.mid(1,questuuid.length()-2);
		QSqlQuery query2(db);
		query2.prepare("UPDATE quest SET quest_uuid = :questuuid WHERE idquest = :questid AND isnull(quest_uuid)");
		query2.bindValue(":questuuid", questuuid);
		query2.bindValue(":questid", questid);
		if(!query2.exec()){
			error = query2.lastError().text();
            Log::err(TAG, "The problem with updating the data " + error);
			return false;
		}
	}
	return true;
}
