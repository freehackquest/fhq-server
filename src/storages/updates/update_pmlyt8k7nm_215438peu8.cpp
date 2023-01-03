#include "update_pmlyt8k7nm_215438peu8.h"

REGISTRY_WSJCPP_STORAGE_UPDATE(Update_pmlyt8k7nm_215438peu8)

Update_pmlyt8k7nm_215438peu8::Update_pmlyt8k7nm_215438peu8()
    : WsjcppStorageUpdateBase("pmlyt8k7nm", "215438peu8", "Delete column games.rules") {

  WsjcppStorageModifyTable *games = modifyTable("games");
  games->dropColumn("rules");
}
