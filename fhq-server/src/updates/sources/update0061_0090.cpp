#include <update0061_0090.h>
#include <QSqlQuery>
#include <QUuid>

// ----------- UPDATE 0061 ---------------

Update0061::Update0061()
    : UpdateBase("u0060", "u0061", "Remove personal quests which not passed"){
}

bool Update0061::update(QSqlDatabase &db, std::string &error){
	QSqlQuery query(db);
	query.prepare("SELECT idquest FROM quest LEFT JOIN users_quests ON users_quests.questid = quest.idquest WHERE for_person <> 0 AND isnull( dt_passed )");
	if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with data selection " + error);
		return false;
	}

	while (query.next()) {
		QSqlRecord record = query.record();
		int questid = record.value("idquest").toInt();
		QSqlQuery query2(db);
		query2.prepare("DELETE FROM quest WHERE idquest = :questid");
		query2.bindValue(":questid", questid);
		if(!query2.exec()){
            error = query2.lastError().text().toStdString();
            Log::err(TAG, "The problem with deleting data " + error);
			return false;
		}
	}
	return true;
}

// ----------- UPDATE 0062 ---------------

Update0062::Update0062()
    : UpdateBase("u0061", "u0062", "Remove table user_old"){
}

bool Update0062::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("DROP TABLE IF EXISTS user_old");
        if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with removing the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0063 ---------------

Update0063::Update0063()
    : UpdateBase("u0062", "u0063", "Remove table userquest"){
}

bool Update0063::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("DROP TABLE userquest");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with removing the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0064 ---------------

Update0064::Update0064()
    : UpdateBase("u0063", "u0064", "Remove personal quests which passed"){
}

bool Update0064::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("SELECT idquest FROM quest LEFT JOIN users_quests ON users_quests.questid = quest.idquest WHERE for_person <> 0");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with data selection " + error);
        return false;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        int questid = record.value("idquest").toInt();
        QSqlQuery query2(db);
        query2.prepare("DELETE FROM quest WHERE idquest = :questid");
        query2.bindValue(":questid", questid);
        query2.exec();
        if(!query2.exec()){
            error = query2.lastError().text().toStdString();
            Log::err(TAG, "The problem with deleting data " + error);
            return false;
        }
    }
    return true;
}

// ----------- UPDATE 0065 ---------------

Update0065::Update0065()
    : UpdateBase("u0064", "u0065", "Update quests uuid"){
}

bool Update0065::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("SELECT idquest FROM quest WHERE isnull(quest_uuid)");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with data selection " + error);
        return false;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        int questid = record.value("idquest").toInt();
        QString questuuid = QUuid::createUuid().toString();
        questuuid = questuuid.mid(1,questuuid.length()-2);
        QSqlQuery query2(db);
        query2.prepare("UPDATE quest SET quest_uuid = :questuuid WHERE idquest = :questid AND isnull(quest_uuid)");
        query2.bindValue(":questuuid", questuuid);
        query2.bindValue(":questid", questid);
        if(!query2.exec()){
            error = query2.lastError().text().toStdString();
            Log::err(TAG, "The problem with updating the data " + error);
            return false;
        }
    }
    return true;
}

// ----------- UPDATE 0066 ---------------

Update0066::Update0066()
    : UpdateBase("u0065", "u0066", "Drop column quest.for_person"){
}

bool Update0066::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE quest DROP COLUMN for_person");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0067 ---------------

Update0067::Update0067()
    : UpdateBase("u0066", "u0067", "Added column copyright to quest"){
}

bool Update0067::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE quest ADD COLUMN copyright VARCHAR(255);");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0068 ---------------

Update0068::Update0068()
    : UpdateBase("u0067", "u0068", "Added table hints"){
}

bool Update0068::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare(
        "CREATE TABLE IF NOT EXISTS `quests_hints` ("
        "	  `id` int(11) NOT NULL AUTO_INCREMENT,"
        "	  `questid` int(11) NOT NULL,"
        "	  `text` varchar(4048) NOT NULL,"
        "	  `dt` datetime NOT NULL,"
        "	  PRIMARY KEY (`id`)"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0069 ---------------

Update0069::Update0069()
    : UpdateBase("u0068", "u0069", "Add columns to users"){
}

bool Update0069::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE `users` "
        "ADD COLUMN `country` VARCHAR(255) DEFAULT '',"
        "ADD COLUMN `region` VARCHAR(255) DEFAULT '',"
        "ADD COLUMN `city` VARCHAR(255) DEFAULT '',"
        "ADD COLUMN `latitude` DOUBLE  DEFAULT 0.0,"
        "ADD COLUMN `longitude` DOUBLE  DEFAULT 0.0"
        ";");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0070 ---------------

Update0070::Update0070()
    : UpdateBase("u0069", "u0070", "Removed table users_ips"){
}

