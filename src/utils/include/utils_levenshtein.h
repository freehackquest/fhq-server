#ifndef UTILS_LEVENSHTEIN_H
#define UTILS_LEVENSHTEIN_H

#include <QString>

class UtilsLevenshtein {
public:
    static int distance(const QString &str1, const QString &str2);
  private:
    static int find_min(int a,int b, int c);
};

#endif
