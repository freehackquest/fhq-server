#include <update0099.h>

Update0099::Update0099()
    : UpdateBase("u0098", "u0099", "Update table settings"){
}

bool Update0099::update(QSqlDatabase &db, std::string &error){

    {
        QSqlQuery query(db);
        query.prepare(""
            "ALTER TABLE settings MODIFY COLUMN value VARCHAR(1024);"
        if(!query.exec()){
            error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
            return false;
        }
    }
    return true;
}
