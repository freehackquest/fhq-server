#include <update0095.h>

Update0095::Update0095(){
    TAG = "Update0095";
}

QString Update0095::from_version(){
    return "u0094";
}

QString Update0095::version(){
    return "u0095";
}

QString Update0095::description(){
    return "Add columns md5_content and uuid to classbook and classbook_localization";
}

bool Update0095::update(QSqlDatabase &db, QString &error){

    QSqlQuery query(db);
    query.prepare("ALTER TABLE classbook ADD COLUMN md5_content CHAR(32) NOT NULL AFTER content");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query2(db);
    query2.prepare("ALTER TABLE classbook_localization ADD COLUMN md5_content CHAR(32) NOT NULL AFTER content");
    if(!query2.exec()){
        error = query2.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query3(db);
    query3.prepare("ALTER TABLE classbook_localization ADD COLUMN uuid VARCHAR(128) NOT NULL AFTER classbookid");
    if(!query3.exec()){
        error = query3.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

