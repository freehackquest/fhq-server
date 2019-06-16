#include "unit_test_hashes.h"
#include <vector>
#include <md5.h>
#include <fallen.h>
#include <sha1_wrapper.h>

REGISTRY_UNIT_TEST(UnitTestHashes)


UnitTestHashes::UnitTestHashes()
    : UnitTestBase("UnitTestHashes") {
    //
}

void UnitTestHashes::init() {
    // nothing
}

bool UnitTestHashes::run() {

    struct LTest {
        LTest(std::string s1, std::string s2, std::string algo) : s1(s1), s2(s2), algo(algo) {}
        std::string s1;
        std::string s2;
        std::string algo;
    };

    std::vector<LTest *> tests;
    tests.push_back(new LTest("test", "098F6BCD4621D373CADE4E832627B4F6", "md5"));
    tests.push_back(new LTest("admin", "21232F297A57A5A743894A0E4A801FC3", "md5"));
    tests.push_back(new LTest("test", "a94a8fe5ccb19ba61c4c0873d391e987982fbbd3", "sha1"));
    tests.push_back(new LTest("admin", "d033e22ae348aeb5660fc2140aec35850c4da997", "sha1"));
   

    unsigned int nSuccess = 0;
    for (unsigned int i = 0; i < tests.size(); i++) {
        std::string s1 = tests[i]->s1;
        std::string s2 = tests[i]->s2;
        std::string algo = tests[i]->algo;
        std::string s3 = "";
        if (algo == "md5") {
            s3 = md5(s1);
        }
        if (algo == "sha1") {
            s3 = sha1::calc_string_to_hex(s1);
        }
        Fallen::to_lower(s2);
        Fallen::to_lower(s3);
        if (s3 == s2) {
            nSuccess++;
        } else {
            Log::err(TAG, "Expected " + s2 + ", but got " + s3 + " for " + s1);
        }
    }
    return nSuccess == tests.size();
}

