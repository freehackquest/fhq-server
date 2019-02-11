#ifndef UTILS_MERGE_TEXT_H
#define UTILS_MERGE_TEXT_H

#include <vector>
#include <string>

struct UtilsMergeTextRow {
    UtilsMergeTextRow(int id, std::string key, std::string line) :
        id(id), key(key), line(line) {}
    int id;
    std::string key;
    std::string line;
};

class UtilsMergeText {
    public:
        static void compare(std::string &txt1, std::string &txt2, std::vector<UtilsMergeTextRow *> &arr);
        static void merge(
            std::string &curtxt,
            std::string &txt1,
            std::string &txt2,
            std::vector<UtilsMergeTextRow *> &arr1,
            std::vector<UtilsMergeTextRow *> &arr2
        );
};

#endif // UTILS_MERGE_TEXT_H

