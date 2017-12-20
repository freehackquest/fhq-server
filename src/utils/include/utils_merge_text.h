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
    bool operator < (const row& a) const {
        return (id < a.id);
    }
    bool operator == (const row& a) const {
        if ((id==a.id)&&(key==a.key)&&(line==a.line))
            return 1;
        else return 0;
    }
};

class UtilsMergeText {
    public:
        static void compare(QString &txt1, QString &txt2, std::vector<row *> &arr);
        static void merge(QString &curtxt, QString &txt1, QString &txt2, std::vector<row *> &arr1, std::vector<row *> &arr2);
};



#endif // UTILS_MERGE_TEXT_H

