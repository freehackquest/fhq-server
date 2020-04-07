#include "wsjcpp_unit_tests.h"

WsjcppUnitTestBase::WsjcppUnitTestBase(const std::string &sTestName) {
    m_sTestName = sTestName;
    TAG = m_sTestName;
    WsjcppUnitTests::addUnitTest(sTestName, this);
}

// ---------------------------------------------------------------------

std::string WsjcppUnitTestBase::name() {
    return m_sTestName;
}

// ---------------------------------------------------------------------

void WsjcppUnitTestBase::compareS(bool &bTestSuccess, const std::string &sPoint,
    const std::string &sValue, const std::string &sExpected) {
    if (sValue != sExpected) {
        WsjcppLog::err(TAG, " {" + sPoint + "} Expected '" + sExpected + "', but got '" + sValue + "'");
        bTestSuccess = false;
    }
}

// ---------------------------------------------------------------------

bool WsjcppUnitTestBase::compareN(bool &bTestSuccess, const std::string &sPoint, int nValue, int nExpected) {
    if (nValue != nExpected) {
        WsjcppLog::err(TAG, " {" + sPoint + "} Expected '" + std::to_string(nExpected) + "', but got '" + std::to_string(nValue) + "'");
        bTestSuccess = false;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppUnitTestBase::compareD(bool &bTestSuccess, const std::string &sPoint, double nValue, double nExpected) {
    if (nValue != nExpected) {
        WsjcppLog::err(TAG, " {" + sPoint + "} Expected '" + std::to_string(nExpected) + "', but got '" + std::to_string(nValue) + "'");
        bTestSuccess = false;
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

void WsjcppUnitTestBase::compareB(bool &bTestSuccess, const std::string &sPoint, bool bValue, bool bExpected) {
    if (bValue != bExpected) {
        WsjcppLog::err(TAG, " {" + sPoint + "} Expected '" + (bExpected ? "true" : "false") + "', but got '" + (bValue ? "true" : "false") + "'");
        bTestSuccess = false;
    }
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
        if (p->name() == sTestName) {
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