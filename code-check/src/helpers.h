#ifndef HELPERS_H
#define HELPERS_H
#include <string>
#include <vector>

class Helpers {
    public:
        static bool dirExists(const std::string &sFilename);
        static bool fileExists(const std::string &sFilename);
        static std::vector<std::string> listOfDirs(const std::string &sDirname);
        static std::vector<std::string> listOfFiles(const std::string &sDirname);    

        static long currentTime_milliseconds();
        static long currentTime_seconds();
        static std::string currentTime_logformat();
};

#endif // HELPERS_H