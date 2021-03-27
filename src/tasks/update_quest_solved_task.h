#ifndef UPDATE_QUEST_SOLVED_TASK_H
#define UPDATE_QUEST_SOLVED_TASK_H

#include <QRunnable>
#include <string>

class UpdateQuestSolvedTask : public QRunnable {
    void run();
    
    public:
        UpdateQuestSolvedTask(int m_nQuestID);
        ~UpdateQuestSolvedTask();

    private:
        int m_nQuestID;
        std::string TAG;
};

#endif // UPDATE_QUEST_SOLVED_TASK_H
