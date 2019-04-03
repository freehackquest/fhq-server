#ifndef FALLEN_H
#define FALLEN_H
#include <string>
#include <vector>

class Fallen {
    public:
        static long currentTime_milliseconds();
        static long currentTime_seconds();
        static std::string currentTime_forFilename();
        static std::string currentTime_logformat();
        static std::string formatTimeForWeb(long nTimeInSec);
        static std::string formatTimeForFilename(long nTimeInSec);
        static std::string formatTimeUTC(int nTimeInSec);
};

#endif // FALLEN_H