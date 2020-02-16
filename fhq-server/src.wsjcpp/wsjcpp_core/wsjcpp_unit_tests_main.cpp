#include <string>
#include <wsjcpp_core.h>
#include <wsjcpp_unit_tests.h>

int main(int argc, char** argv) {
    WSJCppCore::initRandom();
    std::string TAG = "UnitTests";
    WSJCppLog::setPrefixLogFile("unit-tests");
    std::string sLogDir = "./logs"; 
    if (!WSJCppCore::dirExists(sLogDir)) {
        WSJCppCore::makeDir(sLogDir);
    }
    WSJCppLog::setLogDirectory(sLogDir);
    if (!WSJCppCore::dirExists(sLogDir)) {
        WSJCppLog::err(TAG, "Directory '" + sLogDir + "' did'not exists");
        return -1;
    }

    if (!WSJCppUnitTests::runUnitTests()) {
        WSJCppLog::err(TAG, "Some unit tests failed");
        return -1;
    }
    return 0;
}