#include <update0091.h>

Update0091::Update0091(){
    TAG = "Update0091";
}

QString Update0091::from_version(){
    return "u0090";
}

QString Update0091::version(){
    return "u0091";
}

QString Update0091::description(){
    return "Deleted column in table classbook: \"name_ru, name_en, dt\""
           "Added column to table classbook: \"name, content, created, updated\"";
}

bool Update0091::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE classbook DROP COLUMN name_ru;");
    if(!query.exec()){
        error = query.lastError().text();
        Log::info(TAG, "Problem with install update " + error);
        return false;
    }

    QSqlQuery query1(db);
    query1.prepare("ALTER TABLE classbook DROP COLUMN name_en;");
    if(!query1.exec()){
        error = query1.lastError().text();
        Log::info(TAG, "Problem with install update " + error);
        return false;
    }

    QSqlQuery query2(db);
    query2.prepare("ALTER TABLE classbook DROP COLUMN dt;");
    if(!query2.exec()){
        error = query2.lastError().text();
        Log::info(TAG, "Problem with install update " + error);
        return false;
    }

    QSqlQuery query3(db);
    query3.prepare("ALTER TABLE classbook ADD name VARCHAR(128) NOT NULL after parentuuid;");
    if(!query3.exec()){
        error = query3.lastError().text();
        Log::info(TAG, "Problem with install update " + error);
        return false;
    }

    QSqlQuery query4(db);
    query4.prepare("ALTER TABLE classbook ADD content TEXT NOT NULL after name;");
    if(!query4.exec()){
        error = query4.lastError().text();
        Log::info(TAG, "Problem with install update " + error);
        return false;
    }

    QSqlQuery query5(db);
    query5.prepare("ALTER TABLE classbook ADD COLUMN created datetime NOT NULL after content;");
    if(!query5.exec()){
        error = query5.lastError().text();
        Log::info(TAG, "Problem with install update " + error);
        return false;
    }

    QSqlQuery query6(db);
    query6.prepare("ALTER TABLE classbook ADD COLUMN updated datetime NOT NULL after created;");
    if(!query6.exec()){
        error = query6.lastError().text();
        Log::info(TAG, "Problem with install update " + error);
        return false;
    }

    QSqlQuery query7(db);
    query7.prepare("ALTER TABLE classbook ADD COLUMN ordered INT(11) NOT NULL after parentid;");
    if(!query7.exec()){
        error = query7.lastError().text();
        Log::info(TAG, "Problem with install update " + error);
        return false;
    }
    return true;

}
