#include <update0070.h>
#include <QSqlQuery>

Update0070::Update0070(){
    TAG = "Update0070";
}

QString Update0070::from_version(){
	return "u0069";
}

QString Update0070::version(){
	return "u0070";
}

QString Update0070::description(){
	return "Removed table users_ips";
}

bool Update0070::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("DROP TABLE `users_ips`;");
	if(!query.exec()){
		error = query.lastError().text();
        Log::err(TAG, "The problem with removing the table " + error);
		return false;
	}
	return true;
}
