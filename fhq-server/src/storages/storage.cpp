#include <storage.h>
#include <logger.h>
#include <ts.h>
#include <algorithm>

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
    m_mapStringValues.insert( std::pair<std::string,std::string>(sColumnName,sValue) );
}

// ---------------------------------------------------------------------

void StorageInsert::bindValue(const std::string &sColumnName, int nValue) {
    if (this->exists(sColumnName)) {
        Log::warn(TAG, "Skip. Already defined " + m_sTableName + "." + sColumnName);
        return;
    }
    m_mapIntValues.insert( std::pair<std::string,int>(sColumnName,nValue) );
}

// ---------------------------------------------------------------------

std::map<std::string, std::string> StorageInsert::stringValues() const {
    return m_mapStringValues;
}

// ---------------------------------------------------------------------

std::map<std::string, int> StorageInsert::intValues() const {
    return m_mapIntValues;
}

// ---------------------------------------------------------------------

bool StorageInsert::exists(const std::string &sColumnName) {
    return m_mapIntValues.count(sColumnName) || m_mapStringValues.count(sColumnName);
}

// ---------------------------------------------------------------------

bool StorageInsert::isValid(const StorageStruct &storageStruct) const {
    if (storageStruct.tableName() != m_sTableName) {
        Log::err(TAG, "Expeceted '" + m_sTableName + "', but got '" + storageStruct.tableName() + "'");
        return false;
    }

    std::vector<StorageStructColumn> vColumns = storageStruct.listAddColumns();
    std::vector<std::string> vCurrentColumns;

    // string values
    std::map<std::string, std::string> mapCopy1 = m_mapStringValues;
    for (std::map<std::string, std::string>::iterator it = mapCopy1.begin(); it != mapCopy1.end(); ++it) {
        bool bFound = false;
        std::string sColumnName = it->first;
        vCurrentColumns.push_back(sColumnName);
        for (int i = 0; i < vColumns.size(); i++) {
            if (vColumns[i].columnName() == it->first) {
                bFound = true;
                if (vColumns[i].columnType() != "string" && vColumns[i].columnType() != "datetime") {
                    Log::err(TAG, "In struct '" + m_sTableName + "' column '" + sColumnName + "' must be string");
                    return false;
                }
            }
        }
        if (!bFound) {
            Log::err(TAG, "Struct '" + m_sTableName + "' has not column '" + sColumnName + "'");
            return false;
        }
    }

    // int values
    std::map<std::string, int> mapCopy2 = m_mapIntValues;
    for (std::map<std::string, int>::iterator it = mapCopy2.begin(); it != mapCopy2.end(); ++it) {
        bool bFound = false;
        std::string sColumnName = it->first;
        vCurrentColumns.push_back(sColumnName);
        for (int i = 0; i < vColumns.size(); i++) {
            if (vColumns[i].columnName() == it->first) {
                bFound = true;
                if (vColumns[i].columnType() != "number") {
                    Log::err(TAG, "In struct '" + m_sTableName + "' column '" + sColumnName + "' must be number");
                    return false;
                }
            }
        }
        if (!bFound) {
            Log::err(TAG, "Struct '" + m_sTableName + "' has not column '" + sColumnName + "'");
            return false;
        }
    }

    for (int i = 0; i < vColumns.size(); i++) {
        if (!vColumns[i].isAutoIncrement() && vColumns[i].isNotNull()) {
            if(std::find(vCurrentColumns.begin(), vCurrentColumns.end(), vColumns[i].columnName()) == vCurrentColumns.end()) {
                Log::err(TAG, "Missing require field '" + m_sTableName + "." + vColumns[i].columnName() + "'");
                return false;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------

// ***********************
// *** StorageConnection
// ***********************

StorageConnection::StorageConnection() {
    m_nCreated = TS::currentTime_milliseconds();
    TAG = "StorageConnection";
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

bool Storage::applyStruct(StorageConnection *pConn, StorageStruct &storageStruct) {
    std::vector<std::string> v = this->prepareSqlQueries(storageStruct);
    for (int i = 0; i < v.size(); i++) {
        std::string sQuery = v[i];
        if (!pConn->executeQuery(sQuery)) {
            return false;
        }
    }

    std::string sTableName = storageStruct.tableName();
    if (storageStruct.mode() == StorageStructTableMode::CREATE) {
        if (m_mapStructs.count(sTableName)) {
            Log::err(TAG, "Struct '" + sTableName + "' already defined");
            Log::warn(TAG, "TODO need drop table");
            return false;
        } else {
            m_mapStructs.insert( std::pair<std::string,StorageStruct>(sTableName,storageStruct) );
        }
    }

    if (storageStruct.mode() == StorageStructTableMode::ALTER) {
        if (!m_mapStructs.count(sTableName)) {
            Log::err(TAG, "Not found table '" + sTableName + "'");
            return false;
        } else {
            m_mapStructs.find(sTableName)->second.mergeWith(storageStruct);
        }
    }

    if (storageStruct.mode() == StorageStructTableMode::DROP) {
        if (!m_mapStructs.count(sTableName)) {
            Log::err(TAG, "Not found table '" + sTableName + "'");
            return false;
        } else {
            m_mapStructs.erase(sTableName);
        }
        
    }

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
}
