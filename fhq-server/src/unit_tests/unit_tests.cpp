#include "unit_tests.h"

UnitTestBase::UnitTestBase(const std::string &sTestName) {
    m_sTestName = sTestName;
    TAG = m_sTestName;
    UnitTests::addUnitTest(sTestName, this);
}

std::string UnitTestBase::name() {
    return m_sTestName;
}


std::vector<UnitTestBase*> *g_pUnitTests = NULL;

void UnitTests::initGlobalVariables(){
    if(g_pUnitTests == NULL){
        // Log::info(std::string(), "Create handlers map");
        g_pUnitTests = new std::vector<UnitTestBase*>();
    }
}

// ---------------------------------------------------------------------

void UnitTests::addUnitTest(const std::string &sTestName, UnitTestBase* pUnitTest){
    UnitTests::initGlobalVariables();
    bool bFound = false;
    for (int i = 0; i < g_pUnitTests->size(); i++) {
        UnitTestBase* p = g_pUnitTests->at(i);
        if (p->name() == sTestName) {
            bFound = true;
        }
    }
    
    if (bFound) {
        Log::err(sTestName, "Already registered");
    } else {
        g_pUnitTests->push_back(pUnitTest);
        // Log::info(sCmd, "Registered");
    }
}

// ---------------------------------------------------------------------

bool UnitTests::runUnitTests() {
    UnitTests::initGlobalVariables();
    int nAll = g_pUnitTests->size();
    Log::info("runUnitTests",  "All tests count " + std::to_string(nAll));
    int nSuccess = 0;
    for (int i = 0; i < g_pUnitTests->size(); i++) {
        UnitTestBase* pUnitTest = g_pUnitTests->at(i);
        std::string sTestName = pUnitTest->name();
        Log::info("runUnitTests",  "Run test " + sTestName);
        if (pUnitTest->run()) {
            Log::info(sTestName,  "Test passed"); // TODO Log::ok
            nSuccess++;
        } else {
            Log::err(sTestName,  "Test failed");
        }
    }
    Log::info("runUnitTests",  "Passed tests " + std::to_string(nSuccess) + " / " + std::to_string(nAll));
    return nSuccess != nAll;
}

// ---------------------------------------------------------------------