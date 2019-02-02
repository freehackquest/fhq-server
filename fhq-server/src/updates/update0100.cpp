#include <update0100.h>

REGISTRY_UPDATE(Update0100)

Update0100::Update0100()
    : UpdateBase("u0099", "u0100", "Update table quests_writeups"){
}

bool Update0100::update(QSqlDatabase &db, std::string &error){

    {
        QSqlQuery query(db);
        query.prepare("ALTER TABLE quests_writeups ADD COLUMN approve INT DEFAULT 1;");
        if (!query.exec()) {
            error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with update (1) " + error);
            return false;
        }
    }

    {
        QSqlQuery query(db);
        query.prepare("ALTER TABLE quests_writeups ADD COLUMN userid INT DEFAULT 0;");
        if (!query.exec()) {
            error = query.lastError().text().toStdString();
            Log::err(TAG, "The problem with update (2) " + error);
            return false;
        }
    }
    return true;
}
