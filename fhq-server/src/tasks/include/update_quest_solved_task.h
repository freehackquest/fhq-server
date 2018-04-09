#ifndef UPDATE_QUEST_SOLVED_TASK_H
#define UPDATE_QUEST_SOLVED_TASK_H

#include <QRunnable>
#include <iserver.h>

class UpdateQuestSolvedTask : public QRunnable {
	
	void run();
	public:
        UpdateQuestSolvedTask(int m_nQuestID);
        ~UpdateQuestSolvedTask();
	private:
        int m_nQuestID;
		QString TAG;
};

#endif // UPDATE_QUEST_SOLVED_TASK_H
