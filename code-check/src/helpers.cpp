#include "helpers.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <ctime>
#include <chrono>

// ---------------------------------------------------------------------

bool Helpers::fileExists(const std::string &sFilename) {
    struct stat st;
    bool bExists = (stat(sFilename.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) == 0;
    }
    return false;
}

// ---------------------------------------------------------------------

bool Helpers::dirExists(const std::string &sDirname) {
    struct stat st;
    bool bExists = (stat(sDirname.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) != 0;
    }
    return false;
}

// ---------------------------------------------------------------------

std::vector<std::string> Helpers::listOfDirs(const std::string &sDirname) {
    std::vector<std::string> vDirs;
    if (!Helpers::dirExists(sDirname)) {
        return vDirs;
    }
    DIR *dir = opendir(sDirname.c_str());
    struct dirent *entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_type == DT_DIR) {
            std::string sDir(entry->d_name);
            if (sDir != "." && sDir != "..") {
                vDirs.push_back(sDir);
            }
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return vDirs;
}

// ---------------------------------------------------------------------

std::vector<std::string> Helpers::listOfFiles(const std::string &sDirname) {
    std::vector<std::string> vFiles;
    if (!Helpers::dirExists(sDirname)) {
        return vFiles;
    }
    DIR *dir = opendir(sDirname.c_str());
    struct dirent *entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_type != DT_DIR) {
            std::string sDir(entry->d_name);
            if (sDir != "." && sDir != "..") {
                vFiles.push_back(sDir);
            }
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return vFiles;
}

// ---------------------------------------------------------------------

long Helpers::currentTime_milliseconds() {
    long nTimeStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return nTimeStart;
}

// ---------------------------------------------------------------------

long Helpers::currentTime_seconds() {
    long nTimeStart = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return nTimeStart;
}

// ---------------------------------------------------------------------

std::string Helpers::currentTime_logformat() {
    long nTimeStart = Helpers::currentTime_milliseconds();
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