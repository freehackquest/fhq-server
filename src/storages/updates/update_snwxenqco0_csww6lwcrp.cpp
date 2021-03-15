#include "update_snwxenqco0_csww6lwcrp.h"

REGISTRY_WSJCPP_STORAGE_UPDATE(Update_snwxenqco0_csww6lwcrp)

Update_snwxenqco0_csww6lwcrp::Update_snwxenqco0_csww6lwcrp()
    : WsjcppStorageUpdateBase("snwxenqco0", "csww6lwcrp", "Redesign user tokens") {
    
    WsjcppStorageModifyTable *users_tokens = modifyTable("users_tokens");
    users_tokens->addColumn("start_dt").bigNumber().notNull().defaultValue("0");
    users_tokens->addColumn("end_dt").bigNumber().notNull().defaultValue("0");
}
