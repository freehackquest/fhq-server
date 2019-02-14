#include <unit_test_sha1.h>
#include <vector>
#include <sha1_wrapper.h>
#include <str.h>

REGISTRY_UNIT_TEST(UnitTestSha1)


UnitTestSha1::UnitTestSha1()
    : UnitTestBase("UnitTestSha1") {
    //
}

void UnitTestSha1::init() {
    // nothing
}

bool UnitTestSha1::run() {

    struct LTest {
        LTest(std::string s1, std::string s2) : s1(s1), s2(s2) {}
        std::string s1;
        std::string s2;
    };

    std::vector<LTest *> tests;
    tests.push_back(new LTest("test", "a94a8fe5ccb19ba61c4c0873d391e987982fbbd3"));
    tests.push_back(new LTest("admin", "d033e22ae348aeb5660fc2140aec35850c4da997"));

    unsigned int nSuccess = 0;
    for (unsigned int i = 0; i < tests.size(); i++) {
        std::string s1 = tests[i]->s1;
        std::string s2 = tests[i]->s2;
        std::string s3 = sha1::calc_string_to_hex(s1);
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

