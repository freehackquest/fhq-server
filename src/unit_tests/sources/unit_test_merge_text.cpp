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
        LTest(QString txt1, QString txt2, QString txt3, std::vector<row *> arr1, std::vector<row *> arr2, unsigned int n) :
            txt1(txt1), txt2(txt2), txt3(txt3), arr1(arr1), arr2(arr2), n(n) {}
        QString txt1;
        QString txt2;
        QString txt3;
        std::vector<row *> arr1;
        std::vector<row *> arr2;
        unsigned int n;
    };

    std::vector<LTest *> tests;
    std::vector<row *> arr1, arr2;
    tests.push_back(new LTest("You/are/got/and/good", "I/got/islam/and/this is/good/boy", "I/cure/damn/ill/cancern", arr1, arr2, 8));

    unsigned int nSuccess = 0;
    for(unsigned int i = 0; i < tests.size(); i++){
        QString txt1 = tests[i]->txt1;
        QString txt2 = tests[i]->txt2;
        QString txt3 = tests[i]->txt3;
        UtilsMergeText::merge(txt1, txt2, txt3, arr1, arr2);
        unsigned int n = tests[i]->n;

        //TO DO output
        /*QString s = arr1.at(0)->line;
        std::cout << s.toStdString();*/

        if(arr1.size()==n) nSuccess++;
        arr1.clear(), arr2.clear();
    }
    return nSuccess == tests.size();
}