bool Update0070::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("DROP TABLE `users_ips`;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with removing the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0071 ---------------

Update0071::Update0071()
    : UpdateBase("u0070", "u0071", "Added classbook table"){
}

bool Update0071::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare(
        "CREATE TABLE IF NOT EXISTS `classbook` ("
        "	  `id` int(11) NOT NULL AUTO_INCREMENT,"
        "	  `parentid` int(11) NOT NULL,"
        "	  `uuid` varchar(128) NOT NULL,"
        "	  `parentuuid` varchar(128) NOT NULL,"
        "	  `name_ru` varchar(128) NOT NULL,"
        "	  `name_en` varchar(128) NOT NULL,"
        "	  `dt` datetime NOT NULL,"
        "	  PRIMARY KEY (`id`)"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0072 ---------------

Update0072::Update0072()
    : UpdateBase("u0071", "u0072", "Added chatmessages table"){
}

bool Update0072::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare(
        "CREATE TABLE IF NOT EXISTS `chatmessages` ("
        "	  `id` int(11) NOT NULL AUTO_INCREMENT,"
        "	  `user` varchar(128) NOT NULL,"
        "	  `message` varchar(255) NOT NULL,"
        "	  `dt` datetime NOT NULL,"
        "	  PRIMARY KEY (`id`)"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0073 ---------------

Update0073::Update0073()
    : UpdateBase("u0072", "u0073", "Added captcha table"){
}

bool Update0073::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare(
        "CREATE TABLE IF NOT EXISTS `users_captcha` ("
        "	  `id` int(11) NOT NULL AUTO_INCREMENT,"
        "	  `captcha_uuid` varchar(127) NOT NULL,"
        "	  `captcha_val` varchar(127) NOT NULL,"
        "	  `dt_expired` datetime NOT NULL,"
        "	  PRIMARY KEY (`id`)"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0074 ---------------

Update0074::Update0074()
    : UpdateBase("u0073", "u0074", "Added feedback.from"){
}

bool Update0074::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE `feedback` ADD COLUMN `from` VARCHAR(255) DEFAULT '';");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0075 ---------------

Update0075::Update0075()
    : UpdateBase("u0074", "u0075", "Added table quests_writeups"){
}

bool Update0075::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare(
        "CREATE TABLE IF NOT EXISTS `quests_writeups` ("
        "	  `id` int(11) NOT NULL AUTO_INCREMENT,"
        "	  `questid` int(11) NOT NULL,"
        "	  `type` varchar(64) NOT NULL,"
        "	  `link` varchar(1024) NOT NULL,"
        "	  `text` varchar(4048) NOT NULL,"
        "	  `dt` datetime NOT NULL,"
        "	  PRIMARY KEY (`id`)"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0076 ---------------

Update0076::Update0076()
    : UpdateBase("u0075", "u0076", "Added users.rating"){
}

bool Update0076::update(QSqlDatabase &db, std::string &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE `users` ADD COLUMN `rating` INTEGER DEFAULT 0;");
    if(!query.exec()){
        error = query.lastError().text().toStdString();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query3(db);
    query3.prepare(
        "  SELECT SUM(q.score) as rating, uq.userid FROM users_quests uq "
        "   INNER JOIN quest q ON uq.questid = q.idquest"
        "  GROUP BY uq.userid"
    );
    if(!query3.exec()){
        error = query3.lastError().text().toStdString();
        Log::err(TAG, "The problem with data selection " + error);
        return false;
    }

    while (query3.next()) {
        QSqlRecord record = query3.record();
        int userid = record.value("userid").toInt();
        int rating = record.value("rating").toInt();

        QSqlQuery query2(db);
        query2.prepare("UPDATE users SET rating = :rating WHERE id = :userid");
        query2.bindValue(":rating", rating);
        query2.bindValue(":userid", userid);
        if(!query2.exec()){
            error = query2.lastError().text().toStdString();
            Log::err(TAG, "The problem with updating the data " + error);
            return false;
        }
    }
    return true;
}

// ----------- UPDATE 0077 ---------------

Update0077::Update0077()
    : UpdateBase("u0076", "u0077", "Removed from_version,name,result,userid from updates"){
}

bool Update0077::update(QSqlDatabase &db, std::string &error){
    {
        QSqlQuery query(db);
        query.prepare("ALTER TABLE `updates` DROP COLUMN from_version;");
        if(!query.exec()){
            error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with altering the table " + error);
            return false;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare("ALTER TABLE `updates` DROP COLUMN name;");
        if(!query.exec()){
            error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with altering the table " + error);
            return false;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare("ALTER TABLE `updates` DROP COLUMN result;");
        if(!query.exec()){
            error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with altering the table " + error);
            return false;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare("ALTER TABLE `updates` DROP COLUMN userid;");
        if(!query.exec()){
            error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with altering the table " + error);
            return false;
        }
    }
    return true;
}

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
