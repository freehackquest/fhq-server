#include <update0093.h>

Update0093::Update0093()
    : UpdateBase("u0092", "u0093", "Add column ordered into table classbook"){
}

bool Update0093::update(QSqlDatabase &db, std::string &error){

    QSqlQuery query(db);
    query.prepare("ALTER TABLE classbook ADD COLUMN ordered INT(11) NOT NULL after parentid;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;

}
