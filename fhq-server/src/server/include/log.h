#ifndef SERVER_LOG_H
#define SERVER_LOG_H

#include <QString>
#include <QAbstractSocket>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <json.hpp>
#include <deque>


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
        static nlohmann::json last_logs();
        static void initGlobalVariables();
        static std::string currentTime();
        static std::string threadId();

	private:
        static void add(const std::string &sType, const std::string &sTag, const std::string &sMessage);
};

// TODO redesign to extern

extern std::mutex *g_LOG_MUTEX;
extern std::deque<std::string> *g_LAST_LOG_MESSAGES;

static std::string g_LOG_DIR_PATH;

#endif // SERVER_LOG_H
