#include <update0094.h>

Update0094::Update0094(){
    TAG = "Update0094";
}

QString Update0094::from_version(){
    return "u0093";
}

QString Update0094::version(){
    return "u0094";
}

QString Update0094::description(){
    return "Removed column logo from games";
}

bool Update0094::update(QSqlDatabase &db, QString &error){

    QSqlQuery query(db);
    query.prepare("ALTER TABLE classbook DROP COLUMN logo;");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}
