#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QStringList>
#include <QMutex>
#include <QAbstractSocket>

class Log {
	public:
		static void info(QString tag, QString msg);
		static void err(QString tag, QString msg);
		static void err(QString tag, QAbstractSocket::SocketError socketError);
		static void warn(QString tag, QString msg);
		static void setdir(QString dir);
				
	private:
		static void add(QString type, QString tag, QString msg);
};

static QMutex LOG_MUTEX;
static QStringList LAST_LOG_MESSAGES;
static QString LOG_DIR_PATH;

#endif // LOG_H
