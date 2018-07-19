#include <sha1.h>
#include <sha1_wrapper.h>
#include <iostream>
#include <cstring>

namespace sha1 {
    std::string calc_string_to_hex(const std::string &sSource){
        char hexstring[41]; // 40 chars + a zero
        std::memset(hexstring, 0, sizeof hexstring);

        unsigned char hash[20];
        sha1::calc(sSource.c_str(), sSource.length(), hash);
        sha1::toHexString(hash, hexstring);
        return std::string(hexstring);
    }
}