#include <string>
#include <wsjcpp_core.h>
#include <wsjcpp_unit_tests.h>

void printHelp(const std::string &sProgramName) {
    std::string sOutput = "\nHelp:\n";
    sOutput += 
        " '" + sProgramName + "' - run all unit-tests\n"
        " '" + sProgramName + " help' - print this help\n"
        " '" + sProgramName + " list' - print list of unit-tests\n"
        " '" + sProgramName + " run <TestName>' - run single unit-test\n"
    ;
    WsjcppLog::info("UnitTests", sOutput);
}

int main(int argc, char** argv) {
    WsjcppCore::initRandom();
    std::string TAG = "UnitTests";
    WsjcppLog::setPrefixLogFile("unit-tests");
    std::string sLogDir = "./logs"; 
    if (!WsjcppCore::dirExists(sLogDir)) {
        WsjcppCore::makeDir(sLogDir);
    }
    WsjcppLog::setLogDirectory(sLogDir);
    if (!WsjcppCore::dirExists(sLogDir)) {
        WsjcppLog::err(TAG, "Directory '" + sLogDir + "' did'not exists");
        return -1;
    }

    WsjcppUnitTests::initGlobalVariables();
    std::string sProgramName(argv[0]);

    if (argc == 1) {
        int nAll = g_pWsjcppUnitTests->size();
        WsjcppLog::info("runUnitTests",  "All tests count " + std::to_string(nAll));
        int nSuccess = 0;
        for (int i = 0; i < g_pWsjcppUnitTests->size(); i++) {
            WsjcppUnitTestBase* pUnitTest = g_pWsjcppUnitTests->at(i);
            std::string sTestName = pUnitTest->name();
            WsjcppLog::info("runUnitTests",  "Run test " + sTestName);
            if (pUnitTest->run()) {
                WsjcppLog::ok(sTestName,  "Test passed");
                nSuccess++;
            } else {
                WsjcppLog::err(sTestName,  "Test failed");
            }
        }
        WsjcppLog::info(TAG,  "Passed tests " + std::to_string(nSuccess) + " / " + std::to_string(nAll));
        bool bResult = nSuccess == nAll;
        return bResult ? 0 : -1;
    } else if (argc == 2) {
        std::string sArg2(argv[1]);
        if (sArg2 == "list") {
            std::string sOutput = "\nList of unit-tests:\n";
            for (int i = 0; i < g_pWsjcppUnitTests->size(); i++) {
                WsjcppUnitTestBase* pUnitTest = g_pWsjcppUnitTests->at(i);
                sOutput += "  - " + pUnitTest->name() + "\n";
            }
            WsjcppLog::info(TAG,  sOutput);
            return -1;
        } else if (sArg2 == "help") {
            printHelp(sProgramName);
            return -1;
        }
    } else if (argc == 3) {
        std::string sArg2(argv[1]);
        std::string sArg3(argv[2]);
        if (sArg2 == "run") {
            int nSuccess = 0;
            bool bTestFound = false;
            for (int i = 0; i < g_pWsjcppUnitTests->size(); i++) {
                WsjcppUnitTestBase* pUnitTest = g_pWsjcppUnitTests->at(i);
                if (pUnitTest->name() == sArg3) {
                    bTestFound = true;
                    if (pUnitTest->run()) {
                        WsjcppLog::ok(TAG, pUnitTest->name() + " Test passed");
                        nSuccess++;
                    } else {
                        WsjcppLog::err(TAG,  pUnitTest->name() + " Test failed");
                    }
                }
            }
            if (!bTestFound) {
                WsjcppLog::err(TAG,  "Test not found try help");
            }
            return -1;
        }
        printHelp(sProgramName);
        return -1;
    }

    printHelp(sProgramName);
    return 0;
}