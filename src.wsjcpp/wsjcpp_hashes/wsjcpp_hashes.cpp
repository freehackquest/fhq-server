#include "wsjcpp_hashes.h"
#include <smallsha1.h>
#include <md5.h>
#include <iostream>
#include <cstring>

// ---------------------------------------------------------------------

std::string WsjcppHashes::sha1_calc_hex(const std::string &sSource) {
    char hexstring[41]; // 40 chars + a zero
    std::memset(hexstring, 0, sizeof hexstring);

    unsigned char hash[20];
    sha1::calc(sSource.c_str(), sSource.length(), hash);
    sha1::toHexString(hash, hexstring);
    return std::string(hexstring);
}

// ---------------------------------------------------------------------

std::string WsjcppHashes::md5_calc_hex(const std::string &sSource) {
    MD5 md5 = MD5(sSource);
    return md5.hexdigest();
}

// ---------------------------------------------------------------------