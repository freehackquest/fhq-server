#ifndef ADD_PUBLIC_EVENTS_TASK_H
#define ADD_PUBLIC_EVENTS_TASK_H

#include <QRunnable>
#include <iserver.h>

class AddPublicEventsTask : public QRunnable {
	
	void run();
	public:
        AddPublicEventsTask(QString type, QString message);
		~AddPublicEventsTask();
	private:
		QString m_sMessage;
		QString m_sType;
		QString TAG;
};

#endif // ADD_PUBLIC_EVENTS_TASK_H
