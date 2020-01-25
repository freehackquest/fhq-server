// Author: Igor Polyakov

#ifndef WSJCPP_LEVENSHTEIN_H
#define WSJCPP_LEVENSHTEIN_H

#include <string>

class WSJCppLevenshtein {
    public:
        static int distance(const std::string &str1, const std::string &str2);
};

#endif // WSJCPP_LEVENSHTEIN_H