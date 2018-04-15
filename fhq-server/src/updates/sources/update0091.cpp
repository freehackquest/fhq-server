#include <update0091.h>

Update0091::Update0091()
    : UpdateBase("u0090", "u0091", "Deleted column in table classbook: \"name_ru, name_en, dt\""
                                   "Added column to table classbook: \"name, content, created, updated\""){

}

bool Update0091::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE classbook DROP COLUMN name_ru;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query1(db);
    query1.prepare("ALTER TABLE classbook DROP COLUMN name_en;");
    if(!query1.exec()){
        error = query1.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query2(db);
    query2.prepare("ALTER TABLE classbook DROP COLUMN dt;");
    if(!query2.exec()){
        error = query2.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query3(db);
    query3.prepare("ALTER TABLE classbook ADD name VARCHAR(128) NOT NULL after parentuuid;");
    if(!query3.exec()){
        error = query3.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query4(db);
    query4.prepare("ALTER TABLE classbook ADD content TEXT NOT NULL after name;");
    if(!query4.exec()){
        error = query4.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query5(db);
    query5.prepare("ALTER TABLE classbook ADD COLUMN created datetime NOT NULL after content;");
    if(!query5.exec()){
        error = query5.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query6(db);
    query6.prepare("ALTER TABLE classbook ADD COLUMN updated datetime NOT NULL after created;");
    if(!query6.exec()){
        error = query6.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    return true;

}
