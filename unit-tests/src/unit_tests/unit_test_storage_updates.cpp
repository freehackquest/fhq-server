
#include "unit_test_storage_updates.h"
#include <storages.h>

REGISTRY_UNIT_TEST(UnitTestStorageUpdates)

UnitTestStorageUpdates::UnitTestStorageUpdates() 
    : UnitTestBase("UnitTestStorageUpdates") {
    // 
}

// ---------------------------------------------------------------------

void UnitTestStorageUpdates::init() {
}

// ---------------------------------------------------------------------

bool UnitTestStorageUpdates::run() {
    Storage *pStorage = Storages::create("mysql");
    bool bTestSuccess = true;
    
    return bTestSuccess;
}

// ---------------------------------------------------------------------