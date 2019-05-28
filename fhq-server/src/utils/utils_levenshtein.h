#ifndef UTILS_LEVENSHTEIN_H
#define UTILS_LEVENSHTEIN_H

#include <string>

class UtilsLevenshtein {
    public:
        static int distance(const std::string &str1, const std::string &str2);
};

#endif
