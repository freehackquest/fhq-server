#include <update0061_0090.h>
#include <QSqlQuery>
#include <QUuid>

// ----------- UPDATE 0061 ---------------

Update0061::Update0061(){
    TAG = "Update0061";
}

QString Update0061::from_version(){
	return "u0060";
}

QString Update0061::version(){
	return "u0061";
}

QString Update0061::description(){
	return "remove personal quests which not passed";
}

bool Update0061::update(QSqlDatabase &db, QString &error){
	QSqlQuery query(db);
	query.prepare("SELECT idquest FROM quest LEFT JOIN users_quests ON users_quests.questid = quest.idquest WHERE for_person <> 0 AND isnull( dt_passed )");
	if(!query.exec()){
		error = query.lastError().text();
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
			error = query2.lastError().text();
            Log::err(TAG, "The problem with deleting data " + error);
			return false;
		}
	}
	return true;
}

// ----------- UPDATE 0062 ---------------

Update0062::Update0062(){
    TAG = "Update0062";
}

QString Update0062::from_version(){
    return "u0061";
}

QString Update0062::version(){
    return "u0062";
}

QString Update0062::description(){
    return "Remove table user_old";
}

bool Update0062::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("DROP TABLE IF EXISTS user_old");
        if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with removing the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0063 ---------------

Update0063::Update0063(){
    TAG = "Update0063";
}

QString Update0063::from_version(){
    return "u0062";
}

QString Update0063::version(){
    return "u0063";
}

QString Update0063::description(){
    return "Remove table userquest";
}

bool Update0063::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("DROP TABLE userquest");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with removing the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0064 ---------------


Update0064::Update0064(){
    TAG = "Update0064";
}

QString Update0064::from_version(){
    return "u0063";
}

QString Update0064::version(){
    return "u0064";
}

QString Update0064::description(){
    return "Remove personal quests which passed";
}

bool Update0064::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("SELECT idquest FROM quest LEFT JOIN users_quests ON users_quests.questid = quest.idquest WHERE for_person <> 0");
    if(!query.exec()){
        error = query.lastError().text();
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
            error = query2.lastError().text();
            Log::err(TAG, "The problem with deleting data " + error);
            return false;
        }
    }
    return true;
}

// ----------- UPDATE 0065 ---------------

Update0065::Update0065(){
    TAG = "Update0065";
}

QString Update0065::from_version(){
    return "u0064";
}

QString Update0065::version(){
    return "u0065";
}

QString Update0065::description(){
    return "Update quests uuid";
}

bool Update0065::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("SELECT idquest FROM quest WHERE isnull(quest_uuid)");
    if(!query.exec()){
        error = query.lastError().text();
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
            error = query2.lastError().text();
            Log::err(TAG, "The problem with updating the data " + error);
            return false;
        }
    }
    return true;
}

// ----------- UPDATE 0066 ---------------

Update0066::Update0066(){
    TAG = "Update0066";
}

QString Update0066::from_version(){
    return "u0065";
}

QString Update0066::version(){
    return "u0066";
}

QString Update0066::description(){
    return "Drop column quest.for_person";
}

bool Update0066::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE quest DROP COLUMN for_person");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0067 ---------------

Update0067::Update0067(){
    TAG = "Update0067";
}

QString Update0067::from_version(){
    return "u0066";
}

QString Update0067::version(){
    return "u0067";
}

QString Update0067::description(){
    return "Added column copyright to quest";
}

bool Update0067::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE quest ADD COLUMN copyright VARCHAR(255);");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0068 ---------------


Update0068::Update0068(){
    TAG = "Update0068";
}

QString Update0068::from_version(){
    return "u0067";
}

QString Update0068::version(){
    return "u0068";
}

QString Update0068::description(){
    return "Added table hints";
}

