#ifndef UTILS_MERGE_TEXT_H
#define UTILS_MERGE_TEXT_H

#include <QString>
#include <vector>
#include <set>

struct row{
    row(int id, QString key, QString line) :
        id(id), key(key), line(line) {}
    int id;
    QString key;
    QString line;
/*    bool operator = (const row* &a) const {
        return (id = a.id);
    }*/
/*    bool operator () (const row& a, const row& b) const {
        return a.id < b.id;
    }*/
    bool operator == (const row& a) const {
        if ((id==a.id)&&(key==a.key)&&(line==a.line))
            return 1;
        else return 0;
    }
};

/*std::vector<row> m(std::vector<row *> &first, std::vector<row *> &last){
    std::vector<row> v3;

    std::set <row> s1(first.begin(), first.end());
    std::set <row> s2(last.begin(), last.end());

    set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(v3));
    set_difference(s2.begin(), s2.end(), s1.begin(), s1.end(), back_inserter(v3));

    return v3;
}*/

class UtilsMergeText {
    public:
        static void compare(QString &txt1, QString &txt2, std::vector<row *> &arr);
        static void merge(QString &curtxt, QString &txt1, QString &txt2, std::vector<row *> &arr1, std::vector<row *> &arr2);
};



#endif // UTILS_MERGE_TEXT_H

