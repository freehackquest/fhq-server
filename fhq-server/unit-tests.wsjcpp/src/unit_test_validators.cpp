#include <wsjcpp_unit_tests.h>
#include <vector>
#include <iostream>
#include <validators/validators.h>

class UnitTestValidators : public WsjcppUnitTestBase {
    public:
        UnitTestValidators();
        virtual bool doBeforeTest();
        virtual void executeTest();
        virtual bool doAfterTest();
};

REGISTRY_WSJCPP_UNIT_TEST(UnitTestValidators)


UnitTestValidators::UnitTestValidators()
    : WsjcppUnitTestBase("UnitTestValidators") {
    //
}

bool UnitTestValidators::doBeforeTest() {
    // nothing
    return true;
}

void UnitTestValidators::executeTest() {

    struct LTestVld {
        LTestVld(WsjcppValidatorStringBase *pValidator, std::string sValue, bool bExpectedResult) {
            m_pValidator = pValidator;
            m_sValue = sValue;
            m_bExpectedResult = bExpectedResult;
        }

        WsjcppValidatorStringBase *m_pValidator;
        std::string m_sValue;
        int m_bExpectedResult;
    };

    std::vector<LTestVld *> tests;

    ValidatorGameState *pValidatorGameState = new ValidatorGameState();
    tests.push_back(new LTestVld(pValidatorGameState, "test", false));
    tests.push_back(new LTestVld(pValidatorGameState, "original", true));
    tests.push_back(new LTestVld(pValidatorGameState, "copy", true));
    tests.push_back(new LTestVld(pValidatorGameState, "unlicensed_copy", true));
    tests.push_back(new LTestVld(pValidatorGameState, "some", false));

    for (unsigned int i = 0; i < tests.size(); i++) {
        std::string sValue = tests[i]->m_sValue;
        WsjcppValidatorStringBase *pValidator = tests[i]->m_pValidator;
        bool bExpectedResult = tests[i]->m_bExpectedResult;
        std::string sError = "";
        bool bGotResult = pValidator->isValid(sValue, sError);
        compare("Test '" + sValue + "' error: " + sError, bGotResult, bExpectedResult);
    }
}

bool UnitTestValidators::doAfterTest() {
    // nothing
    return true;
}