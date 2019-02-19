#include <string>
#include <logger.h>
#include <unit_tests.h>

int main(int argc, char** argv) {
    std::string TAG = "MAIN";
    Log::setdir("./");

    if (!UnitTests::runUnitTests()) {
        return -1;
    }
    return 0;
}
