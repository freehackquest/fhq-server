#include <update0092.h>

Update0092::Update0092()
    : UpdateBase("u0091", "u0092", "Created tables classbook_proposal, classbook_localization"){
}

bool Update0092::update(QSqlDatabase &db, std::string &error){

    {
        QSqlQuery query(db);
        query.prepare(""
            "CREATE TABLE IF NOT EXISTS `classbook_proposal` ("
            "  `id` int(11) NOT NULL AUTO_INCREMENT,"
            "  `classbookid` int(11) NOT NULL,"
            "  `lang` varchar(8) NOT NULL,"
            "  `name` varchar(128) NOT NULL,"
            "  `content` text NOT NULL,"
            "  `created` datetime NOT NULL,"
            "  PRIMARY KEY (`id`)"
            ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;");
        if(!query.exec()){
            error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with creating a table " + error);
            return false;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare(""
            "CREATE TABLE IF NOT EXISTS `classbook_localization` ("
            "  `id` int(11) NOT NULL AUTO_INCREMENT,"
            "  `classbookid` int(11) NOT NULL,"
            "  `lang` varchar(8) NOT NULL,"
            "  `name` varchar(128) NOT NULL,"
            "  `content` text NOT NULL,"
            "  `created` datetime NOT NULL,"
            "  `updated` datetime NOT NULL,"
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
