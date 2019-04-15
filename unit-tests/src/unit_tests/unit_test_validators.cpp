#include "unit_test_validators.h"
#include <vector>
#include <iostream>
#include <validators.h>

REGISTRY_UNIT_TEST(UnitTestValidators)


UnitTestValidators::UnitTestValidators()
    : UnitTestBase("UnitTestValidators") {
    //
}

void UnitTestValidators::init() {
    // nothing
}

bool UnitTestValidators::run() {

    struct LTestVld {
        LTestVld(ValidatorStringBase *pValidator, std::string sValue, bool bExpectedResult) {
            m_pValidator = pValidator;
            m_sValue = sValue;
            m_bExpectedResult = bExpectedResult;
        }

        ValidatorStringBase *m_pValidator;
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

    ValidatorEmail *pValidatorEmail = new ValidatorEmail();
    tests.push_back(new LTestVld(pValidatorEmail, "some", false));
    tests.push_back(new LTestVld(pValidatorEmail, "some@some", false));
    tests.push_back(new LTestVld(pValidatorEmail, "some@some.rr", true));
    tests.push_back(new LTestVld(pValidatorEmail, "01@some.com", true));
    tests.push_back(new LTestVld(pValidatorEmail, "s_s-some@test.com", true));
    tests.push_back(new LTestVld(pValidatorEmail, "s_s-some@test.c", false));

    ValidatorUUID *pValidatorUUID = new ValidatorUUID();
    tests.push_back(new LTestVld(pValidatorUUID, "some", false));
    tests.push_back(new LTestVld(pValidatorUUID, "00000000-0000-0000-1000-000000000001", true));
    tests.push_back(new LTestVld(pValidatorUUID, "abcdef01-ABCD-EF23-1000-000000000001", true));

    unsigned int nSuccess = 0;
    for (unsigned int i = 0; i < tests.size(); i++) {
        std::string sValue = tests[i]->m_sValue;
        ValidatorStringBase *pValidator = tests[i]->m_pValidator;
        bool bExpectedResult = tests[i]->m_bExpectedResult;
        std::string sError = "";
        bool bGotResult = pValidator->isValid(sValue, sError);
        if (bGotResult == bExpectedResult) {
            nSuccess++;
        } else {
            std::string sExpectedResult = bExpectedResult ? "true" : "false";
            std::string sGotResult = bGotResult ? "true" : "false";

            Log::err(TAG, "Test #" + std::to_string(i) + " validator failed "
                " expected result " + sExpectedResult + ", but got " + sGotResult + 
                " value: " + sValue + ", error: " + sError
            );
        }
    }
    return nSuccess == tests.size();
}

