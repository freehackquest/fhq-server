#ifndef SHA1_WRAPPER_H
#define SHA1_WRAPPER_H
#include <string>

namespace sha1 {
    std::string calc_string_to_hex(const std::string &sSource);
} // namespace sha1

#endif // SHA1_WRAPPER_H