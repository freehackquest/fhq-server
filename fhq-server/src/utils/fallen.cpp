#include "fallen.h"
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <ctime>
#include <math.h>
#include <chrono>

long Fallen::currentTime_milliseconds() {
    long nTimeStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return nTimeStart;
}

// ---------------------------------------------------------------------

long Fallen::currentTime_seconds() {
    long nTimeStart = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return nTimeStart;
}

// ---------------------------------------------------------------------

std::string Fallen::currentTime_logformat(){
    long nTimeStart = Fallen::currentTime_milliseconds();
    std::string sMilliseconds = std::to_string(int(nTimeStart % 1000));
    nTimeStart = nTimeStart / 1000;

    std::time_t tm_ = long(nTimeStart);
    // struct tm tstruct = *localtime(&tm_);
    struct tm tstruct = *gmtime ( &tm_ );

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return std::string(buf) + "." + std::string( 3 - sMilliseconds.length(), '0').append(sMilliseconds);
}

// ---------------------------------------------------------------------

std::string Fallen::formatTimeForWeb(long nTimeInSec) {
    std::time_t tm_ = long(nTimeInSec);
    // struct tm tstruct = *localtime(&tm_);
    struct tm tstruct = *gmtime ( &tm_ );

    
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    char buf[80];
    // Like: Wed, 22 Jul 2009 19:15:56 GMT
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &tstruct);
    return std::string(buf);
}

// ---------------------------------------------------------------------

std::string Fallen::currentTime_forFilename() {
    long nTimeStart = Fallen::currentTime_seconds();
    return Fallen::formatTimeForFilename(nTimeStart);
}

// ---------------------------------------------------------------------

std::string Fallen::formatTimeForFilename(long nTimeInSec) {
    std::time_t tm_ = long(nTimeInSec);
    // struct tm tstruct = *localtime(&tm_);
    struct tm tstruct = *gmtime ( &tm_ );

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    char buf[80];
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tstruct);
    return std::string(buf);
}

// ---------------------------------------------------------------------

std::string Fallen::formatTimeUTC(int nTimeInSec) {
    // datetime
    std::time_t tm_ = long(nTimeInSec);
    // struct tm tstruct = *localtime(&tm_);
    struct tm tstruct = *gmtime ( &tm_ );

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return std::string(buf);
}

// ---------------------------------------------------------------------
