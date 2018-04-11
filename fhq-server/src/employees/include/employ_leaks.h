#ifndef EMPLOY_LEAKS_H
#define EMPLOY_LEAKS_H

#include <employees.h>

class EmployLeaks : public EmployBase {
    public:
       EmployLeaks();
       static std::string name() { return "EmployLeaks"; }
       virtual bool init();


   private:
	
       std::vector<std::string> m_vCacheLeaks;
};

#endif // EMPLOY_LEAKS_H
