#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QStringList>
#include <QJsonArray>
#include <QAbstractSocket>
#include <stdio.h>
#include <iostream>
#include <mutex>

class Log {
	public:
        static void info(QString tag, QString msg);  // depricated
        static void info(const std::string &sTag, const std::string &sMessage);
        static void err(QString tag, QString msg); // depricated
        static void err(const std::string &sTag, const std::string &sMessage);
        static void err(QString tag, QAbstractSocket::SocketError socketError); // Move from here
        static void warn(QString tag, QString msg);  // depricated
        static void warn(const std::string &sTag, const std::string &sMessage);
        static void setdir(const std::string &sDirectoryPath);
        static QJsonArray last_logs();  // depricated

	private:
        static void add(const std::string &sType, const std::string &sTag, const std::string &sMessage);
};

static std::mutex g_LOG_MUTEX;
static QStringList g_LAST_LOG_MESSAGES;
static std::string g_LOG_DIR_PATH;

#endif // LOG_H
