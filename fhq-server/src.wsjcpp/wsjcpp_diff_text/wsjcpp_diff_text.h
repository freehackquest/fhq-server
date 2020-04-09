#ifndef WSJCPP_DIFF_TEXT_H
#define WSJCPP_DIFF_TEXT_H


#include <vector>
#include <string>

class WsjcppDiffTextRow {
    public:
        WsjcppDiffTextRow(int id, std::string key, std::string line);
        int id;
        std::string key;
        std::string line;
};

class WsjcppDiffText {
    public:
        static void compare(std::string &txt1, std::string &txt2, std::vector<WsjcppDiffTextRow *> &arr);
        static void merge(
            std::string &curtxt,
            std::string &txt1,
            std::string &txt2,
            std::vector<WsjcppDiffTextRow *> &arr1,
            std::vector<WsjcppDiffTextRow *> &arr2
        );
};

#endif // WSJCPP_DIFF_TEXT_H