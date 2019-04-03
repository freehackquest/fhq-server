#include <string>
#include <logger.h>
#include <unit_tests.h>

int main(int argc, char** argv) {
    std::string TAG = "UnitTests";
    Log::setdir("./");

    if (!UnitTests::runUnitTests()) {
        Log::err(TAG, "Some unit tests failed");
        return -1;
    }
    return 0;
}
