#ifndef WSJCPP_HASHES_H
#define WSJCPP_HASHES_H

#include <string>

class WsjcppHashes {
    public:
        static std::string sha1_calc_hex(const std::string &sSource); // deprecated
        static std::string md5_calc_hex(const std::string &sSource); // deprecated

        static std::string getMd5ByString(const std::string &sStr);
        static std::string getMd5ByFile(const std::string &sFilename);

        static std::string getSha1ByString(const std::string &sStr);
        static std::string getSha1ByFile(const std::string &sFilename);

};

#endif // WSJCPP_HASHES_H
