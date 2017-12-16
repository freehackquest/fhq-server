#ifndef UTILS_MERGE_TEXT_H
#define UTILS_MERGE_TEXT_H

#include <QString>
#include <vector>

struct row{
    row(int id1, int id2, QString key, QString line) :
        id1(id1), id2(id2), key(key), line(line) {}
    int id1;
    int id2;
    QString key;
    QString line;
};

/*struct rowCmp{
    bool operator()(const row& a, const row& b) const {
        return a.id1 < b.id1;
    }
};*/

class UtilsMergeText {
    public:
        static void compare(QString &txt1, QString &txt2, std::vector<row *> &arr);
        static void merge(QString &curtxt, QString &txt1, QString &txt2, std::vector<row *> &arr1, std::vector<row *> &arr2);
};



#endif // UTILS_MERGE_TEXT_H

