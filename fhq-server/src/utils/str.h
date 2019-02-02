#ifndef STR_H
#define STR_H

#include <string>

class Str {

    public:
        static std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string& to_lower(std::string& str);
};

#endif // STR_H
