#include "wsjcpp_storages.h"
#include <algorithm>

// ***********************
// *** WsjcppStorageColumnDef
// ***********************

// ---------------------------------------------------------------------

WsjcppStorageColumnDef::WsjcppStorageColumnDef(const std::string &sColumnName) {
    TAG = "StorageColumnDef_" + sColumnName;
    // TODO validate column name

    m_sColumnName = sColumnName;
    m_bAutoIncrement = false;
    m_bNotNull = false;
    m_nTypeSize = 0;
    m_sType = "";
    m_bPrimaryKey = false;
    m_sDefaultValue = "";
    m_bEnableIndex = false;
    m_bEnableUniqueIndex = false;
    m_bDefaultValue = false;
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageColumnDef::autoIncrement() {
    m_bAutoIncrement = true;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageColumnDef::notNull() {
    m_bNotNull = true;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageColumnDef::string(int nValue) {
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "string";
    m_nTypeSize = nValue;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageColumnDef::text() {
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "text";
    return *this;
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageColumnDef::datetime() {
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "datetime";
    return *this;
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageColumnDef::number() {
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "number";
    return *this;
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageColumnDef::bigNumber() {
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "bigNumber";
    return *this;
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageColumnDef::doubleNumber() {
    if (m_sType != "") {
        WsjcppLog::throw_err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "doubleNumber";
    return *this;
}


// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageColumnDef::primaryKey() {
    m_bPrimaryKey = true;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageColumnDef::defaultValue(const std::string& sDefaultValue) {
    m_sDefaultValue = sDefaultValue;
    m_bDefaultValue = true;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageColumnDef::enableIndex() {
    m_bEnableIndex = true;
    return *this;
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageColumnDef::enableUniqueIndex(const std::string& sIndexName) {
    m_sNameOfUniqueIndex = sIndexName;
    m_bEnableUniqueIndex = true;
    return *this;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageColumnDef::columnName() const {
    return m_sColumnName;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageColumnDef::columnType() {
    return m_sType;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageColumnDef::columnDefaultValue() {
    return m_sDefaultValue;
}

// ---------------------------------------------------------------------

bool WsjcppStorageColumnDef::isDefaultValue() {
    return m_bDefaultValue;
}

// ---------------------------------------------------------------------

int WsjcppStorageColumnDef::columnTypeSize() {
    return m_nTypeSize;
}

// ---------------------------------------------------------------------

bool WsjcppStorageColumnDef::isAutoIncrement() {
    return m_bAutoIncrement;
}

// ---------------------------------------------------------------------

bool WsjcppStorageColumnDef::isPrimaryKey() {
    return m_bPrimaryKey;
}

// ---------------------------------------------------------------------

bool WsjcppStorageColumnDef::isNotNull() {
    return m_bNotNull;
}

// ---------------------------------------------------------------------

bool WsjcppStorageColumnDef::isEnableIndex() {
    return m_bEnableIndex;
}

// ---------------------------------------------------------------------

bool WsjcppStorageColumnDef::isEnableUniqueIndex() {
    return m_bEnableUniqueIndex;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageColumnDef::nameOfUniqueIndex() {
    return m_sNameOfUniqueIndex;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageChanges
// ***********************

// ---------------------------------------------------------------------

WsjcppStorageChanges::WsjcppStorageChanges(const std::string &sTableName) {
    TAG = "StorageChanges";
    m_sTableName = sTableName;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageChanges::getTableName() const {
    return m_sTableName;
}

// ---------------------------------------------------------------------

WsjcppStorageChangesType WsjcppStorageChanges::getType() const {
    return WSJCPP_STORAGE_CHANGES_TYPE_NOPE;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageChanges::getStartApply() const {
    return "Appling changes for table " + m_sTableName;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageChanges::getAppliedSuccess() const {
    return "Changes success applied for table " + m_sTableName;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageChanges::getAppliedFailed() const {
    return "Could not apply changes for table " + m_sTableName;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageCreateTable
// ***********************

WsjcppStorageCreateTable::WsjcppStorageCreateTable(const std::string &sTableName) 
    : WsjcppStorageChanges(sTableName) {
    TAG = "WsjcppStorageCreateTable";
    // TODO validate table of name
}

// ---------------------------------------------------------------------

WsjcppStorageChangesType WsjcppStorageCreateTable::getType() const {
    return WSJCPP_STORAGE_CHANGES_TYPE_CREATE_TABLE;
};

// ---------------------------------------------------------------------

std::string WsjcppStorageCreateTable::getStartApply() const {
    return "Creating table '" + m_sTableName + "'";
}

// ---------------------------------------------------------------------

std::string WsjcppStorageCreateTable::getAppliedSuccess() const {
    return "Created table '" +  m_sTableName + "'";
}

// ---------------------------------------------------------------------

std::string WsjcppStorageCreateTable::getAppliedFailed() const {
    return "Could not create table '" +  m_sTableName + "'";
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageCreateTable::addColumn(const std::string &sColumnName) {
    for (int i = 0; i < m_vColumns.size(); i++) {
        if (m_vColumns[i].columnName() == sColumnName) {
            std::string sError = "Column '" + sColumnName + "' in table '" + m_sTableName + "' already exists";
            WsjcppLog::throw_err(TAG, sError);
        }
    }
    m_vColumns.push_back(WsjcppStorageColumnDef(sColumnName));
    return m_vColumns[m_vColumns.size()-1];
}

// ---------------------------------------------------------------------

const std::vector<WsjcppStorageColumnDef> &WsjcppStorageCreateTable::getColumns() const {
    return m_vColumns;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageModifyTable
// ***********************

WsjcppStorageModifyTable::WsjcppStorageModifyTable(const std::string &sTableName) 
    : WsjcppStorageChanges(sTableName) {
    TAG = "WsjcppStorageModifyTable";
    // TODO validate table of name
}

// ---------------------------------------------------------------------

WsjcppStorageChangesType WsjcppStorageModifyTable::getType() const {
    return WSJCPP_STORAGE_CHANGES_TYPE_MODIFY_TABLE;
};

// ---------------------------------------------------------------------

std::string WsjcppStorageModifyTable::getStartApply() const {
    return "Modifing table " + m_sTableName;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageModifyTable::getAppliedSuccess() const {
    return "Modified table " +  m_sTableName;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageModifyTable::getAppliedFailed() const {
    return "Could not modify table " +  m_sTableName;
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageModifyTable::addColumn(const std::string &sColumnName) {
    std::string sError;
    if (isColumnDefined(sColumnName, sError)) {
        WsjcppLog::throw_err(TAG, "addColumn, " + sError);
    }
    m_vAddColumns.push_back(WsjcppStorageColumnDef(sColumnName));
    return m_vAddColumns[m_vAddColumns.size()-1];
}

// ---------------------------------------------------------------------

WsjcppStorageColumnDef &WsjcppStorageModifyTable::alterColumn(const std::string &sColumnName) {
    std::string sError;
    if (isColumnDefined(sColumnName, sError)) {
        WsjcppLog::throw_err(TAG, "alterColumn, " + sError);
    }
    m_vAlterColumns.push_back(WsjcppStorageColumnDef(sColumnName));
    return m_vAlterColumns[m_vAlterColumns.size()-1];
}

// ---------------------------------------------------------------------

std::string WsjcppStorageModifyTable::dropColumn(const std::string &sColumnName) {
    std::string sError;
    if (isColumnDefined(sColumnName, sError)) {
        WsjcppLog::throw_err(TAG, "dropColumn, " + sError);
    }
    m_vDropColumns.push_back(sColumnName);
    return m_vDropColumns[m_vDropColumns.size()-1];
}

// ---------------------------------------------------------------------

const std::vector<WsjcppStorageColumnDef> &WsjcppStorageModifyTable::getAddColumns() const {
    return m_vAddColumns;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppStorageColumnDef> &WsjcppStorageModifyTable::getAlterColumns() const {
    return m_vAlterColumns;
}

// ---------------------------------------------------------------------

const std::vector<std::string> &WsjcppStorageModifyTable::getDropColumns() const {
    return m_vDropColumns;
}

// ---------------------------------------------------------------------

bool WsjcppStorageModifyTable::isColumnDefined(const std::string &sColumnName, std::string &sError) const {
    for (int i = 0; i < m_vAddColumns.size(); i++) {
        if (m_vAddColumns[i].columnName() == sColumnName) {
            sError = "Column '" + sColumnName + "' in table '" + m_sTableName + "' already defined as 'add' ";
            return true;
        }
    }

    for (int i = 0; i < m_vAlterColumns.size(); i++) {
        if (m_vAlterColumns[i].columnName() == sColumnName) {
            sError = "Column '" + sColumnName + "' in table '" + m_sTableName + "' already defined as 'alter'";
            return true;
        }
    }

    for (int i = 0; i < m_vDropColumns.size(); i++) {
        if (m_vDropColumns[i] == sColumnName) {
            sError = "Column '" + sColumnName + "' in table '" + m_sTableName + "' already defined as 'drop'";
            return true;
        }
    }

    return false;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageDropTable
// ***********************

WsjcppStorageDropTable::WsjcppStorageDropTable(const std::string &sTableName) 
    : WsjcppStorageChanges(sTableName) {
    TAG = "WsjcppStorageDropTable";
    // TODO validate table of name
}

// ---------------------------------------------------------------------

WsjcppStorageChangesType WsjcppStorageDropTable::getType() const {
    return WSJCPP_STORAGE_CHANGES_TYPE_DROP_TABLE;
};

// ---------------------------------------------------------------------

std::string WsjcppStorageDropTable::getStartApply() const {
    return "Dropping table " + m_sTableName;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageDropTable::getAppliedSuccess() const {
    return "Dropped table " +  m_sTableName;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageDropTable::getAppliedFailed() const {
    return "Could not drop table " +  m_sTableName;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageColumnValue
// ***********************

WsjcppStorageColumnValue::WsjcppStorageColumnValue(const std::string &sColumnName, WsjcppStorageColumnType nType) {
    m_sColumnName = sColumnName;
    m_nColumnType = nType;
}

// ---------------------------------------------------------------------

void WsjcppStorageColumnValue::setValue(std::string sValue) {
    m_sStringValue = sValue;
}

// ---------------------------------------------------------------------

void WsjcppStorageColumnValue::setValue(int nValue) {
    m_nIntValue = nValue;
}

// ---------------------------------------------------------------------

void WsjcppStorageColumnValue::setValue(double nValue) {
    m_nDoubleValue = nValue;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageColumnValue::getColumnName() {
    return m_sColumnName;
}

// ---------------------------------------------------------------------

WsjcppStorageColumnType WsjcppStorageColumnValue::getColumnType() {
    return m_nColumnType;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageColumnValue::getString() {
    return m_sStringValue;
}

// ---------------------------------------------------------------------

int WsjcppStorageColumnValue::getInt() {
    return m_nIntValue;
}

// ---------------------------------------------------------------------

double WsjcppStorageColumnValue::getDouble() {
    return m_nDoubleValue;
}

// ---------------------------------------------------------------------

// ***********************
// *** WsjcppStorageInsert
// ***********************

WsjcppStorageInsert::WsjcppStorageInsert(const std::string &sTableName)
    : WsjcppStorageChanges(sTableName) {
    TAG = "WsjcppStorageInsert";
    m_sTableName = sTableName;
}

// ---------------------------------------------------------------------

WsjcppStorageChangesType WsjcppStorageInsert::getType() const {
    return WSJCPP_STORAGE_CHANGES_TYPE_INSERT_ROW;
};

// ---------------------------------------------------------------------

std::string WsjcppStorageInsert::getStartApply() const {
    return "Inserting into table " + m_sTableName;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageInsert::getAppliedSuccess() const {
    return "Inserted into table " +  m_sTableName;
}

// ---------------------------------------------------------------------

std::string WsjcppStorageInsert::getAppliedFailed() const {
    return "Could not insert into table " +  m_sTableName;
}

// ---------------------------------------------------------------------

void WsjcppStorageInsert::bindValue(const std::string &sColumnName, const std::string &sValue) {
    if (this->exists(sColumnName)) {
        WsjcppLog::throw_err(TAG, "Skip. Already defined " + m_sTableName + "." + sColumnName);
        return;
    }
    WsjcppStorageColumnValue val(sColumnName, WSJCPP_STORAGE_COLUMN_TYPE_STRING);
    val.setValue(sValue);
    m_vValues.push_back(val);
}

// ---------------------------------------------------------------------

void WsjcppStorageInsert::bindValue(const std::string &sColumnName, int nValue) {
    if (this->exists(sColumnName)) {
        WsjcppLog::throw_err(TAG, "Skip. Already defined " + m_sTableName + "." + sColumnName);
        return;
    }
    WsjcppStorageColumnValue val(sColumnName, WSJCPP_STORAGE_COLUMN_TYPE_NUMBER);
    val.setValue(nValue);
    m_vValues.push_back(val);
}

// ---------------------------------------------------------------------

void WsjcppStorageInsert::bindValue(const std::string &sColumnName, double nValue) {
    if (this->exists(sColumnName)) {
        WsjcppLog::throw_err(TAG, "Skip. Already defined " + m_sTableName + "." + sColumnName);
        return;
    }
    WsjcppStorageColumnValue val(sColumnName, WSJCPP_STORAGE_COLUMN_TYPE_DOUBLE_NUMBER);
    val.setValue(nValue);
    m_vValues.push_back(val);
}

// ---------------------------------------------------------------------

std::vector<WsjcppStorageColumnValue> WsjcppStorageInsert::values() const {
    return m_vValues;
}

// ---------------------------------------------------------------------

bool WsjcppStorageInsert::exists(const std::string &sColumnName) {
    for (int i = 0; i < m_vValues.size(); i++) {
        if (m_vValues[i].getColumnName() == sColumnName) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

bool WsjcppStorageInsert::isValid(const WsjcppStorageTable &storageTable) const {
    if (storageTable.getTableName() != m_sTableName) {
        WsjcppLog::err(TAG, "Expeceted '" + m_sTableName + "', but got '" + storageTable.getTableName() + "'");
        return false;
    }

    std::vector<WsjcppStorageColumnDef> vColumns = storageTable.getColumns();
    std::vector<std::string> vCurrentColumns;
    
    for (int x = 0; x < m_vValues.size(); x++) {
        bool bFound = false;
        WsjcppStorageColumnValue val = m_vValues[x];
        vCurrentColumns.push_back(val.getColumnName());
        for (int y = 0; y < vColumns.size(); y++) {
            WsjcppStorageColumnDef st = vColumns[y];
            if (st.columnName() == val.getColumnName()) {
                if (val.getColumnType() == WSJCPP_STORAGE_COLUMN_TYPE_STRING
                    && (st.columnType() == "string" || st.columnType() == "datetime" || st.columnType() == "text")) {
                    bFound = true;
                    break;
                } else if (val.getColumnType() == WSJCPP_STORAGE_COLUMN_TYPE_NUMBER && st.columnType() == "number") {
                    bFound = true;
                    break;
                } else if (val.getColumnType() == WSJCPP_STORAGE_COLUMN_TYPE_DOUBLE_NUMBER && st.columnType() == "doubleNumber") {
                    bFound = true;
                    break;
                } else {
                    WsjcppLog::err(TAG, "StorageInsert::isValid. In struct '" + m_sTableName + "' column '" + val.getColumnName() + "'."
                        " Expected datatype '" + st.columnType() + "', but got '" + std::to_string(val.getColumnType()) + "'");
                    return false;
                }
            }
        }
        if (!bFound) {
            WsjcppLog::err(TAG, "StorageInsert::isValid. Struct '" + m_sTableName + "' has not column '" + val.getColumnName() + "'");
            return false;
        }
    }

    for (int i = 0; i < vColumns.size(); i++) {
        if (!vColumns[i].isAutoIncrement() && vColumns[i].isNotNull()) {
            if (std::find(vCurrentColumns.begin(), vCurrentColumns.end(), vColumns[i].columnName()) == vCurrentColumns.end()) {
                WsjcppLog::err(TAG, "Missing require field '" + m_sTableName + "." + vColumns[i].columnName() + "'");
                return false;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageUpdate
// ***********************

// ---------------------------------------------------------------------

/*
WsjcppStorageUpdate::WsjcppStorageUpdate(const StorageStruct &storageStruct) {
    m_storageStruct = storageStruct;
}
// ---------------------------------------------------------------------
void WsjcppStorageUpdate::bindValue(const std::string &sColumnName, const std::string &sValue) {
}
// ---------------------------------------------------------------------
void WsjcppStorageUpdate::bindValue(const std::string &sColumnName, int nValue) {
}
// ---------------------------------------------------------------------
void WsjcppStorageUpdate::bindValue(const std::string &sColumnName, double nValue) {
}
// ---------------------------------------------------------------------
std::vector<WsjcppStorageColumnValue> StorageUpdate::values() const {
}
// ---------------------------------------------------------------------
bool WsjcppStorageUpdate::isValid() const {
    return false;
}
// ---------------------------------------------------------------------
*/

// ---------------------------------------------------------------------

WsjcppStorageTable::WsjcppStorageTable(const std::string &sTableName) {
    m_sTableName = sTableName;
    TAG = "WsjcppStorageTable";
}

// ---------------------------------------------------------------------

WsjcppStorageTable::WsjcppStorageTable(WsjcppStorageCreateTable &createTable) {
    m_sTableName = createTable.getTableName();
    const std::vector<WsjcppStorageColumnDef> &list = createTable.getColumns();
    for (int i = 0; i < list.size(); i++) {
        m_vColumns.push_back(list[i]);
    }
}

// ---------------------------------------------------------------------

std::string WsjcppStorageTable::getTableName() const {
    return m_sTableName;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppStorageColumnDef> &WsjcppStorageTable::getColumns() const {
    return m_vColumns;
}

// ---------------------------------------------------------------------

void WsjcppStorageTable::mergeWith(WsjcppStorageModifyTable &modifyTable) {

    // TODO check indexes
    std::vector<std::string> vDropColumns = modifyTable.getDropColumns();
    for (int i = 0; i < vDropColumns.size(); i++) {
        bool bFound = false;
        std::string sColumnName = vDropColumns[i];
        for (int i = 0; i < m_vColumns.size(); i++) {
            if (m_vColumns[i].columnName() == sColumnName) {
                m_vColumns.erase(m_vColumns.begin() + i);
                bFound = true;
                break;
            }
        }
        if (!bFound) {
            WsjcppLog::throw_err(TAG, "Not found column " + sColumnName + " in table " + m_sTableName);
        }
    }

    std::vector<WsjcppStorageColumnDef> vAddColumns = modifyTable.getAddColumns();
    for (int i = 0; i < vAddColumns.size(); i++) {
        bool bFound = false;
        WsjcppStorageColumnDef c = vAddColumns[i];
        for (int i = 0; i < m_vColumns.size(); i++) {
            if (m_vColumns[i].columnName() == c.columnName()) {
                bFound = true;
            }
        }
        if (bFound) {
            WsjcppLog::throw_err(TAG, "Column already defined " + c.columnName() + " in table " + m_sTableName);
        } else {
            m_vColumns.push_back(c);
        }
    }

    // TODO check indexes
    std::vector<WsjcppStorageColumnDef> vAlterColumns = modifyTable.getAlterColumns();
    for (int i = 0; i < vAlterColumns.size(); i++) {
        bool bFound = false;
        WsjcppStorageColumnDef c = vAlterColumns[i];
        for (int i = 0; i < m_vColumns.size(); i++) {
            if (m_vColumns[i].columnName() == c.columnName()) {
                if (m_vColumns[i].isEnableIndex()) {
                    c.enableIndex();
                }
                m_vColumns[i] = c;
                bFound = true;
            }
        }
        if (!bFound) {
            WsjcppLog::throw_err(TAG, "Problem with alter column '" + c.columnName() + "'. It's not defined in table '" + m_sTableName + "'");
        }
    }
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageConnection
// ***********************

WsjcppStorageConnection::WsjcppStorageConnection() {
    m_nCreated = WsjcppCore::currentTime_milliseconds();
    TAG = "WsjcppStorageConnection";
}

WsjcppStorageConnection::~WsjcppStorageConnection() {
    // Nothing
}

// ---------------------------------------------------------------------

long WsjcppStorageConnection::created() { // TODO 
    return m_nCreated;
}

// ---------------------------------------------------------------------

long WsjcppStorageConnection::getConnectionDurationInSeconds() {
    long nRet = WsjcppCore::currentTime_milliseconds() - m_nCreated;
    nRet = nRet / 1000;
    return nRet;
}

// ---------------------------------------------------------------------

// ***********************
// *** Storage
// ***********************

WsjcppStorage::WsjcppStorage() {
    TAG = "WsjcppStorage";
}

// ---------------------------------------------------------------------

bool WsjcppStorage::addStorageChanges(WsjcppStorageChanges &storageChanges) {
    std::string sTableName = storageChanges.getTableName();
    std::map<std::string, WsjcppStorageTable>::iterator it = m_mapTables.find(sTableName);

    if (storageChanges.getType() == WSJCPP_STORAGE_CHANGES_TYPE_CREATE_TABLE) {
        if (it != m_mapTables.end()) {
            WsjcppLog::err(TAG, "Table '" + sTableName + "' already defined");
            WsjcppLog::warn(TAG, "TODO need drop table");
            return false;
        }
        WsjcppStorageTable tbl((WsjcppStorageCreateTable &)storageChanges);
        m_mapTables.insert(std::pair<std::string,WsjcppStorageTable>(sTableName,tbl) );
    } else if (storageChanges.getType() == WSJCPP_STORAGE_CHANGES_TYPE_DROP_TABLE) {
        if (it == m_mapTables.end()) {
            WsjcppLog::throw_err(TAG, "Not found table '" + sTableName + "'");
        }
        m_mapTables.erase(sTableName);
    } else if (storageChanges.getType() == WSJCPP_STORAGE_CHANGES_TYPE_MODIFY_TABLE) {
        if (it == m_mapTables.end()) {
            WsjcppLog::throw_err(TAG, "Not found table '" + sTableName + "'");
        }
        it->second.mergeWith((WsjcppStorageModifyTable &)storageChanges);
    } else if (storageChanges.getType() == WSJCPP_STORAGE_CHANGES_TYPE_INSERT_ROW) {
        // skip
    } else {
        WsjcppLog::throw_err(TAG, "addStorageChanges, Unknown operation with table");
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppStorage::executeStorageChanges(WsjcppStorageConnection *pConn, WsjcppStorageChanges &storageChanges) {
    std::string sTableName = storageChanges.getTableName();
    std::vector<std::string> vQueries;

    if (storageChanges.getType() == WSJCPP_STORAGE_CHANGES_TYPE_CREATE_TABLE) {
        WsjcppStorageCreateTable createTable = (WsjcppStorageCreateTable &)storageChanges;
        vQueries = this->prepareSqlQueries(createTable);
    } else if (storageChanges.getType() == WSJCPP_STORAGE_CHANGES_TYPE_MODIFY_TABLE) {
        WsjcppStorageModifyTable modifyTable = (WsjcppStorageModifyTable &)storageChanges;
        vQueries = this->prepareSqlQueries(modifyTable);
    } else if (storageChanges.getType() == WSJCPP_STORAGE_CHANGES_TYPE_DROP_TABLE) {
        WsjcppStorageDropTable dropTable = (WsjcppStorageDropTable &)storageChanges;
        vQueries = this->prepareSqlQueries(dropTable);
    } else if (storageChanges.getType() == WSJCPP_STORAGE_CHANGES_TYPE_INSERT_ROW) {
        WsjcppStorageInsert insRow = (WsjcppStorageInsert &)storageChanges;
        WsjcppStorageTable tableDef = getTableDef(insRow.getTableName());
        if (!insRow.isValid(tableDef)) {
            WsjcppLog::throw_err(TAG, "Insert into table '" + insRow.getTableName() + "' is invalid");
        }
        vQueries = this->prepareSqlQueries(insRow);
    } else {
        throw std::runtime_error("Could not support type of StorageChanges");
    }

    WsjcppLog::info(TAG, storageChanges.getStartApply());
    for (int i = 0; i < vQueries.size(); i++) {
        std::string sQuery = vQueries[i];
        if (!pConn->executeQuery(sQuery)) {
            WsjcppLog::throw_err(TAG, storageChanges.getAppliedFailed() + "\n    query -> " + sQuery);
            return false;
        }
    }

    if (!this->addStorageChanges(storageChanges)) {
        WsjcppLog::throw_err(TAG, storageChanges.getAppliedFailed());
        return false;
    }
    WsjcppLog::ok(TAG, storageChanges.getAppliedSuccess());
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppStorage::insertRow(WsjcppStorageConnection *pConn, const WsjcppStorageInsert &storageInsert) {

    std::string sTableName = storageInsert.getTableName();
    WsjcppStorageTable tableDef = getTableDef(sTableName);

    if (!storageInsert.isValid(tableDef)) {
        return false;
    }

    std::vector<std::string> v = this->prepareSqlQueries(storageInsert);
    for (int i = 0; i < v.size(); i++) {
        std::string sQuery = v[i];
        if (!pConn->executeQuery(sQuery)) {
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------

const std::map<std::string, WsjcppStorageTable> &WsjcppStorage::getTables() {
    return m_mapTables;
}

// ---------------------------------------------------------------------

bool WsjcppStorage::existsTable(const std::string &sTableName) {
    std::map<std::string, WsjcppStorageTable>::iterator it = m_mapTables.find(sTableName);
    return (it != m_mapTables.end());
}

// ---------------------------------------------------------------------

const WsjcppStorageTable &WsjcppStorage::getTableDef(const std::string &sTableName) {
    std::map<std::string, WsjcppStorageTable>::iterator it = m_mapTables.find(sTableName);
    if (it == m_mapTables.end()) {
        WsjcppLog::throw_err(TAG, "Table " + sTableName + " does not exists");
    }
    return it->second; 
}

// ---------------------------------------------------------------------

std::map<std::string, IFabricWsjcppStorage*> *g_pFabricWsjcppStorages = NULL;

// ---------------------------------------------------------------------

void WsjcppStorages::initGlobalVariables() {
    if (g_pFabricWsjcppStorages == NULL) {
        // WsjcppLog::info(std::string(), "Create employees map");
        g_pFabricWsjcppStorages = new std::map<std::string, IFabricWsjcppStorage*>();
    }
}

// ---------------------------------------------------------------------

void WsjcppStorages::add(const std::string &sType, IFabricWsjcppStorage* pStorage) {
    WsjcppStorages::initGlobalVariables();
    const std::string TAG = "addStorage";
    if (g_pFabricWsjcppStorages->count(sType) > 0) {
        WsjcppLog::err(TAG, sType + " - storage already registered");
    } else {
        g_pFabricWsjcppStorages->insert(std::pair<std::string, IFabricWsjcppStorage*>(sType,pStorage));
        // WsjcppLog::info(sType, "Registered");
    }
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppStorages::list() {
    WsjcppStorages::initGlobalVariables();
    std::vector<std::string> vResult;
    std::map<std::string, IFabricWsjcppStorage*>::iterator it;
    for (it = g_pFabricWsjcppStorages->begin(); it != g_pFabricWsjcppStorages->end(); it++) {
        vResult.push_back(it->first);
    }
    return vResult;
}

// ---------------------------------------------------------------------

bool WsjcppStorages::support(const std::string &sType) {
    WsjcppStorages::initGlobalVariables();
    return g_pFabricWsjcppStorages->count(sType) != 0;
}

// ---------------------------------------------------------------------

WsjcppStorage* WsjcppStorages::create(const std::string &sType) {
    WsjcppStorages::initGlobalVariables();
    std::string TAG = "create";
    IFabricWsjcppStorage *pFabricWsjcppStorage = nullptr;
    if (g_pFabricWsjcppStorages->count(sType)) {
        pFabricWsjcppStorage = g_pFabricWsjcppStorages->at(sType);
        return pFabricWsjcppStorage->create();
    }
    WsjcppLog::throw_err(TAG, "Not found storage by name '" + sType + "'");
    return nullptr;
}

// ---------------------------------------------------------------------
// class StorageUpdateBase

WsjcppStorageUpdateBase::WsjcppStorageUpdateBase(const std::string &sFromVersion, const std::string &sVersion, const std::string &sDescription) {
    TAG = "Update_" + sVersion;
    m_sFromVersion = sFromVersion;
    m_sVersion = sVersion;
    m_sDescription = sDescription;
    
    if (sFromVersion != "") {
        WsjcppStorageUpdateBase *pUpdate = WsjcppStorageUpdates::findUpdateVersion(sFromVersion);
        if (pUpdate == nullptr) {
            WsjcppLog::throw_err(TAG, "Not found storage update version: '" + sFromVersion + "'");
        }
    }

    {
        WsjcppStorageUpdateBase *pUpdate = WsjcppStorageUpdates::findUpdateVersion(sVersion);
        if (pUpdate != nullptr) {
            WsjcppLog::throw_err(TAG, "Storage updates already has update with this version: '" + sVersion + "'");
        }
    }
    
    WsjcppStorageUpdates::initGlobalVariables();
    g_pWsjcppStorageUpdates->push_back(this);
}

// ---------------------------------------------------------------------

const std::string &WsjcppStorageUpdateBase::from_version() {
    return m_sFromVersion;
}

// ---------------------------------------------------------------------

const std::string &WsjcppStorageUpdateBase::version() {
    return m_sVersion;
}

// ---------------------------------------------------------------------

const std::string &WsjcppStorageUpdateBase::description() {
    return m_sDescription;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppStorageChanges *> &WsjcppStorageUpdateBase::getChanges() {
    return m_vStorageChanges;
}

// ---------------------------------------------------------------------

void WsjcppStorageUpdateBase::setWeight(int nWeight) {
    m_nWeight = nWeight;
}

// ---------------------------------------------------------------------

int WsjcppStorageUpdateBase::getWeight() {
    return m_nWeight;
}

// ---------------------------------------------------------------------

WsjcppStorageCreateTable *WsjcppStorageUpdateBase::createTable(std::string sTableName) {
    checkTableName(sTableName);
    WsjcppStorageCreateTable *pCreateTable = new WsjcppStorageCreateTable(sTableName);
    m_vStorageChanges.push_back(pCreateTable);
    return pCreateTable;
}

// ---------------------------------------------------------------------

WsjcppStorageModifyTable *WsjcppStorageUpdateBase::modifyTable(std::string sTableName) {
    checkTableName(sTableName);
    WsjcppStorageModifyTable *pModifyTable = new WsjcppStorageModifyTable(sTableName);
    m_vStorageChanges.push_back(pModifyTable);
    return pModifyTable;
}

// ---------------------------------------------------------------------

WsjcppStorageDropTable *WsjcppStorageUpdateBase::dropTable(std::string sTableName) {
    checkTableName(sTableName);
    WsjcppStorageDropTable *pDropTable = new WsjcppStorageDropTable(sTableName);
    m_vStorageChanges.push_back(pDropTable);
    return pDropTable;
}

WsjcppStorageInsert *WsjcppStorageUpdateBase::insertIntoTable(std::string sTableName) {
    WsjcppStorageInsert *pInsert = new WsjcppStorageInsert(sTableName);
    m_vStorageChanges.push_back(pInsert);
    return pInsert;
}

// ---------------------------------------------------------------------

void WsjcppStorageUpdateBase::checkTableName(std::string sTableName) {
    // TODO check table name + runtime error
}

// ---------------------------------------------------------------------
// StorageUpdates

std::vector<WsjcppStorageUpdateBase *> *g_pWsjcppStorageUpdates = nullptr;

// ---------------------------------------------------------------------

void WsjcppStorageUpdates::initGlobalVariables() {
    if (g_pWsjcppStorageUpdates == NULL) {
        // WsjcppLog::info(std::string(), "Create list updates");
        g_pWsjcppStorageUpdates = new std::vector<WsjcppStorageUpdateBase *>();
    }
}

// ---------------------------------------------------------------------

int WsjcppStorageUpdates::calculateWeight(int nWeight, const std::string &sVersion) {
    int nRet = nWeight;
    WsjcppStorageUpdateBase *pCurrentUpdate = WsjcppStorageUpdates::findUpdateVersion(sVersion);
    if (pCurrentUpdate == nullptr) {
        return nRet;
    }
    nRet++;
    std::vector<WsjcppStorageUpdateBase *> vChildUpdates;
    for (int i = 0; i < g_pWsjcppStorageUpdates->size(); i++) {
        WsjcppStorageUpdateBase *pUpdate = g_pWsjcppStorageUpdates->at(i);
        if (sVersion == pUpdate->from_version()) {
            nRet = calculateWeight(nRet, pUpdate->version());
        }
    }
    return nRet;
}

// ---------------------------------------------------------------------

void WsjcppStorageUpdates::sortByWeight(std::vector<WsjcppStorageUpdateBase*> &vUpdates) {
    if (vUpdates.size() <= 1) {
        return;
    }
    for (int i = 0; i < vUpdates.size(); i++) {
        int nWeight = calculateWeight(0, vUpdates[i]->version());
        vUpdates[i]->setWeight(nWeight);
    }

    bool bSorted = false;
    while (!bSorted) {
        bSorted = true;
        for (int i = 0; i < vUpdates.size()-1; i++) {
            if (vUpdates[i]->getWeight() > vUpdates[i+1]->getWeight()) {
                WsjcppStorageUpdateBase *p = vUpdates[i];
                vUpdates[i] = vUpdates[i+1];
                vUpdates[i+1] = p;
                bSorted = false;
            }
        }
    }
}

// ---------------------------------------------------------------------

std::vector<WsjcppStorageUpdateBase *> WsjcppStorageUpdates::findUpdatesFromVersion(const std::string &sFromVersion) {
    std::vector<WsjcppStorageUpdateBase *> vFoundUpdates;
    for (int i = 0; i < g_pWsjcppStorageUpdates->size(); i++) {
        WsjcppStorageUpdateBase * pUpdate = g_pWsjcppStorageUpdates->at(i);
        if (pUpdate->from_version() == sFromVersion) {
            vFoundUpdates.push_back(pUpdate);
        }
    }
    return vFoundUpdates;
}

// ---------------------------------------------------------------------

void WsjcppStorageUpdates::pushUpdatesFromVersion(std::vector<WsjcppStorageUpdateBase *> &vSortedUpdates, const std::string &sFromVersion) {
    std::vector<WsjcppStorageUpdateBase *> vFoundUpdates = findUpdatesFromVersion(sFromVersion);
    sortByWeight(vFoundUpdates);
    // std::cout << std::to_string(vFoundUpdates.size()) << std::endl;
    if (vFoundUpdates.size() == 0) {
        return;
    }

    for (int i = 0; i < vFoundUpdates.size(); i++) {
        vSortedUpdates.push_back(vFoundUpdates[i]);
        pushUpdatesFromVersion(vSortedUpdates, vFoundUpdates[i]->version());
    }
}

// ---------------------------------------------------------------------

std::vector<WsjcppStorageUpdateBase *> WsjcppStorageUpdates::getSortedStorageUpdates() {
    WsjcppStorageUpdates::initGlobalVariables();
    // calculate weights
    std::vector<WsjcppStorageUpdateBase *> vSortedUpdates;
    pushUpdatesFromVersion(vSortedUpdates, "");
    return vSortedUpdates;
}

// ---------------------------------------------------------------------

WsjcppStorageUpdateBase* WsjcppStorageUpdates::findUpdateFromVersion(const std::string &sFromVersion) {
    WsjcppStorageUpdates::initGlobalVariables();
    std::string TAG = "StorageUpdates::findUpdateFromVersion";
    WsjcppStorageUpdateBase* pRet = nullptr;
    for (int i = 0; i < g_pWsjcppStorageUpdates->size(); i++) {
        WsjcppStorageUpdateBase* pUpdate = g_pWsjcppStorageUpdates->at(i);
        if (sFromVersion == pUpdate->from_version()) {
            if (pRet == nullptr) {
                pRet = pUpdate;
            } else {
                WsjcppLog::throw_err(TAG, "Already defined update with from_version " + sFromVersion);
            }
        }
    }
    return pRet;
}

// ---------------------------------------------------------------------

WsjcppStorageUpdateBase* WsjcppStorageUpdates::findUpdateVersion(const std::string &sVersion) {
    WsjcppStorageUpdates::initGlobalVariables();
    std::string TAG = "WsjcppStorageUpdates::findUpdateVersion";
    WsjcppStorageUpdateBase* pRet = nullptr;
    for (int i = 0; i < g_pWsjcppStorageUpdates->size(); i++) {
        WsjcppStorageUpdateBase* pUpdate = g_pWsjcppStorageUpdates->at(i);
        if (sVersion == pUpdate->version()) {
            if (pRet == nullptr) {
                pRet = pUpdate;
            } else {
                WsjcppLog::throw_err(TAG, "Already defined update with version " + sVersion);
            }
        }
    }
    return pRet;
}

// ---------------------------------------------------------------------
/*
bool StorageUpdates::apply(Storage *pStorage) {
    StorageUpdates::initGlobalVariables();
    std::string TAG = "StorageUpdates::apply";
    StorageConnection *pConn = pStorage->connect();
    if (pConn == nullptr) {
        WsjcppLog::err(TAG, "Could not connect to database");
        return false;
    } else {
        WsjcppLog::ok(TAG, "Successfully connection to database");
    }
    std::string sFirstVersion = "";
    std::string sLastVersion = pConn->lastDatabaseVersion();
    WsjcppLog::info(TAG, "Last Version -> '" + sLastVersion + "'");
    // restore struct of storage before sLastVersion
    bool bHasUpdates = true;
    while (bHasUpdates) {
        bHasUpdates = false;
        StorageUpdateBase* pUpdate = StorageUpdates::findUpdateFromVersion(sFirstVersion);
        if (pUpdate != nullptr) {
            if (sFirstVersion != sLastVersion) {
                sFirstVersion = pUpdate->version();
                bHasUpdates = true;
                std::string error = "";
                WsjcppLog::info(TAG, "Apply changes '" + pUpdate->from_version() + "' -> '" + pUpdate->version() + "'");
                // Apply changes
                std::vector<StorageChanges *> vStorageChanges = pUpdate->getChanges();
                for (int i = 0; i < vStorageChanges.size(); i++) {
                    StorageChanges *pChanges = vStorageChanges[i];
                    if (pChanges->getType() == WsjcppStorageChangesType::NOPE) {
                        std::string sError = "Not allowed use a WsjcppStorageChangesType::NOPE";
                        WsjcppLog::throw_err(TAG, sError);
                    }
                    if (!pStorage->addStorageChanges(*pChanges)) {
                        error = "Problem add storage changes '" + pChanges->getTableName() + "' in version " + pUpdate->version();
                        delete pConn;
                        return false;
                    }
                }
            }
        }
    }
    // apply new updates
    WsjcppLog::info(TAG, "apply new updates");
    bHasUpdates = true;
    while (bHasUpdates) {
        bHasUpdates = false;
        StorageUpdateBase* pUpdate = StorageUpdates::findUpdateFromVersion(sLastVersion);
        if (pUpdate != nullptr) {
            WsjcppLog::info(TAG, "Installing update '" + pUpdate->from_version() + "' -> '" + pUpdate->version() + "': " + pUpdate->description());
            sLastVersion = pUpdate->version();
            bHasUpdates = true;
            std::string error = "";
            // Apply changes
            std::vector<StorageChanges *> vStorageChanges = pUpdate->getChanges();
            for (int i = 0; i < vStorageChanges.size(); i++) {
                StorageChanges *pChanges = vStorageChanges[i];
                if (pChanges->getType() == WsjcppStorageChangesType::NOPE) {
                    std::string sError = "Not allowed use a WsjcppStorageChangesType::NOPE";
                    WsjcppLog::err(TAG, sError);
                    throw std::runtime_error(sError);
                }
                if (!pStorage->executeStorageChanges(pConn, *pChanges)) {
                    error = "Problem with table '" + pChanges->getTableName() + "' in version " + pUpdate->version();
                    delete pConn;
                    return false;
                }
            }
            pConn->insertUpdateInfo(pUpdate->version(), pUpdate->description());
        }
    }
    return true;
}
*/
// ---------------------------------------------------------------------

void WsjcppStorageUpdates::applyAllStorageChanges(WsjcppStorage *pStorage, WsjcppStorageUpdateBase *pUpdate) {
    std::string TAG = "StorageUpdates::applyAllStorageChanges";
    WsjcppLog::info(TAG, "Apply changes '" + pUpdate->from_version() + "' -> '" + pUpdate->version() + "'");

    // Apply changes
    std::vector<WsjcppStorageChanges *> vStorageChanges = pUpdate->getChanges();
    for (int i = 0; i < vStorageChanges.size(); i++) {
        WsjcppStorageChanges *pChanges = vStorageChanges[i];
        if (pChanges->getType() == WSJCPP_STORAGE_CHANGES_TYPE_NOPE) {
            WsjcppLog::throw_err(TAG, "Not allowed use a WSJCPP_STORAGE_CHANGES_TYPE_NOPE");
        }

        if (!pStorage->addStorageChanges(*pChanges)) {
            WsjcppLog::throw_err(TAG,"Problem add storage changes '" + pChanges->getTableName() + "' in version " + pUpdate->version());
        }
    }
}

// ---------------------------------------------------------------------

void WsjcppStorageUpdates::executeAllStorageChanges(WsjcppStorage *pStorage, WsjcppStorageConnection *pConn, WsjcppStorageUpdateBase *pUpdate) {
    std::string TAG = "StorageUpdates::executeAllStorageChanges";
    WsjcppLog::info(TAG, "Installing update '" + pUpdate->from_version() + "' -> '" + pUpdate->version() + "': " + pUpdate->description());

    // Apply changes
    std::vector<WsjcppStorageChanges *> vStorageChanges = pUpdate->getChanges();
    for (int i = 0; i < vStorageChanges.size(); i++) {
        WsjcppStorageChanges *pChanges = vStorageChanges[i];
        if (pChanges->getType() == WSJCPP_STORAGE_CHANGES_TYPE_NOPE) {
            WsjcppLog::throw_err(TAG, "Not allowed use a WSJCPP_STORAGE_CHANGES_TYPE_NOPE");
        }
        if (!pStorage->executeStorageChanges(pConn, *pChanges)) {
            WsjcppLog::throw_err(TAG, "Problem with table '" + pChanges->getTableName() + "' in version " + pUpdate->version());
        }
    }
    pConn->insertUpdateInfo(pUpdate->version(), pUpdate->description());
}

// ---------------------------------------------------------------------

bool WsjcppStorageUpdates::apply2(WsjcppStorage *pStorage) {
    WsjcppStorageUpdates::initGlobalVariables();
    std::string TAG = "WsjcppStorageUpdates::apply2";
    WsjcppStorageConnection *pConn = pStorage->connect();

    std::vector<std::string> vVersions = pConn->getInstalledVersions();
    std::vector<WsjcppStorageUpdateBase*> vUpdates = getSortedStorageUpdates();

    for (int i = 0; i < vUpdates.size(); i++) {
        std::string sVersion = vUpdates[i]->version();
        WsjcppStorageUpdateBase* pUpdate = vUpdates[i];
        bool bUpdatedAlreadyInstalled = std::find(vVersions.begin(), vVersions.end(), sVersion) != vVersions.end();

        if (bUpdatedAlreadyInstalled) {
            applyAllStorageChanges(pStorage, pUpdate);
        } else {
            executeAllStorageChanges(pStorage, pConn, pUpdate);
        }            
    }
    return true;
}