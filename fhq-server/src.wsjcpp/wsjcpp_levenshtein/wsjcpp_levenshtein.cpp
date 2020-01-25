// Author: Igor Polyakov 

#include "wsjcpp_levenshtein.h"
#include <vector>

int WSJCppLevenshtein::distance(const std::string &s1, const std::string &s2)
{
    const std::size_t len1 = s1.length(), len2 = s2.length();
    std::vector<std::vector<unsigned int> > d(len1 + 1, std::vector<unsigned int>(len2 + 1));

    d[0][0] = 0;
    for (unsigned int i = 1; i <= len1; ++i) {
        d[i][0] = i;
    }
    for (unsigned int i = 1; i <= len2; ++i) {
        d[0][i] = i;
    }

    for (unsigned int i = 1; i <= len1; ++i) {
        for (unsigned int j = 1; j <= len2; ++j) {
            d[i][j] = std::min(std::min( d[i - 1][j] + 1, d[i][j - 1] + 1), (d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1)));
        }
    }
    return d[len1][len2];
}