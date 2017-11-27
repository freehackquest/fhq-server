#include <unit_test_merge_text.h>
#include <utils_merge_text.h>
#include <vector>
#include <iostream>

QString UnitTestMergeText::name(){
    return "UnitTestMergeText";
}

void UnitTestMergeText::init(){
}

bool UnitTestMergeText::run(){

    struct LTest {
        LTest(QString txt1, QString txt2) : txt1(txt1), txt2(txt2) {}
        QString txt1;
        QString txt2;
    };

    std::vector<LTest *> tests;
    tests.push_back(new LTest("test", "test"));
    std::vector<row> arr[tests.size()];

    unsigned int nSuccess = 0;
    for(unsigned int i = 0; i < tests.size(); i++){
        QString txt1 = tests[i]->txt1;
        QString txt2 = tests[i]->txt2;
        UtilsMergeText::compare(txt1, txt2, arr[i]);
        //TO DO logical condition

        //std::cout << str.toStdString();
    }
    return nSuccess;
}
