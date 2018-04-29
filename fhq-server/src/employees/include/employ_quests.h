#ifndef EMPLOY_QUESTS_H
#define EMPLOY_QUESTS_H

#include <employees.h>

class EmployQuests : public EmployBase {
    public:
       EmployQuests();
       static std::string name() { return "EmployQuests"; }
       virtual bool init();

   private:
		std::string TAG;
		std::map<std::string, int> m_mapQuestsSubjects;

};

#endif // EMPLOY_QUESTS_H
