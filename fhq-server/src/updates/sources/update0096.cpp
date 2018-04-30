#include <update0096.h>

Update0096::Update0096()
    : UpdateBase("u0095", "u0096", "Add columns name_before and content_before to classbook_proposal"){
}

bool Update0096::update(QSqlDatabase &db, std::string &error){

    QSqlQuery query(db);
    query.prepare("ALTER TABLE classbook_proposal ADD COLUMN name_before VARCHAR(128) NOT NULL AFTER content");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    query.prepare("ALTER TABLE classbook_proposal ADD COLUMN content_before TEXT NOT NULL AFTER name_before");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

