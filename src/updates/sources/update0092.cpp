#include <update0092.h>

Update0092::Update0092(){
    TAG = "Update0092";
}

QString Update0092::from_version(){
    return "u0091";
}

QString Update0092::version(){
    return "u0092";
}

QString Update0092::description(){
    return "Created tables classbook_proposal, classbook_localization";
}

bool Update0092::update(QSqlDatabase &db, QString &error){

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
            error = query.lastError().text();
            Log::err(TAG, "Problem with creating a table " + error);
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
            error = query.lastError().text();
            Log::err(TAG, "Problem with creating a table " + error);
            return false;
        }
    }
    return true;
}
