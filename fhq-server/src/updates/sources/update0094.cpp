#include <update0094.h>

Update0094::Update0094()
    : UpdateBase("u0093", "u0094", "Removed column logo from games"){
}

bool Update0094::update(QSqlDatabase &db, std::string &error){

    QSqlQuery query(db);
    query.prepare("ALTER TABLE games DROP COLUMN logo;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}
