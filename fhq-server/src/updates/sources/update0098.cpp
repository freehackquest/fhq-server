#include <update0098.h>

Update0098::Update0098()
    : UpdateBase("u0097", "u0098", "Add new tables for leaks"){
}

bool Update0098::update(QSqlDatabase &db, std::string &error){

    {
        // Table structure for table `leaks`
        QSqlQuery query(db);
        query.prepare(""
            "CREATE TABLE IF NOT EXISTS `leaks` ("
            "  `id` int(11) NOT NULL AUTO_INCREMENT,"
            "  `uuid` varchar(255) NOT NULL,"
            "  `gameid` int(11) NOT NULL,"
            "  `name` varchar(255) NOT NULL,"
            "  `content` text(4096) NOT NULL,"
            "  `score` int(11) NOT NULL,"
            "  `created` datetime NOT NULL,"
            "  `updated` datetime NOT NULL,"
            "  `sold` int(10) NOT NULL,"
            "  PRIMARY KEY (`id`)"
            ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
        if(!query.exec()){
            error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
            return false;
        }
    }

    {
        // Table structure for table `users_leaks`
        QSqlQuery query(db);
        query.prepare(""
            "CREATE TABLE IF NOT EXISTS `users_leaks` ("
            "  `id` int(11) NOT NULL AUTO_INCREMENT,"
            "  `leakid` int(11) NOT NULL,"
            "  `userid` int(11) NOT NULL,"
            "  `grade` int(10) NOT NULL,"
            "  `dt` datetime NOT NULL,"
            "  PRIMARY KEY (`id`)"
            ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
        if(!query.exec()){
            error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
            return false;
        }
    }

    {
        // Table structure for table `leaks_files`
        QSqlQuery query(db);
        query.prepare(""
            "CREATE TABLE IF NOT EXISTS `leaks_files` ("
            "  `id` int(11) NOT NULL AUTO_INCREMENT,"
            "  `uuid` varchar(255) NOT NULL,"
            "  `leakid` int(11) NOT NULL,"
            "  `filename_orig` varchar(255) NOT NULL,"
            "  `md5` varchar(255) NOT NULL,"
            "  `size` int(11) NOT NULL,"
            "  PRIMARY KEY (`id`)"
            ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
        if(!query.exec()){
            error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
            return false;
        }
    }
    return true;
}
