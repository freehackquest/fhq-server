#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QStringList>
#include <QMutex>
#include <QJsonArray>
#include <QAbstractSocket>

class Log {
	public:
		static void info(QString tag, QString msg);
		static void err(QString tag, QString msg);
		static void err(QString tag, QAbstractSocket::SocketError socketError);
		static void warn(QString tag, QString msg);
		static void setdir(QString dir);
        static QJsonArray last_logs();

	private:
		static void add(QString type, QString tag, QString msg);
};

static QMutex g_LOG_MUTEX;
static QStringList g_LAST_LOG_MESSAGES;
static QString g_LOG_DIR_PATH;

#endif // LOG_H
