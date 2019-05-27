#ifndef UTILS_STATIC_ANALIZING_TEXT_H
#define UTILS_STATIC_ANALIZING_TEXT_H

#include <string>
#include <map>

class UtilsStaticAnalizingText {
    public:
        static std::map<char, double> calc(const std::string &text);
    private:
        static double calcWeightOfOneChar(int strLen); 
};

#endif
