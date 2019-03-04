#include <update0061_0090.h>
#include <QSqlQuery>
#include <QUuid>

// ----------- UPDATE 0078 ---------------

Update0078::Update0078()
    : UpdateBase("u0077", "u0078", "Removed quest.min_score"){
}

bool Update0078::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE `quest` DROP COLUMN min_score;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0079 ---------------

Update0079::Update0079()
    : UpdateBase("u0078", "u0079", "Rename table tryanswer to users_quests_answers"){
}

bool Update0079::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("RENAME TABLE tryanswer TO users_quests_answers;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with renaming a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0080 ---------------

Update0080::Update0080()
    : UpdateBase("u0079", "u0080", "Merge table tryanswer_backup to users_quests_answers and drop this"){
}

bool Update0080::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("INSERT INTO users_quests_answers (iduser, idquest, answer_try, answer_real, passed, levenshtein, datetime_try)"
                "SELECT iduser, idquest, answer_try, answer_real, passed, levenshtein, datetime_try FROM tryanswer_backup;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with data insertion " + error);
        return false;
    }

    QSqlQuery query2(db);
    query2.prepare("DROP TABLE IF EXISTS tryanswer_backup");
    if(!query2.exec()){
        error = query2.lastError().text().toStdString();
        Log::err(TAG, "The problem with removing the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0081 ---------------

Update0081::Update0081()
    : UpdateBase("u0080", "u0081", "Rename columns in users_quests_answers. "
        "iduser => userid, "
        "idquest => questid, "
        "datetime_try => dt, "
        "answer_try => user_answer, "
        "answer_real => quest_answer"){
}

bool Update0081::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `iduser` `userid` INTEGER NOT NULL;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query1(db);
    query1.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `idquest` `questid` INTEGER NOT NULL;");
    if(!query1.exec()){
        error = query1.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query2(db);
    query2.prepare("ALTER TABLE users_quests_answers ADD COLUMN `user_answer` VARCHAR(255) NOT NULL;");
    if(!query2.exec()){
        error = query2.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query3(db);
    query3.prepare("UPDATE users_quests_answers SET user_answer = LEFT(answer_try, 255);");
    if(!query3.exec()){
        error = query3.lastError().text().toStdString();
        Log::err(TAG, "The problem with updating the data " + error);
        return false;
    }

    QSqlQuery query4(db);
    query4.prepare("ALTER TABLE users_quests_answers DROP COLUMN `answer_try`");
    if(!query4.exec()){
        error = query4.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }


    QSqlQuery query5(db);
    query5.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `answer_real` `quest_answer` VARCHAR(255) NOT NULL;");
    if(!query5.exec()){
        error = query5.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }


    QSqlQuery query6(db);
    query6.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `datetime_try` `dt` DATETIME NOT NULL;");
    if(!query6.exec()){
        error = query6.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0082 ---------------

Update0082::Update0082()
    : UpdateBase("u0081", "u0082", "New column answer_format in table quest "){
}

bool Update0082::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE quest ADD COLUMN `answer_format` VARCHAR(255) DEFAULT '';");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0083 ---------------

Update0083::Update0083()
    : UpdateBase("u0082", "u0083", "Create table users_offers"){
}

bool Update0083::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare(
        "CREATE TABLE IF NOT EXISTS `users_offers` ("
        "	  `id` int(11) NOT NULL AUTO_INCREMENT,"
        "	  `userid` int(11) NOT NULL,"
        "	  `data` text NOT NULL,"
        "	  `type` varchar(255) NOT NULL,"
        "	  `scomment` varchar(255) NOT NULL,"
        "	  `status` varchar(255) NOT NULL,"
        "	  `created` datetime NOT NULL,"
        "	  `updated` datetime NOT NULL,"
        "	  PRIMARY KEY (`id`)"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0084 ---------------

Update0084::Update0084()
    : UpdateBase("u0083", "u0084", "Add table settings"){
}

bool Update0084::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare(
        "CREATE TABLE IF NOT EXISTS `settings` ("
        "	  `id` int(11) NOT NULL AUTO_INCREMENT,"
        "	  `name` varchar(128) NOT NULL,"
        "	  `value` varchar(255) NOT NULL,"
        "	  PRIMARY KEY (`id`)"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0085 ---------------

Update0085::Update0085()
    : UpdateBase("u0084", "u0085", "Renamed column quest_uuid to uuid"){
}

bool Update0085::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE quest CHANGE COLUMN `quest_uuid` `uuid` varchar(255) NOT NULL;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0086 ---------------

Update0086::Update0086()
    : UpdateBase("u0085", "u0086", "Removed column idauthor from table quest"){
}

bool Update0086::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE quest DROP COLUMN `idauthor`");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0087 ---------------

Update0087::Update0087()
    : UpdateBase("u0086", "u0087", "Added column about to table users"){
}

bool Update0087::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE users ADD COLUMN `about` TEXT NOT NULL");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0088 ---------------

Update0088::Update0088()
    : UpdateBase("u0087", "u0088", "Added column group to table settings"){
}

bool Update0088::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE settings ADD COLUMN `group` VARCHAR(255) DEFAULT ''");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0089 ---------------

Update0089::Update0089()
    : UpdateBase("u0088", "u0089", "Added column type to table settings"){
}

bool Update0089::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE settings ADD COLUMN `type` VARCHAR(255) DEFAULT 'string'");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0090 ---------------

Update0090::Update0090()
    : UpdateBase("u0089", "u0090", "Added column type to table settings"){
}

bool Update0090::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE users ADD COLUMN `university` VARCHAR(255) DEFAULT ''");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}
