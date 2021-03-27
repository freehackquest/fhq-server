#include <wsjcpp_unit_tests.h>
#include <wsjcpp_storages.h>

class UnitTestStorageEscapes : public WsjcppUnitTestBase {
    public:
        UnitTestStorageEscapes();
        virtual bool doBeforeTest();
        virtual void executeTest();
        virtual bool doAfterTest();
};

REGISTRY_WSJCPP_UNIT_TEST(UnitTestStorageEscapes)

UnitTestStorageEscapes::UnitTestStorageEscapes() 
    : WsjcppUnitTestBase("UnitTestStorageEscapes") {
    // 
}

// ---------------------------------------------------------------------

bool UnitTestStorageEscapes::doBeforeTest() {
    // nothing
    return true;
}

// ---------------------------------------------------------------------

void UnitTestStorageEscapes::executeTest() {
    if (!compare("Not supported mysql", WsjcppStorages::support("mysql"), true)) {
        return;
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
        compare("sql", s3, s2);
    }
}

// ---------------------------------------------------------------------

bool UnitTestStorageEscapes::doAfterTest() {
    // nothing
    return true;
}