#include "wsjcpp_unit_tests.h"
#include <cmath>

// ---------------------------------------------------------------------
// WsjcppUnitTestBase

WsjcppUnitTestBase::WsjcppUnitTestBase(const std::string &sTestName) {
    m_sTestName = sTestName;
    TAG = m_sTestName;
    m_bTestResult = true;
    WsjcppUnitTests::addUnitTest(sTestName, this);
}

// ---------------------------------------------------------------------

std::string WsjcppUnitTestBase::getName() {
    return m_sTestName;
}

// ---------------------------------------------------------------------

void WsjcppUnitTestBase::ok(const std::string &sSuccessMessage) {
    // print obly success message
    WsjcppLog::ok(TAG,  sSuccessMessage);
}

// ---------------------------------------------------------------------

void WsjcppUnitTestBase::fail(const std::string &sFailedMessage) {
    WsjcppLog::err(TAG, sFailedMessage);
    m_bTestResult = false;
}

// ---------------------------------------------------------------------

bool WsjcppUnitTestBase::runTest() {
    WsjcppLog::info(TAG, "Start unit-test");
    WsjcppLog::info(TAG, "Do before unit-test");
    if (!doBeforeTest()) {
        fail("Problem with before unit-test");
        return false;
    }
    WsjcppLog::info(TAG,  "Execute unit-test");
    try {
        executeTest();
    } catch(const std::exception& e) {
        fail(e.what());
    } catch(...) {

    }
    if (m_bTestResult) {
        ok("Test passed.");
    } else {
        fail("Test failed.");
    }
    WsjcppLog::info(TAG, "Do after unit-test");
    if (!doAfterTest()) {
        fail("Problem with after unit-test");
    }
    WsjcppLog::info(TAG, "End unit-test");
    return m_bTestResult;
}

// ---------------------------------------------------------------------

bool WsjcppUnitTestBase::compareD(const std::string &sMark, double nValue, double nExpected) {
    if (abs(nValue - nExpected) > std::numeric_limits<double>::epsilon()) {
        fail(" {" + sMark + "} Expected '" + std::to_string(nExpected) + "', but got '" + std::to_string(nValue) + "'");
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

std::vector<WsjcppUnitTestBase*> *g_pWsjcppUnitTests = nullptr;

void WsjcppUnitTests::initGlobalVariables() {
    if (g_pWsjcppUnitTests == nullptr) {
        // WsjcppLog::info(std::string(), "Create handlers map");
        g_pWsjcppUnitTests = new std::vector<WsjcppUnitTestBase*>();
    }
}

// ---------------------------------------------------------------------

void WsjcppUnitTests::addUnitTest(const std::string &sTestName, WsjcppUnitTestBase* pUnitTest) {
    WsjcppUnitTests::initGlobalVariables();
    bool bFound = false;
    for (int i = 0; i < g_pWsjcppUnitTests->size(); i++) {
        WsjcppUnitTestBase* p = g_pWsjcppUnitTests->at(i);
        if (p->getName() == sTestName) {
            bFound = true;
        }
    }
    
    if (bFound) {
        WsjcppLog::err(sTestName, "Already registered");
    } else {
        g_pWsjcppUnitTests->push_back(pUnitTest);
        // Log::info(sCmd, "Registered");
    }
}

// ---------------------------------------------------------------------