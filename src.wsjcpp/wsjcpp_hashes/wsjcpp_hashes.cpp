#include "wsjcpp_hashes.h"
#include <smallsha1.h>
#include <md5.h>
#include <iostream>
#include <cstring>
#include <fstream>

// ---------------------------------------------------------------------
// WsjcppHashes

std::string WsjcppHashes::sha1_calc_hex(const std::string &sSource) { // deprecated
    char hexstring[41]; // 40 chars + a zero
    std::memset(hexstring, 0, sizeof hexstring);

    unsigned char hash[20];
    sha1::calc(sSource.c_str(), sSource.length(), hash);
    sha1::toHexString(hash, hexstring);
    return std::string(hexstring);
}

std::string WsjcppHashes::md5_calc_hex(const std::string &sSource) { // deprecated
    MD5 md5 = MD5(sSource);
    return md5.hexdigest();
}

std::string WsjcppHashes::getMd5ByString(const std::string &sStr) {
    MD5 md5 = MD5(sStr);
    return md5.hexdigest();
}

std::string WsjcppHashes::getMd5ByFile(const std::string &sFilename) {
    std::ifstream f(sFilename, std::ifstream::binary);
    if (!f) {
        return "Could not open file";
    }

    // get length of file:
    f.seekg (0, f.end);
    int nBufferSize = f.tellg();
    f.seekg (0, f.beg);

    char *pBuffer = new char [nBufferSize];

    // read data as a block:
    f.read(pBuffer, nBufferSize);
    if (!f) {
        delete[] pBuffer;
        // f.close();
        return "Could not read file. Only " + std::to_string(f.gcount()) + " could be read";
    }
    f.close();

    MD5 md5;
    // deepcode ignore insecureHash: legacy support
    md5.update(pBuffer, nBufferSize);
    md5.finalize();
    return md5.hexdigest();
}

std::string WsjcppHashes::getSha1ByString(const std::string &sStr) {
    char hexstring[41]; // 40 chars + a zero
    std::memset(hexstring, 0, sizeof hexstring);

    unsigned char hash[20];
    sha1::calc(sStr.c_str(), sStr.length(), hash);
    sha1::toHexString(hash, hexstring);
    return std::string(hexstring);
}

std::string WsjcppHashes::getSha1ByFile(const std::string &sFilename) {
    std::ifstream f(sFilename, std::ifstream::binary);
    if (!f) {
        return "Could not open file";
    }

    // get length of file:
    f.seekg (0, f.end);
    int nBufferSize = f.tellg();
    f.seekg (0, f.beg);

    char *pBuffer = new char [nBufferSize];

    // read data as a block:
    f.read(pBuffer, nBufferSize);
    if (!f) {
        delete[] pBuffer;
        // f.close();
        return "Could not read file. Only " + std::to_string(f.gcount()) + " could be read";
    }
    f.close();

    char hexstring[41]; // 40 chars + a zero
    std::memset(hexstring, 0, sizeof hexstring);
    unsigned char hash[20];
    sha1::calc(pBuffer, nBufferSize, hash);
    sha1::toHexString(hash, hexstring);
    return std::string(hexstring);
}

