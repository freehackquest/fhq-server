#include <update0093.h>

Update0093::Update0093(){
    TAG = "Update0093";
}

QString Update0093::from_version(){
    return "u0091";
}

QString Update0093::version(){
    return "u0093";
}

QString Update0093::description(){
    return "Add column ordered into table classbook";
}

bool Update0093::update(QSqlDatabase &db, QString &error){

    QSqlQuery query(db);
    query.prepare("ALTER TABLE classbook ADD COLUMN ordered INT(11) NOT NULL after parentid;");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "Problem with install update " + error);
        return false;
    }
    return true;

}
