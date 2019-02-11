#include <storage.h>
#include <utils_logger.h>

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

std::string StorageStructColumn::columnName() {
    return m_sColumnName;
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

std::string StorageStruct::tableName() {
    return m_sTableName;
}

// ---------------------------------------------------------------------

StorageStructTableMode StorageStruct::mode() {
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

const std::vector<StorageStructColumn> &StorageStruct::listAddColumns() {
    return m_vAddColumns;
}

// ---------------------------------------------------------------------


const std::vector<StorageStructColumn> &StorageStruct::listAlterColumns() {
    return m_vAlterColumns;
}

// ---------------------------------------------------------------------


const std::vector<std::string> &StorageStruct::listDropColumns() {
    return m_vDropColumns;
}

// ---------------------------------------------------------------------
