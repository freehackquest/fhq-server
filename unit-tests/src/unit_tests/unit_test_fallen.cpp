#include "unit_test_fallen.h"
#include <vector>
#include <iostream>
#include <fallen.h>

REGISTRY_UNIT_TEST(UnitTestFallen)


UnitTestFallen::UnitTestFallen()
    : UnitTestBase("UnitTestFallen") {
    //
}

void UnitTestFallen::init() {
    // nothing
}

bool UnitTestFallen::run() {
    std::vector<std::string> vUuids;
    for (int i = 0; i < 100; i++) {
        std::string sUuid = Fallen::createUuid();
        Log::info(TAG, "sUuid: " + sUuid);
        vUuids.push_back(sUuid);
    }

    int nCounter = 0;
    for (int x = 0; x < 100; x++) {
        for (int y = 0; y < 100; y++) {
            if (vUuids[x] == vUuids[y] && x != y) {
                nCounter++;
                Log::err(TAG, "Wrong generation random uuids: \nvUuids[" + std::to_string(x)+ "] == vUuids[" + std::to_string(y) + "] == '" + vUuids[x] + "'");
            }
        } 
    }

    return nCounter == 0;
}

