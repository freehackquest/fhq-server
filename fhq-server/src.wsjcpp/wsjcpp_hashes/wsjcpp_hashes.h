#ifndef WSJCPP_HASHES_H
#define WSJCPP_HASHES_H

#include <string>

class WsjcppHashes {
    public:
        static std::string sha1_calc_hex(const std::string &sSource);
        static std::string md5_calc_hex(const std::string &sSource);
};

#endif // WSJCPP_HASHES_H
