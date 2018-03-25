#ifndef LOG_H
#define LOG_H

#include <QAbstractSocket>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <vector>
#include <json.hpp>

class Log {
	public:
        static void info(const std::string &sTag, const std::string & sMessage);
        static void err(const std::string &sTag, const std::string &sMessage);
        static void err(const std::string &sTag, QAbstractSocket::SocketError socketError); // TODO move to server
        static void warn(const std::string &sTag, const std::string &sMessage);
        static void setdir(const std::string &sDir);
        static nlohmann::json last_logs();

	private:
        static void add(const std::string &type, const std::string &tag, const std::string &msg);
};

static std::mutex g_LOG_MUTEX;
static std::vector<std::string> g_LAST_LOG_MESSAGES;
static std::string g_LOG_DIR_PATH;

#endif // LOG_H