bool Update0068::update(QSqlDatabase &db, QString &error){
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
        error = query.lastError().text();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0069 ---------------

Update0069::Update0069(){
    TAG = "Update0069";
}

QString Update0069::from_version(){
    return "u0068";
}

QString Update0069::version(){
    return "u0069";
}

QString Update0069::description(){
    return "Add columns to users";
}

bool Update0069::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE `users` "
        "ADD COLUMN `country` VARCHAR(255) DEFAULT '',"
        "ADD COLUMN `region` VARCHAR(255) DEFAULT '',"
        "ADD COLUMN `city` VARCHAR(255) DEFAULT '',"
        "ADD COLUMN `latitude` DOUBLE  DEFAULT 0.0,"
        "ADD COLUMN `longitude` DOUBLE  DEFAULT 0.0"
        ";");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0070 ---------------

Update0070::Update0070(){
    TAG = "Update0070";
}

QString Update0070::from_version(){
    return "u0069";
}

QString Update0070::version(){
    return "u0070";
}

QString Update0070::description(){
    return "Removed table users_ips";
}

bool Update0070::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("DROP TABLE `users_ips`;");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with removing the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0071 ---------------

Update0071::Update0071(){
    TAG = "Update0071";
}

QString Update0071::from_version(){
    return "u0070";
}

QString Update0071::version(){
    return "u0071";
}

QString Update0071::description(){
    return "Added classbook table";
}

