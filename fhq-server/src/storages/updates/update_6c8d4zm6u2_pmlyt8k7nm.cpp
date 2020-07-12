#include "update_6c8d4zm6u2_pmlyt8k7nm.h"

REGISTRY_WSJCPP_STORAGE_UPDATE(Update_6c8d4zm6u2_pmlyt8k7nm)

Update_6c8d4zm6u2_pmlyt8k7nm::Update_6c8d4zm6u2_pmlyt8k7nm()
    : WsjcppStorageUpdateBase("6c8d4zm6u2", "pmlyt8k7nm", "Added new column meta to public_events") {
    
    WsjcppStorageModifyTable *public_events = modifyTable("public_events");
    public_events->addColumn("meta").string(4096).notNull().defaultValue("");

    // fill the array with struct changes
}

