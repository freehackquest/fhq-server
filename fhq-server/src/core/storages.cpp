#include <storages.h>
#include <logger.h>
#include <algorithm>
#include <fallen.h>

// ***********************
// *** StorageStructColumn
// ***********************

// ---------------------------------------------------------------------

StorageStructColumn::StorageStructColumn(const std::string &sColumnName) {
    TAG = "StorageStructColumn_" + sColumnName;
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
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::autoIncrement() {
    m_bAutoIncrement = true;
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::notNull() {
    m_bNotNull = true;
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::string(int nValue) {
    if (m_sType != "") {
        Log::err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "string";
    m_nTypeSize = nValue;
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::text() {
    if (m_sType != "") {
        Log::err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "text";
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::datetime() {
    if (m_sType != "") {
        Log::err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "datetime";
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::number() {
    if (m_sType != "") {
        Log::err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "number";
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::doubleNumber() {
    if (m_sType != "") {
        Log::err(TAG, "Datatype already defined: " + m_sType);
        return *this;
    }
    m_sType = "doubleNumber";
    return *this;
}


// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::primaryKey() {
    m_bPrimaryKey = true;
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::defaultValue(const std::string& sDefaultValue) {
    m_sDefaultValue = sDefaultValue;
    m_bDefaultValue = true;
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::enableIndex() {
    m_bEnableIndex = true;
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::enableUniqueIndex(const std::string& sIndexName) {
    m_sNameOfUniqueIndex = sIndexName;
    m_bEnableUniqueIndex = true;
    return *this;
}

// ---------------------------------------------------------------------

std::string StorageStructColumn::columnName() {
    return m_sColumnName;
}

// ---------------------------------------------------------------------

std::string StorageStructColumn::columnType() {
    return m_sType;
}

// ---------------------------------------------------------------------

std::string StorageStructColumn::columnDefaultValue() {
    return m_sDefaultValue;
}

// ---------------------------------------------------------------------

bool StorageStructColumn::isDefaultValue() {
    return m_bDefaultValue;
}

// ---------------------------------------------------------------------

int StorageStructColumn::columnTypeSize() {
    return m_nTypeSize;
}

// ---------------------------------------------------------------------

bool StorageStructColumn::isAutoIncrement() {
    return m_bAutoIncrement;
}

// ---------------------------------------------------------------------

bool StorageStructColumn::isPrimaryKey() {
    return m_bPrimaryKey;
}

// ---------------------------------------------------------------------

bool StorageStructColumn::isNotNull() {
    return m_bNotNull;
}

// ---------------------------------------------------------------------

bool StorageStructColumn::isEnableIndex() {
    return m_bEnableIndex;
}

// ---------------------------------------------------------------------

bool StorageStructColumn::isEnableUniqueIndex() {
    return m_bEnableUniqueIndex;
}

// ---------------------------------------------------------------------

std::string StorageStructColumn::nameOfUniqueIndex() {
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
    return "Creating table " + m_sTableName;
}

// ---------------------------------------------------------------------

std::string StorageCreateTable::getAppliedSuccess() const {
    return "Created table " +  m_sTableName;
}

// ---------------------------------------------------------------------

std::string StorageCreateTable::getAppliedFailed() const {
    return "Could not create table " +  m_sTableName;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageCreateTable::addColumn(const std::string &sColumnName) {
    for (int i = 0; i < m_vColumns.size(); i++) {
        if (m_vColumns[i].columnName() == sColumnName) {
            std::string sError = "Column '" + sColumnName + "' in table '" + m_sTableName + "' already exists";
            Log::err(TAG, sError);
            throw std::runtime_error(sError);
        }
    }
    m_vColumns.push_back(StorageStructColumn(sColumnName));
    return m_vColumns[m_vColumns.size()-1];
}

// ---------------------------------------------------------------------

const std::vector<StorageStructColumn> &StorageCreateTable::getColumns() const {
    return m_vColumns;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageStruct
// ***********************

// ---------------------------------------------------------------------

StorageStruct::StorageStruct(const std::string &sTableName, StorageStructTableMode nMode) {
    m_sTableName = sTableName;
    // TODO validate name table
    m_nMode = nMode;
    TAG = "StorageStruct_" + sTableName;
    if (m_nMode == StorageStructTableMode::CREATE) {
        TAG += "_CREATE";
    }
    if (m_nMode == StorageStructTableMode::DROP) {
        TAG += "_DROP";
    }
    if (m_nMode == StorageStructTableMode::ALTER) {
        TAG += "_ALTER";
    }
}

// ---------------------------------------------------------------------

std::string StorageStruct::tableName() const {
    return m_sTableName;
}

// ---------------------------------------------------------------------

StorageStructTableMode StorageStruct::mode() const {
    return m_nMode;
}

// ---------------------------------------------------------------------

bool StorageStruct::addColumn(StorageStructColumn &column) {
    if (m_nMode == StorageStructTableMode::DROP) {
        Log::err(TAG, "addColumn not allowed for a drop mode");
        return false;
    }
    // TODO check if already exists
    m_vAddColumns.push_back(column);
    return true;
}

// ---------------------------------------------------------------------

bool StorageStruct::alterColumn(StorageStructColumn &column) {
    if (m_nMode == StorageStructTableMode::DROP) {
        Log::err(TAG, "alterColumn not allowed for a drop mode");
        return false;
    }

    if (m_nMode == StorageStructTableMode::CREATE) {
        Log::err(TAG, "alterColumn not allowed for a create mode");
        return false;
    }

    m_vAlterColumns.push_back(column);
    // TODO check if exists
    return true;
}

// ---------------------------------------------------------------------

bool StorageStruct::dropColumn(const std::string &sColumnName) {
    if (m_nMode == StorageStructTableMode::CREATE) {
        Log::err(TAG, "dropColumn not allowed for a create mode");
        return false;
    }
    if (m_nMode == StorageStructTableMode::DROP) {
        Log::err(TAG, "dropColumn not allowed for a drop mode");
        return false;
    }
    // TODO check if exists
    m_vDropColumns.push_back(sColumnName);
    return true;
}

// ---------------------------------------------------------------------

const std::vector<StorageStructColumn> &StorageStruct::listAddColumns() const {
    return m_vAddColumns;
}

// ---------------------------------------------------------------------


const std::vector<StorageStructColumn> &StorageStruct::listAlterColumns() const {
    return m_vAlterColumns;
}

// ---------------------------------------------------------------------

const std::vector<std::string> &StorageStruct::listDropColumns() const {
    return m_vDropColumns;
}

// ---------------------------------------------------------------------

bool StorageStruct::mergeWith(const StorageStruct &storageStruct) {
    if (m_nMode != StorageStructTableMode::CREATE) {
        Log::err(TAG, "Could not merge if current define mode is not CREATE");
        return false;
    };

    if (storageStruct.mode() == StorageStructTableMode::CREATE) {
        Log::err(TAG, "Could not merge if target define mode is CREATE");
        return false;
    };

    if (storageStruct.tableName() != m_sTableName) {
        Log::err(TAG, "Could not merge current '" + m_sTableName + "' and got '" + storageStruct.tableName() + "'");
        return false;
    };

    // TODO check indexes
    std::vector<std::string> vDropColumns = storageStruct.listDropColumns();
    for (int i = 0; i < vDropColumns.size(); i++) {
        bool bFound = false;    
        std::string sColumnName = vDropColumns[i];
        for (int i = 0; i < m_vAddColumns.size(); i++) {
            if (m_vAddColumns[i].columnName() == sColumnName) {
                m_vAddColumns.erase(m_vAddColumns.begin() + i);
                bFound = true;
                break;
            }
        }
        if (!bFound) {
            Log::err(TAG, "Not found column " + sColumnName + " in table " + m_sTableName);
            return false;
        }
    }

    // TODO check indexes
    std::vector<StorageStructColumn> vAddColumns = storageStruct.listAddColumns();
    for (int i = 0; i < vAddColumns.size(); i++) {
        bool bFound = false;
        StorageStructColumn c = vAddColumns[i];
        for (int i = 0; i < m_vAddColumns.size(); i++) {
            if (m_vAddColumns[i].columnName() == c.columnName()) {
                bFound = true;
            }
        }
        if (bFound) {
            Log::err(TAG, "Column already defined " + c.columnName() + " in table " + m_sTableName);
            return false;
        } else {
            m_vAddColumns.push_back(c);
        }
    }

    // TODO check indexes
    std::vector<StorageStructColumn> vAlterColumns = storageStruct.listAlterColumns();
    for (int i = 0; i < vAlterColumns.size(); i++) {
        bool bFound = false;
        StorageStructColumn c = vAlterColumns[i];
        for (int i = 0; i < m_vAddColumns.size(); i++) {
            if (m_vAddColumns[i].columnName() == c.columnName()) {
                if (m_vAddColumns[i].isEnableIndex()) {
                    c.enableIndex();
                }
                m_vAddColumns[i] = c;
                bFound = true;
            }
        }
        if (!bFound) {
            Log::err(TAG, "Problem with alter column '" + c.columnName() + "'. It's not defined in table '" + m_sTableName + "'");
            return false;
        }
    }

    return true;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageColumnValue
// ***********************

StorageColumnValue::StorageColumnValue(const std::string &sColumnName, StorageStructColumnType nType) {
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

StorageStructColumnType StorageColumnValue::getColumnType() {
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

StorageInsert::StorageInsert(const std::string &sTableName) {
    TAG = "StorageInsert";
    m_sTableName = sTableName;
}

// ---------------------------------------------------------------------

std::string StorageInsert::tableName() const {
    return m_sTableName;
}

// ---------------------------------------------------------------------

void StorageInsert::bindValue(const std::string &sColumnName, const std::string &sValue) {
    if (this->exists(sColumnName)) {
        Log::warn(TAG, "Skip. Already defined " + m_sTableName + "." + sColumnName);
        return;
    }
    StorageColumnValue val(sColumnName, StorageStructColumnType::STRING);
    val.setValue(sValue);
    m_vValues.push_back(val);
}

// ---------------------------------------------------------------------

void StorageInsert::bindValue(const std::string &sColumnName, int nValue) {
    if (this->exists(sColumnName)) {
        Log::warn(TAG, "Skip. Already defined " + m_sTableName + "." + sColumnName);
        return;
    }
    StorageColumnValue val(sColumnName, StorageStructColumnType::NUMBER);
    val.setValue(nValue);
    m_vValues.push_back(val);
}

// ---------------------------------------------------------------------

void StorageInsert::bindValue(const std::string &sColumnName, double nValue) {
    if (this->exists(sColumnName)) {
        Log::warn(TAG, "Skip. Already defined " + m_sTableName + "." + sColumnName);
        return;
    }
    StorageColumnValue val(sColumnName, StorageStructColumnType::DOUBLE_NUMBER);
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

bool StorageInsert::isValid(const StorageStruct &storageStruct) const {
    if (storageStruct.tableName() != m_sTableName) {
        Log::err(TAG, "Expeceted '" + m_sTableName + "', but got '" + storageStruct.tableName() + "'");
        return false;
    }

    std::vector<StorageStructColumn> vColumns = storageStruct.listAddColumns();
    std::vector<std::string> vCurrentColumns;
    
    for (int x = 0; x < m_vValues.size(); x++) {
        bool bFound = false;
        StorageColumnValue val = m_vValues[x];
        vCurrentColumns.push_back(val.getColumnName());
        for (int y = 0; y < vColumns.size(); y++) {
            StorageStructColumn st = vColumns[y];
            if (st.columnName() == val.getColumnName()) {
                if (val.getColumnType() == StorageStructColumnType::STRING
                    && (st.columnType() == "string" || st.columnType() == "datetime" || st.columnType() == "text")) {
                    bFound = true;
                    break;
                } else if (val.getColumnType() == StorageStructColumnType::NUMBER && st.columnType() == "number") {
                    bFound = true;
                    break;
                } else if (val.getColumnType() == StorageStructColumnType::DOUBLE_NUMBER && st.columnType() == "doubleNumber") {
                    bFound = true;
                    break;
                } else {
                    Log::err(TAG, "StorageInsert::isValid. In struct '" + m_sTableName + "' column '" + val.getColumnName() + "'."
                        " Expected datatype '" + st.columnType() + "', but got '" + std::to_string(val.getColumnType()) + "'");
                    return false;
                }
            }
        }
        if (!bFound) {
            Log::err(TAG, "StorageInsert::isValid. Struct '" + m_sTableName + "' has not column '" + val.getColumnName() + "'");
            return false;
        }
    }

    for (int i = 0; i < vColumns.size(); i++) {
        if (!vColumns[i].isAutoIncrement() && vColumns[i].isNotNull()) {
            if (std::find(vCurrentColumns.begin(), vCurrentColumns.end(), vColumns[i].columnName()) == vCurrentColumns.end()) {
                Log::err(TAG, "Missing require field '" + m_sTableName + "." + vColumns[i].columnName() + "'");
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

const std::vector<StorageStructColumn> &StorageTable::getColumns() const {
    return m_vColumns;
}

// ---------------------------------------------------------------------

void StorageTable::mergeWith(StorageCreateTable &createTable) {
    const std::vector<StorageStructColumn> &list = createTable.getColumns();
    for (int i = 0; i < list.size(); i++) {
        m_vColumns.push_back(list[i]);
    }
}

// ---------------------------------------------------------------------

void StorageTable::mergeWith(StorageChanges &storageChanges) {
    if (storageChanges.getType() == StorageChangesType::CREATE_TABLE) {
        StorageCreateTable createTable = (StorageCreateTable &)storageChanges;
        mergeWith(createTable);
    } else {
        throw std::runtime_error("Could not support type of storageChanges for merge with table");
    }
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageConnection
// ***********************

StorageConnection::StorageConnection() {
    m_nCreated = Fallen::currentTime_milliseconds();
    TAG = "StorageConnection";
}

StorageConnection::~StorageConnection() {
    // Nothing
}

// ---------------------------------------------------------------------

long StorageConnection::created() {
    return m_nCreated;
}

// ---------------------------------------------------------------------

// ***********************
// *** Storage
// ***********************

Storage::Storage() {
    TAG = "Storage";
}

// ---------------------------------------------------------------------
// TODO: deprecated

bool Storage::addStruct(StorageStruct &storageStruct) {
    std::string sTableName = storageStruct.tableName();
    std::map<std::string,StorageStruct>::iterator it = m_mapStructs.find(sTableName);

    if (storageStruct.mode() == StorageStructTableMode::CREATE) {
        if (it != m_mapStructs.end()) {
            Log::err(TAG, "Struct '" + sTableName + "' already defined");
            Log::warn(TAG, "TODO need drop table");
            return false;
        }
        m_mapStructs.insert( std::pair<std::string,StorageStruct>(sTableName,storageStruct) );
    } else  if (storageStruct.mode() == StorageStructTableMode::ALTER) {
        if (it == m_mapStructs.end()) {
            Log::err(TAG, "Not found table '" + sTableName + "'");
            return false;
        }
        m_mapStructs.find(sTableName)->second.mergeWith(storageStruct);
    } else if (storageStruct.mode() == StorageStructTableMode::DROP) {
        if (it == m_mapStructs.end()) {
            Log::err(TAG, "Not found table '" + sTableName + "'");
            return false;
        }
        m_mapStructs.erase(sTableName);
    } else {
        Log::err(TAG, "Unknown operation with table");
        std::runtime_error("Unknown operation with table");
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------
// TODO: deprecated

bool Storage::applyStruct(StorageConnection *pConn, StorageStruct &storageStruct) {
    std::string sTableName = storageStruct.tableName();
    std::string sStartApply = "";
    std::string sAppliedSuccess = "";
    std::string sAppliedFailed = "";
    if (storageStruct.mode() == StorageStructTableMode::CREATE) {
        sStartApply = "Creating table " + sTableName;
        sAppliedSuccess = "Created table " +  sTableName;
        sAppliedFailed = "Could not create table " +  sTableName;
    } else if (storageStruct.mode() == StorageStructTableMode::DROP) {
        sStartApply = "Dropping";
        sAppliedSuccess = "Dropped table " +  sTableName;
        sAppliedFailed = "Could not drop table " +  sTableName;
    } else if (storageStruct.mode() == StorageStructTableMode::ALTER) {
        sStartApply = "Altering table " +  sTableName;
        sAppliedSuccess = "Altered table " +  sTableName;
        sAppliedFailed = "Could not alter table " +  sTableName;
    } else {
        throw std::runtime_error("Unknown type of StorageStructTableMode");
    }
    
    Log::info(TAG, sStartApply);
    std::vector<std::string> v = this->prepareSqlQueries(storageStruct);
    for (int i = 0; i < v.size(); i++) {
        std::string sQuery = v[i];
        if (!pConn->executeQuery(sQuery)) {
            Log::err(TAG, sAppliedFailed + "\n    query -> " + sQuery);
            return false;
        }
    }
    
    if (!this->addStruct(storageStruct)) {
        Log::err(TAG, sAppliedFailed);
        return false;
    }
    Log::ok(TAG, sAppliedSuccess);
    return true;
}

// ---------------------------------------------------------------------

bool Storage::addStorageChanges(StorageChanges &storageChanges) {
    std::string sTableName = storageChanges.getTableName();
    std::map<std::string,StorageTable>::iterator it = m_mapTables.find(sTableName);

    if (storageChanges.getType() == StorageChangesType::CREATE_TABLE) {
        if (it != m_mapTables.end()) {
            Log::err(TAG, "Table '" + sTableName + "' already defined");
            Log::warn(TAG, "TODO need drop table");
            return false;
        }
        StorageTable tbl(sTableName);
        tbl.mergeWith(storageChanges);
        m_mapTables.insert( std::pair<std::string,StorageTable>(sTableName,tbl) );
    } else {
        Log::err(TAG, "addStorageChanges, Unknown operation with table");
        throw std::runtime_error("addStorageChanges, Unknown operation with table");
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
    } else {
        throw std::runtime_error("Could not support type of StorageChanges");
    }

    Log::info(TAG, storageChanges.getStartApply());
    for (int i = 0; i < vQueries.size(); i++) {
        std::string sQuery = vQueries[i];
        if (!pConn->executeQuery(sQuery)) {
            Log::err(TAG, storageChanges.getAppliedFailed() + "\n    query -> " + sQuery);
            return false;
        }
    }

    if (!this->addStorageChanges(storageChanges)) {
        Log::err(TAG, storageChanges.getAppliedFailed());
        return false;
    }
    Log::ok(TAG, storageChanges.getAppliedSuccess());
    return true;
}

// ---------------------------------------------------------------------

bool Storage::insertRow(StorageConnection *pConn, const StorageInsert &storageInsert) {

    std::string sTableName = storageInsert.tableName();
    std::map<std::string,StorageStruct>::iterator it = m_mapStructs.find(sTableName);
    if (it == m_mapStructs.end()) {
        Log::err(TAG, "Struct '" + sTableName + "' not found");
        return false;
    }

    if (!storageInsert.isValid(it->second)) {
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

const std::map<std::string, StorageStruct> &Storage::storageStruct() {
    return m_mapStructs;
}

// ---------------------------------------------------------------------

std::map<std::string, IFabricStorage*> *g_pFabricStorages = NULL;

// ---------------------------------------------------------------------

void Storages::initGlobalVariables() {
    if (g_pFabricStorages == NULL) {
        // Log::info(std::string(), "Create employees map");
        g_pFabricStorages = new std::map<std::string, IFabricStorage*>();
    }
}

// ---------------------------------------------------------------------

void Storages::add(const std::string &sType, IFabricStorage* pStorage) {
    Storages::initGlobalVariables();
    const std::string TAG = "addStorage";
    if (g_pFabricStorages->count(sType) > 0) {
        Log::err(TAG, sType + " - storage already registered");
    } else {
        g_pFabricStorages->insert(std::pair<std::string, IFabricStorage*>(sType,pStorage));
        // Log::info(sType, "Registered");
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

std::vector<StorageUpdateBase*> *g_pStorageUpdates = nullptr;

// ---------------------------------------------------------------------
// class StorageUpdateBase

StorageUpdateBase::StorageUpdateBase(const std::string &sFromVersion, const std::string &sVersion, const std::string &sDescription) {
    TAG = "Update_" + sVersion;
    m_sFromVersion = sFromVersion;
    m_sVersion = sVersion;
    m_sDescription = sDescription;
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

// TODO deprecated
const std::vector<StorageStruct> &StorageUpdateBase::listOfStructChanges() {
    return m_vStructChanges;
}

// ---------------------------------------------------------------------

const std::vector<StorageChanges> &StorageUpdateBase::getListChanges() {
    return m_vStorageChanges;
}

// ---------------------------------------------------------------------

void StorageUpdateBase::add(StorageStruct st) {
    m_vStructChanges.push_back(st);
}

// ---------------------------------------------------------------------

void StorageUpdateBase::addStorageChanges(const StorageChanges &st) {
    m_vStorageChanges.push_back(st);
}

// ---------------------------------------------------------------------

void StorageUpdates::initGlobalVariables() {
    if (g_pStorageUpdates == NULL) {
        // Log::info(std::string(), "Create list updates");
        g_pStorageUpdates = new std::vector<StorageUpdateBase*>();
    }
}

// ---------------------------------------------------------------------

StorageUpdateBase* StorageUpdates::findUpdateFromVersion(const std::string &sFromVersion) {
    std::string TAG = "StorageUpdates::findUpdateFromVersion";
    StorageUpdateBase* pRet = nullptr;
    for (int i = 0; i < g_pStorageUpdates->size(); i++) {
        StorageUpdateBase* pUpdate = g_pStorageUpdates->at(i);
        if (sFromVersion == pUpdate->from_version()) {
            if (pRet == nullptr) {
                pRet = pUpdate;
            } else {
                Log::warn(TAG, "Already defined update with from_version " + sFromVersion);
            }
        }
    }
    return pRet;
}

// ---------------------------------------------------------------------

bool StorageUpdates::apply(Storage *pStorage) {
    StorageUpdates::initGlobalVariables();
    std::string TAG = "StorageUpdates::apply";

    StorageConnection *pConn = pStorage->connect();
    if (pConn == nullptr) {
        Log::err(TAG, "Could not connect to database");
        return false;
    } else {
        Log::ok(TAG, "Successfully connection to database");
    }

    std::string sFirstVersion = "";
    std::string sLastVersion = pConn->lastDatabaseVersion();
    Log::info(TAG, "Last Version -> '" + sLastVersion + "'");

    // restore struct of storage before sLastVersion
    bool bHasUpdates = true;
    while (bHasUpdates) {
        bHasUpdates = false;
        StorageUpdateBase* pUpdate = StorageUpdates::findUpdateFromVersion(sFirstVersion);
        if (pUpdate != nullptr) {
            if (sFirstVersion != sLastVersion) {
                sFirstVersion = pUpdate->version();
                Log::info(TAG, "Add struct from '" + pUpdate->from_version() + "' -> '" + pUpdate->version() + "'");
                bHasUpdates = true;
                std::string error = "";

                // TODO: Apply struct - deprecated
                std::vector<StorageStruct> vStructChanges = pUpdate->listOfStructChanges();
                for (int i = 0; i < vStructChanges.size(); i++) {
                    StorageStruct st = vStructChanges[i];
                    if (!pStorage->addStruct(st)) {
                        error = " Problem with add struct '" + st.tableName() + "'";
                        Log::err(TAG, error);
                        delete pConn;
                        return false;
                    }
                }
                Log::info(TAG, "Apply changes '" + pUpdate->from_version() + "' -> '" + pUpdate->version() + "'");
                // Apply changes
                std::vector<StorageChanges> vStorageChanges = pUpdate->getListChanges();
                for (int i = 0; i < vStorageChanges.size(); i++) {
                    Log::info(TAG, "Apply changes (1)");
                    StorageChanges st2 = vStorageChanges[i];
                    if (st2.getType() == StorageChangesType::NOPE) {
                        std::string sError = "Not allowed use a StorageChangesType::NOPE";
                        Log::err(TAG, sError);
                        throw std::runtime_error(sError);
                    }

                    if (!pStorage->addStorageChanges(st2)) {
                        error = "Problem add storage changes '" + st2.getTableName() + "' in version " + pUpdate->version();
                        delete pConn;
                        return false;
                    }
                }
                Log::info(TAG, "Apply changes (2)");
            }
        }
    }

    // apply new updates
    Log::info(TAG, "apply new updates");
    bHasUpdates = true;
    while (bHasUpdates) {
        bHasUpdates = false;
        StorageUpdateBase* pUpdate = StorageUpdates::findUpdateFromVersion(sLastVersion);
        if (pUpdate != nullptr) {
            Log::info(TAG, "Installing update '" + pUpdate->from_version() + "' -> '" + pUpdate->version() + "': " + pUpdate->description());
            sLastVersion = pUpdate->version();
            bHasUpdates = true;
            std::string error = "";
            std::vector<StorageStruct> vStructChanges = pUpdate->listOfStructChanges();

            // TODO: Apply struct - deprecated
            for (int i = 0; i < vStructChanges.size(); i++) {
                StorageStruct st = vStructChanges[i];
                if (!pStorage->applyStruct(pConn, st)) {
                    error = "Problem with table '" + st.tableName() + "' in version " + pUpdate->version();
                    delete pConn;
                    return false;
                }
            }

            // Apply changes
            std::vector<StorageChanges> vStorageChanges = pUpdate->getListChanges();
            for (int i = 0; i < vStorageChanges.size(); i++) {
                StorageChanges st2 = vStorageChanges[i];
                if (st2.getType() == StorageChangesType::NOPE) {
                    std::string sError = "Not allowed use a StorageChangesType::NOPE";
                    Log::err(TAG, sError);
                    throw std::runtime_error(sError);
                }
                if (!pStorage->executeStorageChanges(pConn, st2)) {
                    error = "Problem with table '" + st2.getTableName() + "' in version " + pUpdate->version();
                    delete pConn;
                    return false;
                }
            }

            // TODO rename to after
            if (!pUpdate->custom(pStorage, pConn, error)) {
                Log::err(TAG, "Error on install custom update: \n\t -> " + error);
                delete pConn;
                return false;
            }
            pConn->insertUpdateInfo(pUpdate->version(), pUpdate->description());
        }
    }
    return true;
}
