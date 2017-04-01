#include "create_list_updates.h"
#include "update0067.h"
#include "update0068.h"
#include "update0069.h"
#include "update0070.h"
#include "update0071.h"

void create_list_updates(QVector<IUpdate *> &vUpdates){
	vUpdates.push_back(new Update0067());
	vUpdates.push_back(new Update0068());
	vUpdates.push_back(new Update0069());
	vUpdates.push_back(new Update0070());
	vUpdates.push_back(new Update0071());
}

void tryUpdateDatabase(QSqlDatabase *pDatabase){
	QSqlDatabase db = *pDatabase;
	QSqlQuery query(db);

	query.prepare("SELECT * FROM updates ORDER BY id ASC");
	query.exec();
	QString last_version;
	while (query.next()) {
		QSqlRecord record = query.record();
		// int updateid = record.value("id").toInt();
		QString from_version = record.value("from_version").toString();
		QString version = record.value("version").toString();
		QString name = record.value("name").toString();
		QString description = record.value("description").toString();
		QString result = record.value("result").toString();
		// int userid = record.value("userid").toInt();
		last_version = version;
	}
	
	qDebug() << "last_version:" << last_version;
	QVector<IUpdate *> vUpdates;
	create_list_updates(vUpdates);

	bool bHasUpdates = true;
	while(bHasUpdates){
		bHasUpdates = false;
		for(int i = 0; i < vUpdates.size(); i++){
			IUpdate* pUpdate = vUpdates[i];
			if(last_version == pUpdate->from_version()){
				qDebug().nospace().noquote() << "Installing update " << pUpdate->from_version() << " -> " << pUpdate->version() << ": " << pUpdate->name();
				last_version = pUpdate->version();
				bHasUpdates = true;
				pUpdate->update(db);

				QSqlQuery insert_query(db);
				insert_query.prepare("INSERT INTO updates (from_version, version, name, description, result, userid, datetime_update) "
					  "VALUES (:from_version, :version, :name, :description, :result, :userid, NOW())");
				insert_query.bindValue(":from_version", pUpdate->from_version());
				insert_query.bindValue(":version", pUpdate->version());
				insert_query.bindValue(":name", pUpdate->name());
				insert_query.bindValue(":description", pUpdate->description());
				insert_query.bindValue(":result", "updated");
				insert_query.bindValue(":userid", 0);
				if(!insert_query.exec()){
					qDebug() << "[ERROR] sql error: " << insert_query.lastError();
					return;
				}
			}
		}
	}
}
