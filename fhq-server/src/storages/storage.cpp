#include <storage.h>

// ---------------------------------------------------------------------

StorageStructColumn::StorageStructColumn(const std::string &sColumnName) {
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

StorageStructColumn &StorageStructColumn::notNull() {
    m_bNotNull = true;
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::string(int nValue) {
    m_sType = "string";
    m_nTypeSize = nValue;
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::text() {
    m_sType = "text";
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::datetime() {
    m_sType = "datetime";
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::number() {
    m_sType = "number";
    return *this;
}

// ---------------------------------------------------------------------

StorageStructColumn &StorageStructColumn::primaryKey() {
    m_bPrimaryKey = true;
    return *this;
}

// ---------------------------------------------------------------------

StorageStruct::StorageStruct(const std::string &sTableName, StorageStructTableMode nMode) {
    m_sTableName = sTableName;
    m_nMode = nMode;
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

void StorageStruct::addColumn(StorageStructColumn &column) {
    m_vAddColumns.push_back(column);
}

// ---------------------------------------------------------------------

void StorageStruct::alterColumn(const std::string &sColumnName) {

}

// ---------------------------------------------------------------------

void StorageStruct::dropColumn(const std::string &sColumnName) {

}