#include "create_list_updates.h"
#include "headers/database0060.h"
#include "headers/update0061.h"
#include "headers/update0062.h"
#include "headers/update0063.h"
#include "headers/update0064.h"
#include "headers/update0065.h"
#include "headers/update0066.h"
#include "headers/update0067.h"
#include "headers/update0068.h"
#include "headers/update0069.h"
#include "headers/update0070.h"
#include "headers/update0071.h"
#include "headers/update0072.h"
#include "headers/update0073.h"
#include "headers/update0074.h"
#include "headers/update0075.h"
#include "headers/update0076.h"
#include "headers/update0077.h"
#include "headers/update0078.h"

void create_list_updates(QVector<IUpdate *> &vUpdates){
	vUpdates.push_back(new Database0060());
	vUpdates.push_back(new Update0061());
	vUpdates.push_back(new Update0062());
	vUpdates.push_back(new Update0063());
	vUpdates.push_back(new Update0064());
	vUpdates.push_back(new Update0065());
	vUpdates.push_back(new Update0066());
	vUpdates.push_back(new Update0067());
	vUpdates.push_back(new Update0068());
	vUpdates.push_back(new Update0069());
	vUpdates.push_back(new Update0070());
	vUpdates.push_back(new Update0071());
	vUpdates.push_back(new Update0072());
	vUpdates.push_back(new Update0073());
	vUpdates.push_back(new Update0074());
	vUpdates.push_back(new Update0075());
	vUpdates.push_back(new Update0076());
	vUpdates.push_back(new Update0077());
	vUpdates.push_back(new Update0078());
}

void tryUpdateDatabase(QSqlDatabase *pDatabase){
	QSqlDatabase db = *pDatabase;
	QSqlQuery query(db);

	query.prepare("SELECT * FROM updates ORDER BY id DESC LIMIT 0,1");
	query.exec();
	QString last_version = "";
	if (query.next()) {
		QSqlRecord record = query.record();
		// int updateid = record.value("id").toInt();
		QString version = record.value("version").toString();
		QString description = record.value("description").toString();
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
				qDebug().nospace() << "Installing update " << pUpdate->from_version() << " -> " << pUpdate->version() << ": " << pUpdate->description();
				last_version = pUpdate->version();
				bHasUpdates = true;
				pUpdate->update(db);

				QSqlQuery insert_query(db);
				insert_query.prepare("INSERT INTO updates (version, description, datetime_update) "
					  "VALUES (:version, :description, NOW())");
				insert_query.bindValue(":version", pUpdate->version());
				insert_query.bindValue(":description", pUpdate->description());
				if(!insert_query.exec()){
					qDebug() << "[ERROR] sql error: " << insert_query.lastError();
					return;
				}
			}
		}
	}
}
