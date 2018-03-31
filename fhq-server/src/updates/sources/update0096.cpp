#include <update0096.h>

Update0096::Update0096(){
    TAG = "Update0096";
}

QString Update0096::from_version(){
    return "u0095";
}

QString Update0096::version(){
    return "u0096";
}

QString Update0096::description(){
    return "Add columns name_before and content_before to classbook_proposal";
}

bool Update0096::update(QSqlDatabase &db, QString &error){

    QSqlQuery query(db);
    query.prepare("ALTER TABLE classbook_proposal ADD COLUMN name_before VARCHAR(128) NOT NULL AFTER content");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    query.prepare("ALTER TABLE classbook_proposal ADD COLUMN content_before TEXT NOT NULL AFTER name_before");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

