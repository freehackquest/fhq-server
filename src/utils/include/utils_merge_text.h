#ifndef UTILS_MERGE_TEXT_H
#define UTILS_MERGE_TEXT_H

#include <QString>
#include <vector>

struct row{
    row(int id, QString key, QString line) :
        id(id), key(key), line(line) {}
    int id;
    QString key;
    QString line;
};

class UtilsMergeText {
    public:
        static void compare(QString &txt1, QString &txt2, std::vector<row *> &arr);
        static void merge(QString &curtxt, QString &txt1, QString &txt2, std::vector<row *> &arr1, std::vector<row *> &arr2);
};

#endif // UTILS_MERGE_TEXT_H

