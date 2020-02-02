#include "wsjcpp_unit_tests.h"

UnitTestBase::UnitTestBase(const std::string &sTestName) {
    m_sTestName = sTestName;
    TAG = m_sTestName;
    UnitTests::addUnitTest(sTestName, this);
}

// ---------------------------------------------------------------------

std::string UnitTestBase::name() {
    return m_sTestName;
}

// ---------------------------------------------------------------------

void UnitTestBase::compareS(bool &bTestSuccess, const std::string &sPoint,
    const std::string &sValue, const std::string &sExpected) {
    if (sValue != sExpected) {
        WSJCppLog::err(TAG, " {" + sPoint + "} Expected '" + sExpected + "', but got '" + sValue + "'");
        bTestSuccess = false;
    }
}

// ---------------------------------------------------------------------

bool UnitTestBase::compareN(bool &bTestSuccess, const std::string &sPoint, int nValue, int nExpected) {
    if (nValue != nExpected) {
        WSJCppLog::err(TAG, " {" + sPoint + "} Expected '" + std::to_string(nExpected) + "', but got '" + std::to_string(nValue) + "'");
        bTestSuccess = false;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool UnitTestBase::compareD(bool &bTestSuccess, const std::string &sPoint, double nValue, double nExpected) {
    if (nValue != nExpected) {
        WSJCppLog::err(TAG, " {" + sPoint + "} Expected '" + std::to_string(nExpected) + "', but got '" + std::to_string(nValue) + "'");
        bTestSuccess = false;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

void UnitTestBase::compareB(bool &bTestSuccess, const std::string &sPoint, bool bValue, bool bExpected) {
    if (bValue != bExpected) {
        WSJCppLog::err(TAG, " {" + sPoint + "} Expected '" + (bExpected ? "true" : "false") + "', but got '" + (bValue ? "true" : "false") + "'");
        bTestSuccess = false;
    }
}

// ---------------------------------------------------------------------

std::vector<UnitTestBase*> *g_pUnitTests = NULL;

void UnitTests::initGlobalVariables() {
    if (g_pUnitTests == NULL) {
        // Log::info(std::string(), "Create handlers map");
        g_pUnitTests = new std::vector<UnitTestBase*>();
    }
}

// ---------------------------------------------------------------------

void UnitTests::addUnitTest(const std::string &sTestName, UnitTestBase* pUnitTest) {
    UnitTests::initGlobalVariables();
    bool bFound = false;
    for (int i = 0; i < g_pUnitTests->size(); i++) {
        UnitTestBase* p = g_pUnitTests->at(i);
        if (p->name() == sTestName) {
            bFound = true;
        }
    }
    
    if (bFound) {
        WSJCppLog::err(sTestName, "Already registered");
    } else {
        g_pUnitTests->push_back(pUnitTest);
        // Log::info(sCmd, "Registered");
    }
}

// ---------------------------------------------------------------------

bool UnitTests::runUnitTests() {
    UnitTests::initGlobalVariables();
    int nAll = g_pUnitTests->size();
    WSJCppLog::info("runUnitTests",  "All tests count " + std::to_string(nAll));
    int nSuccess = 0;
    for (int i = 0; i < g_pUnitTests->size(); i++) {
        UnitTestBase* pUnitTest = g_pUnitTests->at(i);
        std::string sTestName = pUnitTest->name();
        WSJCppLog::info("runUnitTests",  "Run test " + sTestName);
        if (pUnitTest->run()) {
            WSJCppLog::ok(sTestName,  "Test passed");
            nSuccess++;
        } else {
            WSJCppLog::err(sTestName,  "Test failed");
        }
    }
    WSJCppLog::info("runUnitTests",  "Passed tests " + std::to_string(nSuccess) + " / " + std::to_string(nAll));
    return nSuccess == nAll;
}

// ---------------------------------------------------------------------