#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QStringList>
#include <QMutex>
#include <QJsonArray>
#include <QAbstractSocket>
#include <stdio.h>
#include <iostream>

class Log {
	public:
        static void info(QString tag, QString msg);  // depricated
        static void info(const std::string &sTag, const std::string &sMessage);
        static void err(QString tag, QString msg); // depricated
        static void err(const std::string &sTag, const std::string &sMessage);
        static void err(QString tag, QAbstractSocket::SocketError socketError); // Move from here
        static void warn(QString tag, QString msg);  // depricated
        static void warn(const std::string &sTag, const std::string &sMessage);
        static void setdir(QString dir);  // depricated
        static QJsonArray last_logs();  // depricated

	private:
		static void add(QString type, QString tag, QString msg);
};

static QMutex g_LOG_MUTEX;
static QStringList g_LAST_LOG_MESSAGES;
static QString g_LOG_DIR_PATH;

#endif // LOG_H