bool Update0071::update(QSqlDatabase &db, QString &error){
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
        error = query.lastError().text();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0072 ---------------

Update0072::Update0072(){
    TAG = "Update0072";
}

QString Update0072::from_version(){
    return "u0071";
}

QString Update0072::version(){
    return "u0072";
}

QString Update0072::description(){
    return "Added chatmessages table";
}

bool Update0072::update(QSqlDatabase &db, QString &error){
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
        error = query.lastError().text();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0073 ---------------

Update0073::Update0073(){
    TAG = "Update0073";
}

QString Update0073::from_version(){
    return "u0072";
}

QString Update0073::version(){
    return "u0073";
}

QString Update0073::description(){
    return "Added captcha table";
}

bool Update0073::update(QSqlDatabase &db, QString &error){
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
        error = query.lastError().text();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0074 ---------------

Update0074::Update0074(){
    TAG = "Update0074";
}

QString Update0074::from_version(){
    return "u0073";
}

QString Update0074::version(){
    return "u0074";
}

QString Update0074::description(){
    return "Added feedback.from";
}

bool Update0074::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE `feedback` ADD COLUMN `from` VARCHAR(255) DEFAULT '';");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0075 ---------------

Update0075::Update0075(){
    TAG = "Update0075";
}

QString Update0075::from_version(){
    return "u0074";
}

QString Update0075::version(){
    return "u0075";
}

QString Update0075::description(){
    return "Added table quests_writeups";
}

bool Update0075::update(QSqlDatabase &db, QString &error){
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
        error = query.lastError().text();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0076 ---------------

Update0076::Update0076(){
    TAG = "Update0076";
}

QString Update0076::from_version(){
    return "u0075";
}

QString Update0076::version(){
    return "u0076";
}

QString Update0076::description(){
    return "Added users.score";
}

bool Update0076::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE `users` ADD COLUMN `rating` INTEGER DEFAULT 0;");
    if(!query.exec()){
        error = query.lastError().text();
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
        error = query3.lastError().text();
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
            error = query2.lastError().text();
            Log::err(TAG, "The problem with updating the data " + error);
            return false;
        }
    }
    return true;
}

// ----------- UPDATE 0077 ---------------

Update0077::Update0077(){
    TAG = "Update0077";
}

QString Update0077::from_version(){
    return "u0076";
}

QString Update0077::version(){
    return "u0077";
}

QString Update0077::description(){
    return "Removed from_version,name,result,userid from updates";
}

bool Update0077::update(QSqlDatabase &db, QString &error){
    {
        QSqlQuery query(db);
        query.prepare("ALTER TABLE `updates` DROP COLUMN from_version;");
        if(!query.exec()){
            error = query.lastError().text();
            Log::err(TAG, "The problem with altering the table " + error);
            return false;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare("ALTER TABLE `updates` DROP COLUMN name;");
        if(!query.exec()){
            error = query.lastError().text();
            Log::err(TAG, "The problem with altering the table " + error);
            return false;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare("ALTER TABLE `updates` DROP COLUMN result;");
        if(!query.exec()){
            error = query.lastError().text();
            Log::err(TAG, "The problem with altering the table " + error);
            return false;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare("ALTER TABLE `updates` DROP COLUMN userid;");
        if(!query.exec()){
            error = query.lastError().text();
            Log::err(TAG, "The problem with altering the table " + error);
            return false;
        }
    }
    return true;
}

// ----------- UPDATE 0078 ---------------

Update0078::Update0078(){
    TAG = "Update0078";
}

QString Update0078::from_version(){
    return "u0077";
}

QString Update0078::version(){
    return "u0078";
}

QString Update0078::description(){
    return "Removed quest.min_score";
}

bool Update0078::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE `quest` DROP COLUMN min_score;");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0079 ---------------

Update0079::Update0079(){
    TAG = "Update0079";
}

QString Update0079::from_version(){
    return "u0078";
}

QString Update0079::version(){
    return "u0079";
}

QString Update0079::description(){
    return "Rename table tryanswer to users_quests_answers";
}

bool Update0079::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("RENAME TABLE tryanswer TO users_quests_answers;");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with renaming a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0080 ---------------

Update0080::Update0080(){
    TAG = "Update0080";
}

QString Update0080::from_version(){
    return "u0079";
}

QString Update0080::version(){
    return "u0080";
}

QString Update0080::description(){
    return "Merge table tryanswer_backup to users_quests_answers and drop this";
}

bool Update0080::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("INSERT INTO users_quests_answers (iduser, idquest, answer_try, answer_real, passed, levenshtein, datetime_try)"
                "SELECT iduser, idquest, answer_try, answer_real, passed, levenshtein, datetime_try FROM tryanswer_backup;");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with data insertion " + error);
        return false;
    }

    QSqlQuery query2(db);
    query2.prepare("DROP TABLE IF EXISTS tryanswer_backup");
    if(!query2.exec()){
        error = query2.lastError().text();
        Log::err(TAG, "The problem with removing the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0081 ---------------

Update0081::Update0081(){
    TAG = "Update0081";
}

QString Update0081::from_version(){
    return "u0080";
}

QString Update0081::version(){
    return "u0081";
}

QString Update0081::description(){
    return "Rename columns in users_quests_answers. "
        "iduser => userid, "
        "idquest => questid, "
        "datetime_try => dt, "
        "answer_try => user_answer, "
        "answer_real => quest_answer";
}

bool Update0081::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `iduser` `userid` INTEGER NOT NULL;");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query1(db);
    query1.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `idquest` `questid` INTEGER NOT NULL;");
    if(!query1.exec()){
        error = query1.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query2(db);
    query2.prepare("ALTER TABLE users_quests_answers ADD COLUMN `user_answer` VARCHAR(255) NOT NULL;");
    if(!query2.exec()){
        error = query2.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }

    QSqlQuery query3(db);
    query3.prepare("UPDATE users_quests_answers SET user_answer = LEFT(answer_try, 255);");
    if(!query3.exec()){
        error = query3.lastError().text();
        Log::err(TAG, "The problem with updating the data " + error);
        return false;
    }

    QSqlQuery query4(db);
    query4.prepare("ALTER TABLE users_quests_answers DROP COLUMN `answer_try`");
    if(!query4.exec()){
        error = query4.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }


    QSqlQuery query5(db);
    query5.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `answer_real` `quest_answer` VARCHAR(255) NOT NULL;");
    if(!query5.exec()){
        error = query5.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }


    QSqlQuery query6(db);
    query6.prepare("ALTER TABLE users_quests_answers CHANGE COLUMN `datetime_try` `dt` DATETIME NOT NULL;");
    if(!query6.exec()){
        error = query6.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0082 ---------------

Update0082::Update0082(){
    TAG = "Update0082";
}

QString Update0082::from_version(){
    return "u0081";
}

QString Update0082::version(){
    return "u0082";
}

QString Update0082::description(){
    return "New column answer_format in table quest ";
}

bool Update0082::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE quest ADD COLUMN `answer_format` VARCHAR(255) DEFAULT '';");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0083 ---------------

Update0083::Update0083(){
    TAG = "Update0083";
}

QString Update0083::from_version(){
    return "u0082";
}

QString Update0083::version(){
    return "u0083";
}

QString Update0083::description(){
    return "Create table users_offers";
}

bool Update0083::update(QSqlDatabase &db, QString &error){
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
        error = query.lastError().text();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0084 ---------------

Update0084::Update0084(){
    TAG = "Update0084";
}

QString Update0084::from_version(){
    return "u0083";
}

QString Update0084::version(){
    return "u0084";
}

QString Update0084::description(){
    return "Add table settings";
}

bool Update0084::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare(
        "CREATE TABLE IF NOT EXISTS `settings` ("
        "	  `id` int(11) NOT NULL AUTO_INCREMENT,"
        "	  `name` varchar(128) NOT NULL,"
        "	  `value` varchar(255) NOT NULL,"
        "	  PRIMARY KEY (`id`)"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with creating a table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0085 ---------------

Update0085::Update0085(){
    TAG = "Update0085";
}

QString Update0085::from_version(){
    return "u0084";
}

QString Update0085::version(){
    return "u0085";
}

QString Update0085::description(){
    return "Renamed column quest_uuid to uuid";
}

bool Update0085::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE quest CHANGE COLUMN `quest_uuid` `uuid` varchar(255) NOT NULL;");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0086 ---------------

Update0086::Update0086(){
    TAG = "Update0086";
}

QString Update0086::from_version(){
    return "u0085";
}

QString Update0086::version(){
    return "u0086";
}

QString Update0086::description(){
    return "Removed column idauthor from table quest";
}

bool Update0086::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE quest DROP COLUMN `idauthor`");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0087 ---------------

Update0087::Update0087(){
    TAG = "Update0087";
}

QString Update0087::from_version(){
    return "u0086";
}

QString Update0087::version(){
    return "u0087";
}

QString Update0087::description(){
    return "Added column about to table users";
}

bool Update0087::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE users ADD COLUMN `about` TEXT NOT NULL");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0088 ---------------

Update0088::Update0088(){
    TAG = "Update0088";
}

QString Update0088::from_version(){
    return "u0087";
}

QString Update0088::version(){
    return "u0088";
}

QString Update0088::description(){
    return "Added column group to table settings";
}

bool Update0088::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE settings ADD COLUMN `group` VARCHAR(255) DEFAULT ''");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0089 ---------------

Update0089::Update0089(){
    TAG = "Update0089";
}

QString Update0089::from_version(){
    return "u0088";
}

QString Update0089::version(){
    return "u0089";
}

QString Update0089::description(){
    return "Added column type to table settings";
}

bool Update0089::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE settings ADD COLUMN `type` VARCHAR(255) DEFAULT 'string'");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}

// ----------- UPDATE 0090 ---------------

Update0090::Update0090(){
    TAG = "Update0090";
}

QString Update0090::from_version(){
    return "u0089";
}

QString Update0090::version(){
    return "u0090";
}

QString Update0090::description(){
    return "Added column type to table settings";
}

bool Update0090::update(QSqlDatabase &db, QString &error){
    QSqlQuery query(db);
    query.prepare("ALTER TABLE users ADD COLUMN `university` VARCHAR(255) DEFAULT ''");
    if(!query.exec()){
        error = query.lastError().text();
        Log::err(TAG, "The problem with altering the table " + error);
        return false;
    }
    return true;
}
