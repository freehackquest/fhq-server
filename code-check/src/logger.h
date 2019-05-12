#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <iostream>
#include <mutex>
#include <json.hpp>
#include <deque>
#include <ostream>

namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_YELLOW   = 93,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
};

class Log {
    public:
        static void info(const std::string &sTag, const std::string &sMessage);
        static void err(const std::string &sTag, const std::string &sMessage);
        static void throw_err(const std::string &sTag, const std::string &sMessage);
        static void warn(const std::string &sTag, const std::string &sMessage);
        static void ok(const std::string &sTag, const std::string &sMessage);
        static std::string currentTime();
        static std::string threadId();

    private:
        static void add(Color::Modifier &clr, const std::string &sType, const std::string &sTag, const std::string &sMessage);
};

#endif // LOGGER_H
