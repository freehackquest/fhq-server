#include <unit_test_levenshtein.h>
#include <utils_levenshtein.h>
#include <vector>
#include <iostream>

QString UnitTestLevenshtein::name(){
	return "UnitTestLevenshtein";
}

void UnitTestLevenshtein::init(){
}

bool UnitTestLevenshtein::run(){

    struct LTest {
        LTest(QString s1, QString s2, int n) : s1(s1), s2(s2), n(n) {}
        QString s1;
        QString s2;
        int n;
    };

    std::vector<LTest *> tests;
    tests.push_back(new LTest("test", "test", 0));
    tests.push_back(new LTest("tttt", "aaaa", 4));
    tests.push_back(new LTest("ta", "t0", 1));
    tests.push_back(new LTest("taf", "t0", 2));
    tests.push_back(new LTest("111111111111111", "1111111112111111", 1));
    tests.push_back(new LTest("!@#$%^&*()_+", "!@#%$^&*()+", 3));

    unsigned int nSuccess = 0;
    for(unsigned int i = 0; i < tests.size(); i++){
        QString s1 = tests[i]->s1;
        QString s2 = tests[i]->s2;
        int n = tests[i]->n;
        int n1 = UtilsLevenshtein::distance(s1, s2);
        if (n1 != n) {
            std::cout << "Test levenshtein distance between " << s1.toStdString() << " and " << s2.toStdString() << "\n"
                      << " (expected " << n << ", but got " << n1 << ")";
        }else{
            nSuccess++;
        }
    }
    return nSuccess == tests.size();
}

