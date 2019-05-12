#include <logger.h>
#include <iostream>
#include <thread>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <sstream>
#include <ostream>
#include <helpers.h>

// ---------------------------------------------------------------------

void Log::info(const std::string & sTag, const std::string &sMessage) {
    Color::Modifier def(Color::FG_DEFAULT);
    Log::add(def, "INFO",sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::err(const std::string & sTag, const std::string &sMessage) {
    Color::Modifier red(Color::FG_RED);
    Log::add(red, "ERR", sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::throw_err(const std::string &sTag, const std::string &sMessage) {
    Color::Modifier red(Color::FG_RED);
    Log::add(red, "ERR", sTag, sMessage);
    throw std::runtime_error(sMessage);
}

// ---------------------------------------------------------------------

void Log::warn(const std::string & sTag, const std::string &sMessage) {
    Color::Modifier yellow(Color::FG_YELLOW);
    Log::add(yellow, "WARN",sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::ok(const std::string &sTag, const std::string &sMessage) {
    Color::Modifier green(Color::FG_GREEN);
    Log::add(green, "OK", sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::add(Color::Modifier &clr, const std::string &sType, const std::string &sTag, const std::string &sMessage) {
    Color::Modifier def(Color::FG_DEFAULT);
    std::string sLogMessage = Helpers::currentTime_logformat() + ", " + " [" + sType + "] " + sTag + ": " + sMessage;
    std::cout << clr << sLogMessage << def << std::endl;
}

// ---------------------------------------------------------------------
