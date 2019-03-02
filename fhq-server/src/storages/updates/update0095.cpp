#include <update0095.h>

Update0095::Update0095()
    : UpdateBase("u0094", "u0095", "Add columns md5_content and uuid to classbook, classbook_localization and classbook_proposal"){
}

bool Update0095::update(QSqlDatabase &db, std::string &error){

    QSqlQuery query(db);
    query.prepare("ALTER TABLE classbook ADD COLUMN md5_content CHAR(32) NOT NULL AFTER content");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query2(db);
    query2.prepare("ALTER TABLE classbook_localization ADD COLUMN md5_content CHAR(32) NOT NULL AFTER content");
    if(!query2.exec()){
        error = query2.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query3(db);
    query3.prepare("ALTER TABLE classbook_localization ADD COLUMN uuid VARCHAR(128) NOT NULL AFTER classbookid");
    if(!query3.exec()){
        error = query3.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query4(db);
    query4.prepare("ALTER TABLE classbook_proposal ADD COLUMN md5_content CHAR(32) NOT NULL AFTER content");
    if(!query4.exec()){
        error = query4.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query5(db);
    query5.prepare("ALTER TABLE classbook_proposal ADD COLUMN uuid VARCHAR(128) NOT NULL AFTER classbookid");
    if(!query5.exec()){
        error = query3.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

