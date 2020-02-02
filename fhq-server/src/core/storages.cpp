#include <core/storages.h>
#include <algorithm>
#include <core/fallen.h>

// ***********************
// *** StorageColumnDef
// ***********************

// ---------------------------------------------------------------------

StorageColumnDef::StorageColumnDef(const std::string &sColumnName) {
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

StorageColumnDef &StorageColumnDef::autoIncrement() {
    m_bAutoIncrement = true;
    return *this;
}

// ---------------------------------------------------------------------

StorageColumnDef &StorageColumnDef::notNull() {
    m_bNotNull = true;
    return *this;
}

// ---------------------------------------------------------------------

StorageColumnDef &StorageColumnDef::string(int nValue) {
    if (m_sType != "") {
        WSJCppLog::throw_err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "string";
    m_nTypeSize = nValue;
    return *this;
}

// ---------------------------------------------------------------------

StorageColumnDef &StorageColumnDef::text() {
    if (m_sType != "") {
        WSJCppLog::throw_err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "text";
    return *this;
}

// ---------------------------------------------------------------------

StorageColumnDef &StorageColumnDef::datetime() {
    if (m_sType != "") {
        WSJCppLog::throw_err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "datetime";
    return *this;
}

// ---------------------------------------------------------------------

StorageColumnDef &StorageColumnDef::number() {
    if (m_sType != "") {
        WSJCppLog::throw_err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "number";
    return *this;
}

// ---------------------------------------------------------------------

StorageColumnDef &StorageColumnDef::doubleNumber() {
    if (m_sType != "") {
        WSJCppLog::throw_err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "doubleNumber";
    return *this;
}


// ---------------------------------------------------------------------

StorageColumnDef &StorageColumnDef::primaryKey() {
    m_bPrimaryKey = true;
    return *this;
}

// ---------------------------------------------------------------------

StorageColumnDef &StorageColumnDef::defaultValue(const std::string& sDefaultValue) {
    m_sDefaultValue = sDefaultValue;
    m_bDefaultValue = true;
    return *this;
}

// ---------------------------------------------------------------------

StorageColumnDef &StorageColumnDef::enableIndex() {
    m_bEnableIndex = true;
    return *this;
}

// ---------------------------------------------------------------------

StorageColumnDef &StorageColumnDef::enableUniqueIndex(const std::string& sIndexName) {
    m_sNameOfUniqueIndex = sIndexName;
    m_bEnableUniqueIndex = true;
    return *this;
}

// ---------------------------------------------------------------------

std::string StorageColumnDef::columnName() const {
    return m_sColumnName;
}

// ---------------------------------------------------------------------

std::string StorageColumnDef::columnType() {
    return m_sType;
}

// ---------------------------------------------------------------------

std::string StorageColumnDef::columnDefaultValue() {
    return m_sDefaultValue;
}

// ---------------------------------------------------------------------

bool StorageColumnDef::isDefaultValue() {
    return m_bDefaultValue;
}

// ---------------------------------------------------------------------

int StorageColumnDef::columnTypeSize() {
    return m_nTypeSize;
}

// ---------------------------------------------------------------------

bool StorageColumnDef::isAutoIncrement() {
    return m_bAutoIncrement;
}

// ---------------------------------------------------------------------

bool StorageColumnDef::isPrimaryKey() {
    return m_bPrimaryKey;
}

// ---------------------------------------------------------------------

bool StorageColumnDef::isNotNull() {
    return m_bNotNull;
}

// ---------------------------------------------------------------------

bool StorageColumnDef::isEnableIndex() {
    return m_bEnableIndex;
}

// ---------------------------------------------------------------------

bool StorageColumnDef::isEnableUniqueIndex() {
    return m_bEnableUniqueIndex;
}

// ---------------------------------------------------------------------

std::string StorageColumnDef::nameOfUniqueIndex() {
    return m_sNameOfUniqueIndex;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageChanges
// ***********************

// ---------------------------------------------------------------------

StorageChanges::StorageChanges(const std::string &sTableName) {
    TAG = "StorageChanges";
    m_sTableName = sTableName;
}

// ---------------------------------------------------------------------

std::string StorageChanges::getTableName() const {
    return m_sTableName;
}

// ---------------------------------------------------------------------

StorageChangesType StorageChanges::getType() const {
    return StorageChangesType::NOPE;
}

// ---------------------------------------------------------------------

std::string StorageChanges::getStartApply() const {
    return "Appling changes for table " + m_sTableName;
}

// ---------------------------------------------------------------------

std::string StorageChanges::getAppliedSuccess() const {
    return "Changes success applied for table " + m_sTableName;
}

// ---------------------------------------------------------------------

std::string StorageChanges::getAppliedFailed() const {
    return "Could not apply changes for table " + m_sTableName;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageCreateTable
// ***********************

StorageCreateTable::StorageCreateTable(const std::string &sTableName) 
    : StorageChanges(sTableName) {
    TAG = "StorageCreateTable";
    // TODO validate table of name
}

// ---------------------------------------------------------------------

StorageChangesType StorageCreateTable::getType() const {
    return StorageChangesType::CREATE_TABLE;
};

// ---------------------------------------------------------------------

std::string StorageCreateTable::getStartApply() const {
    return "Creating table '" + m_sTableName + "'";
}

// ---------------------------------------------------------------------

std::string StorageCreateTable::getAppliedSuccess() const {
    return "Created table '" +  m_sTableName + "'";
}

// ---------------------------------------------------------------------

std::string StorageCreateTable::getAppliedFailed() const {
    return "Could not create table '" +  m_sTableName + "'";
}

// ---------------------------------------------------------------------

StorageColumnDef &StorageCreateTable::addColumn(const std::string &sColumnName) {
    for (int i = 0; i < m_vColumns.size(); i++) {
        if (m_vColumns[i].columnName() == sColumnName) {
            std::string sError = "Column '" + sColumnName + "' in table '" + m_sTableName + "' already exists";
            WSJCppLog::err(TAG, sError);
            throw std::runtime_error(sError);
        }
    }
    m_vColumns.push_back(StorageColumnDef(sColumnName));
    return m_vColumns[m_vColumns.size()-1];
}

// ---------------------------------------------------------------------

const std::vector<StorageColumnDef> &StorageCreateTable::getColumns() const {
    return m_vColumns;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageModifyTable
// ***********************

StorageModifyTable::StorageModifyTable(const std::string &sTableName) 
    : StorageChanges(sTableName) {
    TAG = "StorageModifyTable";
    // TODO validate table of name
}

// ---------------------------------------------------------------------

StorageChangesType StorageModifyTable::getType() const {
    return StorageChangesType::MODIFY_TABLE;
};

// ---------------------------------------------------------------------

std::string StorageModifyTable::getStartApply() const {
    return "Modifing table " + m_sTableName;
}

// ---------------------------------------------------------------------

std::string StorageModifyTable::getAppliedSuccess() const {
    return "Modified table " +  m_sTableName;
}

// ---------------------------------------------------------------------

std::string StorageModifyTable::getAppliedFailed() const {
    return "Could not modify table " +  m_sTableName;
}

// ---------------------------------------------------------------------

StorageColumnDef &StorageModifyTable::addColumn(const std::string &sColumnName) {
    std::string sError;
    if (isColumnDefined(sColumnName, sError)) {
        WSJCppLog::throw_err(TAG, "addColumn, " + sError);
    }
    m_vAddColumns.push_back(StorageColumnDef(sColumnName));
    return m_vAddColumns[m_vAddColumns.size()-1];
}

// ---------------------------------------------------------------------

StorageColumnDef &StorageModifyTable::alterColumn(const std::string &sColumnName) {
    std::string sError;
    if (isColumnDefined(sColumnName, sError)) {
        WSJCppLog::throw_err(TAG, "alterColumn, " + sError);
    }
    m_vAlterColumns.push_back(StorageColumnDef(sColumnName));
    return m_vAlterColumns[m_vAlterColumns.size()-1];
}

// ---------------------------------------------------------------------

std::string StorageModifyTable::dropColumn(const std::string &sColumnName) {
    std::string sError;
    if (isColumnDefined(sColumnName, sError)) {
        WSJCppLog::throw_err(TAG, "dropColumn, " + sError);
    }
    m_vDropColumns.push_back(sColumnName);
    return m_vDropColumns[m_vDropColumns.size()-1];
}

// ---------------------------------------------------------------------

const std::vector<StorageColumnDef> &StorageModifyTable::getAddColumns() const {
    return m_vAddColumns;
}

// ---------------------------------------------------------------------

const std::vector<StorageColumnDef> &StorageModifyTable::getAlterColumns() const {
    return m_vAlterColumns;
}

// ---------------------------------------------------------------------

const std::vector<std::string> &StorageModifyTable::getDropColumns() const {
    return m_vDropColumns;
}

// ---------------------------------------------------------------------

bool StorageModifyTable::isColumnDefined(const std::string &sColumnName, std::string &sError) const {
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

StorageDropTable::StorageDropTable(const std::string &sTableName) 
    : StorageChanges(sTableName) {
    TAG = "StorageDropTable";
    // TODO validate table of name
}

// ---------------------------------------------------------------------

StorageChangesType StorageDropTable::getType() const {
    return StorageChangesType::DROP_TABLE;
};

// ---------------------------------------------------------------------

std::string StorageDropTable::getStartApply() const {
    return "Dropping table " + m_sTableName;
}

// ---------------------------------------------------------------------

std::string StorageDropTable::getAppliedSuccess() const {
    return "Dropped table " +  m_sTableName;
}

// ---------------------------------------------------------------------

std::string StorageDropTable::getAppliedFailed() const {
    return "Could not drop table " +  m_sTableName;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageColumnValue
// ***********************

StorageColumnValue::StorageColumnValue(const std::string &sColumnName, StorageColumnType nType) {
    m_sColumnName = sColumnName;
    m_nColumnType = nType;
}

// ---------------------------------------------------------------------

void StorageColumnValue::setValue(std::string sValue) {
    m_sStringValue = sValue;
}

// ---------------------------------------------------------------------

void StorageColumnValue::setValue(int nValue) {
    m_nIntValue = nValue;
}

// ---------------------------------------------------------------------

void StorageColumnValue::setValue(double nValue) {
    m_nDoubleValue = nValue;
}

// ---------------------------------------------------------------------

std::string StorageColumnValue::getColumnName() {
    return m_sColumnName;
}

// ---------------------------------------------------------------------

StorageColumnType StorageColumnValue::getColumnType() {
    return m_nColumnType;
}

// ---------------------------------------------------------------------

std::string StorageColumnValue::getString() {
    return m_sStringValue;
}

// ---------------------------------------------------------------------

int StorageColumnValue::getInt() {
    return m_nIntValue;
}

// ---------------------------------------------------------------------

double StorageColumnValue::getDouble() {
    return m_nDoubleValue;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageInsert
// ***********************

StorageInsert::StorageInsert(const std::string &sTableName)
    : StorageChanges(sTableName) {
    TAG = "StorageInsert";
    m_sTableName = sTableName;
}

// ---------------------------------------------------------------------

StorageChangesType StorageInsert::getType() const {
    return StorageChangesType::INSERT_ROW;
};

// ---------------------------------------------------------------------

std::string StorageInsert::getStartApply() const {
    return "Inserting into table " + m_sTableName;
}

// ---------------------------------------------------------------------

std::string StorageInsert::getAppliedSuccess() const {
    return "Inserted into table " +  m_sTableName;
}

// ---------------------------------------------------------------------

std::string StorageInsert::getAppliedFailed() const {
    return "Could not insert into table " +  m_sTableName;
}

// ---------------------------------------------------------------------

void StorageInsert::bindValue(const std::string &sColumnName, const std::string &sValue) {
    if (this->exists(sColumnName)) {
        WSJCppLog::throw_err(TAG, "Skip. Already defined " + m_sTableName + "." + sColumnName);
        return;
    }
    StorageColumnValue val(sColumnName, StorageColumnType::STRING);
    val.setValue(sValue);
    m_vValues.push_back(val);
}

// ---------------------------------------------------------------------

void StorageInsert::bindValue(const std::string &sColumnName, int nValue) {
    if (this->exists(sColumnName)) {
        WSJCppLog::throw_err(TAG, "Skip. Already defined " + m_sTableName + "." + sColumnName);
        return;
    }
    StorageColumnValue val(sColumnName, StorageColumnType::NUMBER);
    val.setValue(nValue);
    m_vValues.push_back(val);
}

// ---------------------------------------------------------------------

void StorageInsert::bindValue(const std::string &sColumnName, double nValue) {
    if (this->exists(sColumnName)) {
        WSJCppLog::throw_err(TAG, "Skip. Already defined " + m_sTableName + "." + sColumnName);
        return;
    }
    StorageColumnValue val(sColumnName, StorageColumnType::DOUBLE_NUMBER);
    val.setValue(nValue);
    m_vValues.push_back(val);
}

// ---------------------------------------------------------------------

std::vector<StorageColumnValue> StorageInsert::values() const {
    return m_vValues;
}

// ---------------------------------------------------------------------

bool StorageInsert::exists(const std::string &sColumnName) {
    for (int i = 0; i < m_vValues.size(); i++) {
        if (m_vValues[i].getColumnName() == sColumnName) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

bool StorageInsert::isValid(const StorageTable &storageTable) const {
    if (storageTable.getTableName() != m_sTableName) {
        WSJCppLog::err(TAG, "Expeceted '" + m_sTableName + "', but got '" + storageTable.getTableName() + "'");
        return false;
    }

    std::vector<StorageColumnDef> vColumns = storageTable.getColumns();
    std::vector<std::string> vCurrentColumns;
    
    for (int x = 0; x < m_vValues.size(); x++) {
        bool bFound = false;
        StorageColumnValue val = m_vValues[x];
        vCurrentColumns.push_back(val.getColumnName());
        for (int y = 0; y < vColumns.size(); y++) {
            StorageColumnDef st = vColumns[y];
            if (st.columnName() == val.getColumnName()) {
                if (val.getColumnType() == StorageColumnType::STRING
                    && (st.columnType() == "string" || st.columnType() == "datetime" || st.columnType() == "text")) {
                    bFound = true;
                    break;
                } else if (val.getColumnType() == StorageColumnType::NUMBER && st.columnType() == "number") {
                    bFound = true;
                    break;
                } else if (val.getColumnType() == StorageColumnType::DOUBLE_NUMBER && st.columnType() == "doubleNumber") {
                    bFound = true;
                    break;
                } else {
                    WSJCppLog::err(TAG, "StorageInsert::isValid. In struct '" + m_sTableName + "' column '" + val.getColumnName() + "'."
                        " Expected datatype '" + st.columnType() + "', but got '" + std::to_string(val.getColumnType()) + "'");
                    return false;
                }
            }
        }
        if (!bFound) {
            WSJCppLog::err(TAG, "StorageInsert::isValid. Struct '" + m_sTableName + "' has not column '" + val.getColumnName() + "'");
            return false;
        }
    }

    for (int i = 0; i < vColumns.size(); i++) {
        if (!vColumns[i].isAutoIncrement() && vColumns[i].isNotNull()) {
            if (std::find(vCurrentColumns.begin(), vCurrentColumns.end(), vColumns[i].columnName()) == vCurrentColumns.end()) {
                WSJCppLog::err(TAG, "Missing require field '" + m_sTableName + "." + vColumns[i].columnName() + "'");
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
StorageUpdate::StorageUpdate(const StorageStruct &storageStruct) {
    m_storageStruct = storageStruct;
}

// ---------------------------------------------------------------------

void StorageUpdate::bindValue(const std::string &sColumnName, const std::string &sValue) {

}

// ---------------------------------------------------------------------

void StorageUpdate::bindValue(const std::string &sColumnName, int nValue) {

}

// ---------------------------------------------------------------------

void StorageUpdate::bindValue(const std::string &sColumnName, double nValue) {

}

// ---------------------------------------------------------------------

std::vector<StorageColumnValue> StorageUpdate::values() const {

}

// ---------------------------------------------------------------------

bool StorageUpdate::isValid() const {
    return false;
}

// ---------------------------------------------------------------------
*/

// ---------------------------------------------------------------------

StorageTable::StorageTable(const std::string &sTableName) {
    m_sTableName = sTableName;
    TAG = "StorageTable";
}

// ---------------------------------------------------------------------

StorageTable::StorageTable(StorageCreateTable &createTable) {
    m_sTableName = createTable.getTableName();
    const std::vector<StorageColumnDef> &list = createTable.getColumns();
    for (int i = 0; i < list.size(); i++) {
        m_vColumns.push_back(list[i]);
    }
}

// ---------------------------------------------------------------------

std::string StorageTable::getTableName() const {
    return m_sTableName;
}

// ---------------------------------------------------------------------

const std::vector<StorageColumnDef> &StorageTable::getColumns() const {
    return m_vColumns;
}

// ---------------------------------------------------------------------

void StorageTable::mergeWith(StorageModifyTable &modifyTable) {

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
            WSJCppLog::throw_err(TAG, "Not found column " + sColumnName + " in table " + m_sTableName);
        }
    }

    std::vector<StorageColumnDef> vAddColumns = modifyTable.getAddColumns();
    for (int i = 0; i < vAddColumns.size(); i++) {
        bool bFound = false;
        StorageColumnDef c = vAddColumns[i];
        for (int i = 0; i < m_vColumns.size(); i++) {
            if (m_vColumns[i].columnName() == c.columnName()) {
                bFound = true;
            }
        }
        if (bFound) {
            WSJCppLog::throw_err(TAG, "Column already defined " + c.columnName() + " in table " + m_sTableName);
        } else {
            m_vColumns.push_back(c);
        }
    }

    // TODO check indexes
    std::vector<StorageColumnDef> vAlterColumns = modifyTable.getAlterColumns();
    for (int i = 0; i < vAlterColumns.size(); i++) {
        bool bFound = false;
        StorageColumnDef c = vAlterColumns[i];
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
            WSJCppLog::throw_err(TAG, "Problem with alter column '" + c.columnName() + "'. It's not defined in table '" + m_sTableName + "'");
        }
    }
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageConnection
// ***********************

StorageConnection::StorageConnection() {
    m_nCreated = WSJCppCore::currentTime_milliseconds();
    TAG = "StorageConnection";
}

StorageConnection::~StorageConnection() {
    // Nothing
}

// ---------------------------------------------------------------------

long StorageConnection::created() { // TODO 
    return m_nCreated;
}

// ---------------------------------------------------------------------

long StorageConnection::getConnectionDurationInSeconds() {
    long nRet = WSJCppCore::currentTime_milliseconds() - m_nCreated;
    nRet = nRet / 1000;
    return nRet;
}

// ---------------------------------------------------------------------

// ***********************
// *** Storage
// ***********************

Storage::Storage() {
    TAG = "Storage";
}

// ---------------------------------------------------------------------

bool Storage::addStorageChanges(StorageChanges &storageChanges) {
    std::string sTableName = storageChanges.getTableName();
    std::map<std::string,StorageTable>::iterator it = m_mapTables.find(sTableName);

    if (storageChanges.getType() == StorageChangesType::CREATE_TABLE) {
        if (it != m_mapTables.end()) {
            WSJCppLog::err(TAG, "Table '" + sTableName + "' already defined");
            WSJCppLog::warn(TAG, "TODO need drop table");
            return false;
        }
        StorageTable tbl((StorageCreateTable &)storageChanges);
        m_mapTables.insert(std::pair<std::string,StorageTable>(sTableName,tbl) );
    } else if (storageChanges.getType() == StorageChangesType::DROP_TABLE) {
        if (it == m_mapTables.end()) {
            WSJCppLog::throw_err(TAG, "Not found table '" + sTableName + "'");
        }
        m_mapTables.erase(sTableName);
    } else if (storageChanges.getType() == StorageChangesType::MODIFY_TABLE) {
        if (it == m_mapTables.end()) {
            WSJCppLog::throw_err(TAG, "Not found table '" + sTableName + "'");
        }
        it->second.mergeWith((StorageModifyTable &)storageChanges);
    } else if (storageChanges.getType() == StorageChangesType::INSERT_ROW) {
        // skip
    } else {
        WSJCppLog::throw_err(TAG, "addStorageChanges, Unknown operation with table");
    }
    return true;
}

// ---------------------------------------------------------------------

bool Storage::executeStorageChanges(StorageConnection *pConn, StorageChanges &storageChanges) {
    std::string sTableName = storageChanges.getTableName();
    std::vector<std::string> vQueries;

    if (storageChanges.getType() == StorageChangesType::CREATE_TABLE) {
        StorageCreateTable createTable = (StorageCreateTable &)storageChanges;
        vQueries = this->prepareSqlQueries(createTable);
    } else if (storageChanges.getType() == StorageChangesType::MODIFY_TABLE) {
        StorageModifyTable modifyTable = (StorageModifyTable &)storageChanges;
        vQueries = this->prepareSqlQueries(modifyTable);
    } else if (storageChanges.getType() == StorageChangesType::DROP_TABLE) {
        StorageDropTable dropTable = (StorageDropTable &)storageChanges;
        vQueries = this->prepareSqlQueries(dropTable);
    } else if (storageChanges.getType() == StorageChangesType::INSERT_ROW) {
        StorageInsert insRow = (StorageInsert &)storageChanges;
        StorageTable tableDef = getTableDef(insRow.getTableName());
        if (!insRow.isValid(tableDef)) {
            WSJCppLog::throw_err(TAG, "Insert into table '" + insRow.getTableName() + "' is invalid");
        }
        vQueries = this->prepareSqlQueries(insRow);
    } else {
        throw std::runtime_error("Could not support type of StorageChanges");
    }

    WSJCppLog::info(TAG, storageChanges.getStartApply());
    for (int i = 0; i < vQueries.size(); i++) {
        std::string sQuery = vQueries[i];
        if (!pConn->executeQuery(sQuery)) {
            WSJCppLog::throw_err(TAG, storageChanges.getAppliedFailed() + "\n    query -> " + sQuery);
            return false;
        }
    }

    if (!this->addStorageChanges(storageChanges)) {
        WSJCppLog::throw_err(TAG, storageChanges.getAppliedFailed());
        return false;
    }
    WSJCppLog::ok(TAG, storageChanges.getAppliedSuccess());
    return true;
}

// ---------------------------------------------------------------------

bool Storage::insertRow(StorageConnection *pConn, const StorageInsert &storageInsert) {

    std::string sTableName = storageInsert.getTableName();
    StorageTable tableDef = getTableDef(sTableName);

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

const std::map<std::string, StorageTable> &Storage::getTables() {
    return m_mapTables;
}

// ---------------------------------------------------------------------

bool Storage::existsTable(const std::string &sTableName) {
    std::map<std::string, StorageTable>::iterator it = m_mapTables.find(sTableName);
    return (it != m_mapTables.end());
}

// ---------------------------------------------------------------------

const StorageTable &Storage::getTableDef(const std::string &sTableName) {
    std::map<std::string, StorageTable>::iterator it = m_mapTables.find(sTableName);
    if (it == m_mapTables.end()) {
        WSJCppLog::throw_err(TAG, "Table " + sTableName + " does not exists");
    }
    return it->second; 
}

// ---------------------------------------------------------------------

std::map<std::string, IFabricStorage*> *g_pFabricStorages = NULL;

// ---------------------------------------------------------------------

void Storages::initGlobalVariables() {
    if (g_pFabricStorages == NULL) {
        // WSJCppLog::info(std::string(), "Create employees map");
        g_pFabricStorages = new std::map<std::string, IFabricStorage*>();
    }
}

// ---------------------------------------------------------------------

void Storages::add(const std::string &sType, IFabricStorage* pStorage) {
    Storages::initGlobalVariables();
    const std::string TAG = "addStorage";
    if (g_pFabricStorages->count(sType) > 0) {
        WSJCppLog::err(TAG, sType + " - storage already registered");
    } else {
        g_pFabricStorages->insert(std::pair<std::string, IFabricStorage*>(sType,pStorage));
        // WSJCppLog::info(sType, "Registered");
    }
}

// ---------------------------------------------------------------------

std::vector<std::string> Storages::list() {
    Storages::initGlobalVariables();
    std::vector<std::string> vResult;
    std::map<std::string, IFabricStorage*>::iterator it;
    for (it = g_pFabricStorages->begin(); it != g_pFabricStorages->end(); it++) {
        vResult.push_back(it->first);
    }
    return vResult;
}

// ---------------------------------------------------------------------

bool Storages::support(const std::string &sType) {
    Storages::initGlobalVariables();
    return g_pFabricStorages->count(sType) != 0;
}

// ---------------------------------------------------------------------

Storage* Storages::create(const std::string &sType) {
    Storages::initGlobalVariables();
    std::string TAG = "findStorage";
    IFabricStorage *pFabricStorage = nullptr;
    if (g_pFabricStorages->count(sType)) {
        pFabricStorage = g_pFabricStorages->at(sType);
        return pFabricStorage->create();
    }
    return nullptr;
}

// ---------------------------------------------------------------------
// class StorageUpdateBase

StorageUpdateBase::StorageUpdateBase(const std::string &sFromVersion, const std::string &sVersion, const std::string &sDescription) {
    TAG = "Update_" + sVersion;
    m_sFromVersion = sFromVersion;
    m_sVersion = sVersion;
    m_sDescription = sDescription;
    
    if (sFromVersion != "") {
        StorageUpdateBase *pUpdate = StorageUpdates::findUpdateVersion(sFromVersion);
        if (pUpdate == nullptr) {
            WSJCppLog::throw_err(TAG, "Not found storage update version: '" + sFromVersion + "'");
        }
    }

    {
        StorageUpdateBase *pUpdate = StorageUpdates::findUpdateVersion(sVersion);
        if (pUpdate != nullptr) {
            WSJCppLog::throw_err(TAG, "Storage updates already has update with this version: '" + sVersion + "'");
        }
    }
    
    StorageUpdates::initGlobalVariables();
    g_pStorageUpdates->push_back(this);
}

// ---------------------------------------------------------------------

const std::string &StorageUpdateBase::from_version() {
    return m_sFromVersion;
}

// ---------------------------------------------------------------------

const std::string &StorageUpdateBase::version() {
    return m_sVersion;
}

// ---------------------------------------------------------------------

const std::string &StorageUpdateBase::description() {
    return m_sDescription;
}

// ---------------------------------------------------------------------

const std::vector<StorageChanges *> &StorageUpdateBase::getChanges() {
    return m_vStorageChanges;
}

// ---------------------------------------------------------------------

void StorageUpdateBase::setWeight(int nWeight) {
    m_nWeight = nWeight;
}

// ---------------------------------------------------------------------

int StorageUpdateBase::getWeight() {
    return m_nWeight;
}

// ---------------------------------------------------------------------

StorageCreateTable *StorageUpdateBase::createTable(std::string sTableName) {
    checkTableName(sTableName);
    StorageCreateTable *pCreateTable = new StorageCreateTable(sTableName);
    m_vStorageChanges.push_back(pCreateTable);
    return pCreateTable;
}

// ---------------------------------------------------------------------

StorageModifyTable *StorageUpdateBase::modifyTable(std::string sTableName) {
    checkTableName(sTableName);
    StorageModifyTable *pModifyTable = new StorageModifyTable(sTableName);
    m_vStorageChanges.push_back(pModifyTable);
    return pModifyTable;
}

// ---------------------------------------------------------------------

StorageDropTable *StorageUpdateBase::dropTable(std::string sTableName) {
    checkTableName(sTableName);
    StorageDropTable *pDropTable = new StorageDropTable(sTableName);
    m_vStorageChanges.push_back(pDropTable);
    return pDropTable;
}

StorageInsert *StorageUpdateBase::insertIntoTable(std::string sTableName) {
    StorageInsert *pInsert = new StorageInsert(sTableName);
    m_vStorageChanges.push_back(pInsert);
    return pInsert;
}

// ---------------------------------------------------------------------

void StorageUpdateBase::checkTableName(std::string sTableName) {
    // TODO check table name + runtime error
}

// ---------------------------------------------------------------------
// StorageUpdates

std::vector<StorageUpdateBase*> *g_pStorageUpdates = nullptr;

// ---------------------------------------------------------------------

void StorageUpdates::initGlobalVariables() {
    if (g_pStorageUpdates == NULL) {
        // WSJCppLog::info(std::string(), "Create list updates");
        g_pStorageUpdates = new std::vector<StorageUpdateBase*>();
    }
}

// ---------------------------------------------------------------------

int StorageUpdates::calculateWeight(int nWeight, const std::string &sVersion) {
    int nRet = nWeight;
    StorageUpdateBase* pCurrentUpdate = StorageUpdates::findUpdateVersion(sVersion);
    if (pCurrentUpdate == nullptr) {
        return nRet;
    }
    nRet++;
    std::vector<StorageUpdateBase*> vChildUpdates;
    for (int i = 0; i < g_pStorageUpdates->size(); i++) {
        StorageUpdateBase* pUpdate = g_pStorageUpdates->at(i);
        if (sVersion == pUpdate->from_version()) {
            nRet = calculateWeight(nRet, pUpdate->version());
        }
    }
    return nRet;
}

// ---------------------------------------------------------------------

void StorageUpdates::sortByWeight(std::vector<StorageUpdateBase*> &vUpdates) {
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
                StorageUpdateBase* p = vUpdates[i];
                vUpdates[i] = vUpdates[i+1];
                vUpdates[i+1] = p;
                bSorted = false;
            }
        }
    }
}

// ---------------------------------------------------------------------

std::vector<StorageUpdateBase*> StorageUpdates::findUpdatesFromVersion(const std::string &sFromVersion) {
    std::vector<StorageUpdateBase*> vFoundUpdates;
    for (int i = 0; i < g_pStorageUpdates->size(); i++) {
        StorageUpdateBase* pUpdate = g_pStorageUpdates->at(i);
        if (pUpdate->from_version() == sFromVersion) {
            vFoundUpdates.push_back(pUpdate);
        }
    }
    return vFoundUpdates;
}

// ---------------------------------------------------------------------

void StorageUpdates::pushUpdatesFromVersion(std::vector<StorageUpdateBase*> &vSortedUpdates, const std::string &sFromVersion) {
    std::vector<StorageUpdateBase*> vFoundUpdates = findUpdatesFromVersion(sFromVersion);
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

std::vector<StorageUpdateBase*> StorageUpdates::getSortedStorageUpdates() {
    StorageUpdates::initGlobalVariables();
    // calculate weights
    std::vector<StorageUpdateBase*> vSortedUpdates;
    pushUpdatesFromVersion(vSortedUpdates, "");
    return vSortedUpdates;
}

// ---------------------------------------------------------------------

StorageUpdateBase* StorageUpdates::findUpdateFromVersion(const std::string &sFromVersion) {
    StorageUpdates::initGlobalVariables();
    std::string TAG = "StorageUpdates::findUpdateFromVersion";
    StorageUpdateBase* pRet = nullptr;
    for (int i = 0; i < g_pStorageUpdates->size(); i++) {
        StorageUpdateBase* pUpdate = g_pStorageUpdates->at(i);
        if (sFromVersion == pUpdate->from_version()) {
            if (pRet == nullptr) {
                pRet = pUpdate;
            } else {
                WSJCppLog::throw_err(TAG, "Already defined update with from_version " + sFromVersion);
            }
        }
    }
    return pRet;
}

// ---------------------------------------------------------------------

StorageUpdateBase* StorageUpdates::findUpdateVersion(const std::string &sVersion) {
    StorageUpdates::initGlobalVariables();
    std::string TAG = "StorageUpdates::findUpdateVersion";
    StorageUpdateBase* pRet = nullptr;
    for (int i = 0; i < g_pStorageUpdates->size(); i++) {
        StorageUpdateBase* pUpdate = g_pStorageUpdates->at(i);
        if (sVersion == pUpdate->version()) {
            if (pRet == nullptr) {
                pRet = pUpdate;
            } else {
                WSJCppLog::throw_err(TAG, "Already defined update with version " + sVersion);
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
        WSJCppLog::err(TAG, "Could not connect to database");
        return false;
    } else {
        WSJCppLog::ok(TAG, "Successfully connection to database");
    }

    std::string sFirstVersion = "";
    std::string sLastVersion = pConn->lastDatabaseVersion();
    WSJCppLog::info(TAG, "Last Version -> '" + sLastVersion + "'");

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

                WSJCppLog::info(TAG, "Apply changes '" + pUpdate->from_version() + "' -> '" + pUpdate->version() + "'");
                // Apply changes
                std::vector<StorageChanges *> vStorageChanges = pUpdate->getChanges();
                for (int i = 0; i < vStorageChanges.size(); i++) {
                    StorageChanges *pChanges = vStorageChanges[i];
                    if (pChanges->getType() == StorageChangesType::NOPE) {
                        std::string sError = "Not allowed use a StorageChangesType::NOPE";
                        WSJCppLog::throw_err(TAG, sError);
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
    WSJCppLog::info(TAG, "apply new updates");
    bHasUpdates = true;
    while (bHasUpdates) {
        bHasUpdates = false;
        StorageUpdateBase* pUpdate = StorageUpdates::findUpdateFromVersion(sLastVersion);
        if (pUpdate != nullptr) {
            WSJCppLog::info(TAG, "Installing update '" + pUpdate->from_version() + "' -> '" + pUpdate->version() + "': " + pUpdate->description());
            sLastVersion = pUpdate->version();
            bHasUpdates = true;
            std::string error = "";

            // Apply changes
            std::vector<StorageChanges *> vStorageChanges = pUpdate->getChanges();
            for (int i = 0; i < vStorageChanges.size(); i++) {
                StorageChanges *pChanges = vStorageChanges[i];
                if (pChanges->getType() == StorageChangesType::NOPE) {
                    std::string sError = "Not allowed use a StorageChangesType::NOPE";
                    WSJCppLog::err(TAG, sError);
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

void StorageUpdates::applyAllStorageChanges(Storage *pStorage, StorageUpdateBase *pUpdate) {
    std::string TAG = "StorageUpdates::applyAllStorageChanges";
    WSJCppLog::info(TAG, "Apply changes '" + pUpdate->from_version() + "' -> '" + pUpdate->version() + "'");

    // Apply changes
    std::vector<StorageChanges *> vStorageChanges = pUpdate->getChanges();
    for (int i = 0; i < vStorageChanges.size(); i++) {
        StorageChanges *pChanges = vStorageChanges[i];
        if (pChanges->getType() == StorageChangesType::NOPE) {
            WSJCppLog::throw_err(TAG, "Not allowed use a StorageChangesType::NOPE");
        }

        if (!pStorage->addStorageChanges(*pChanges)) {
            WSJCppLog::throw_err(TAG,"Problem add storage changes '" + pChanges->getTableName() + "' in version " + pUpdate->version());
        }
    }
}

// ---------------------------------------------------------------------

void StorageUpdates::executeAllStorageChanges(Storage *pStorage, StorageConnection *pConn, StorageUpdateBase *pUpdate) {
    std::string TAG = "StorageUpdates::executeAllStorageChanges";
    WSJCppLog::info(TAG, "Installing update '" + pUpdate->from_version() + "' -> '" + pUpdate->version() + "': " + pUpdate->description());

    // Apply changes
    std::vector<StorageChanges *> vStorageChanges = pUpdate->getChanges();
    for (int i = 0; i < vStorageChanges.size(); i++) {
        StorageChanges *pChanges = vStorageChanges[i];
        if (pChanges->getType() == StorageChangesType::NOPE) {
            WSJCppLog::throw_err(TAG, "Not allowed use a StorageChangesType::NOPE");
        }
        if (!pStorage->executeStorageChanges(pConn, *pChanges)) {
            WSJCppLog::throw_err(TAG, "Problem with table '" + pChanges->getTableName() + "' in version " + pUpdate->version());
        }
    }
    pConn->insertUpdateInfo(pUpdate->version(), pUpdate->description());
}

// ---------------------------------------------------------------------

bool StorageUpdates::apply2(Storage *pStorage) {
    StorageUpdates::initGlobalVariables();
    std::string TAG = "StorageUpdates::apply";
    StorageConnection *pConn = pStorage->connect();

    std::vector<std::string> vVersions = pConn->getInstalledVersions();
    std::vector<StorageUpdateBase*> vUpdates = getSortedStorageUpdates();

    for (int i = 0; i < vUpdates.size(); i++) {
        std::string sVersion = vUpdates[i]->version();
        StorageUpdateBase* pUpdate = vUpdates[i];
        bool bUpdatedAlreadyInstalled = std::find(vVersions.begin(), vVersions.end(), sVersion) != vVersions.end();

        if (bUpdatedAlreadyInstalled) {
            applyAllStorageChanges(pStorage, pUpdate);
        } else {
            executeAllStorageChanges(pStorage, pConn, pUpdate);
        }            
    }
    return true;
}
