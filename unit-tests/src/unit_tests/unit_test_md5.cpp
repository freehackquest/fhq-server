#include "unit_test_md5.h"
#include <vector>
#include <md5.h>
#include <str.h>

REGISTRY_UNIT_TEST(UnitTestM5)


UnitTestM5::UnitTestM5()
    : UnitTestBase("UnitTestM5") {
    //
}

void UnitTestM5::init() {
    // nothing
}

bool UnitTestM5::run() {

    struct LTest {
        LTest(std::string s1, std::string s2) : s1(s1), s2(s2) {}
        std::string s1;
        std::string s2;
    };

    std::vector<LTest *> tests;
    tests.push_back(new LTest("test", "098F6BCD4621D373CADE4E832627B4F6"));
    tests.push_back(new LTest("admin", "21232F297A57A5A743894A0E4A801FC3"));

    unsigned int nSuccess = 0;
    for (unsigned int i = 0; i < tests.size(); i++) {
        std::string s1 = tests[i]->s1;
        std::string s2 = tests[i]->s2;
        std::string s3 = md5(s1);
        Str::to_lower(s2);
        Str::to_lower(s3);
        if (s3 == s2) {
            nSuccess++;
        } else {
            Log::err(TAG, "Expected " + s2 + ", but got " + s3 + " for " + s1);
        }
    }
    return nSuccess == tests.size();
}

