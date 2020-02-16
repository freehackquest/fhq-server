#include "unit_test_validators.h"
#include <vector>
#include <iostream>
#include <validators/validators.h>

REGISTRY_UNIT_TEST(UnitTestValidators)


UnitTestValidators::UnitTestValidators()
    : WSJCppUnitTestBase("UnitTestValidators") {
    //
}

void UnitTestValidators::init() {
    // nothing
}

bool UnitTestValidators::run() {

    struct LTestVld {
        LTestVld(WSJCppValidatorStringBase *pValidator, std::string sValue, bool bExpectedResult) {
            m_pValidator = pValidator;
            m_sValue = sValue;
            m_bExpectedResult = bExpectedResult;
        }

        WSJCppValidatorStringBase *m_pValidator;
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

    bool bTestSuccess = true;
    for (unsigned int i = 0; i < tests.size(); i++) {
        std::string sValue = tests[i]->m_sValue;
        WSJCppValidatorStringBase *pValidator = tests[i]->m_pValidator;
        bool bExpectedResult = tests[i]->m_bExpectedResult;
        std::string sError = "";
        bool bGotResult = pValidator->isValid(sValue, sError);
        compareB(bTestSuccess, "Test '" + sValue + "' error: " + sError, bGotResult, bExpectedResult);
    }
    return bTestSuccess;
}

