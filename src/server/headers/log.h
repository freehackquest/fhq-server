#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QStringList>
#include <QMutex>

class Log {
	public:
		static void info(QString tag, QString msg);
		static void err(QString tag, QString msg);
		static void warn(QString tag, QString msg);
				
	private:
		static void add(QString type, QString tag, QString msg);
};

static QMutex LOG_MUTEX;
static QStringList LAST_LOG_MESSAGES;

#endif // LOG_H
