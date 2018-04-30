#ifndef DATABASE_0060_H
#define DATABASE_0060_H

#include <updates.h>

class Database0060 : public UpdateBase {
	
    public:
        Database0060();
        virtual bool update(QSqlDatabase &db, std::string &error);
};

REGISTRY_UPDATE(Database0060)

#endif // DATABASE_0060_H
