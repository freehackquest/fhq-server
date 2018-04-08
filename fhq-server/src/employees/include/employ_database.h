#ifndef EMPLOY_DATABASE_H
#define EMPLOY_DATABASE_H

#include <employees.h>

class EmployDatabase : public EmployBase {
    public:
       EmployDatabase();
       static std::string name() { return "EmployDatabase"; }
       virtual bool init();
};

#endif // EMPLOY_DATABASE_H
