#include <string>
#include <fallen.h>
#include <unit_tests.h>

int main(int argc, char** argv) {
    Fallen::initRandom();
    std::string TAG = "UnitTests";
    Log::setPrefixLogFile("unit-tests");
    std::string sLogDir = "./"; 
    Log::setLogDirectory(sLogDir);
    if (!Fallen::dirExists(sLogDir)) {
        Log::err(TAG, "Directory '" + sLogDir + "' did'not exists");
        return -1;
    }

    if (!UnitTests::runUnitTests()) {
        Log::err(TAG, "Some unit tests failed");
        return -1;
    }
    return 0;
}
