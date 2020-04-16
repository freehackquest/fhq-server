
#include "unit_test_storage_escapes.h"
#include <wsjcpp_storages.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestStorageEscapes)

UnitTestStorageEscapes::UnitTestStorageEscapes() 
    : WsjcppUnitTestBase("UnitTestStorageEscapes") {
    // 
}

// ---------------------------------------------------------------------

void UnitTestStorageEscapes::init() {
}

// ---------------------------------------------------------------------

bool UnitTestStorageEscapes::run() {
    if (!WsjcppStorages::support("mysql")) {
        WsjcppLog::err(TAG, "Not supported mysql");
        return false;
    }
    WsjcppStorage *pStorage = WsjcppStorages::create("mysql");
    
    struct LTest {
        LTest(std::string s1, std::string s2) : s1(s1), s2(s2) {}
        std::string s1;
        std::string s2;
    };


    std::vector<LTest *> tests;
    tests.push_back(new LTest("\'\"", "\\\'\\\""));
    tests.push_back(new LTest("test usually string 12 34", "test usually string 12 34"));
    tests.push_back(new LTest("test1 \r\n text2", "test1 \\r\\n text2"));
    tests.push_back(new LTest("test1 \0 text2", "test1 "));

    unsigned int nSuccess = 0;
    for (unsigned int i = 0; i < tests.size(); i++) {
        std::string s1 = tests[i]->s1;
        std::string s2 = "\"" + tests[i]->s2 + "\"";
        std::string s3 = pStorage->prepareStringValue(s1);
        if (s3 == s2) {
            nSuccess++;
        } else {
            WsjcppLog::err(TAG, "Expected [" + s2 + "], but got [" + s3 + "] for [" + s1 + "]");
        }
    }
    return nSuccess == tests.size();
}