#include <string>
#include <fallen.h>
#include <unit_tests.h>

int main(int argc, char** argv) {
    Fallen::initRandom();
    std::string TAG = "UnitTests";
    Log::setdir("./");

    if (!UnitTests::runUnitTests()) {
        Log::err(TAG, "Some unit tests failed");
        return -1;
    }
    return 0;
}
