#include <model_quest_file.h>
#include <iostream>
#include <algorithm>
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------

ModelQuestFile::ModelQuestFile() {
    TAG = "ModelQuestFile";
    m_nLocalId = 0;
    m_nQuestLocalId = 0;
    m_nFileSize = 0;
    m_sUuid = "";
    m_sFileName = "";
    m_sDatetime = "";
    m_sFilePath = "";
}

ModelQuestFile::~ModelQuestFile() {
    // 
}

int ModelQuestFile::getLocalId() const {
    return m_nLocalId;
}

void ModelQuestFile::setLocalId(int nLocalId) {
    m_nLocalId = nLocalId;
}

const std::string &ModelQuestFile::getUuid() const {
    return m_sUuid;
}

void ModelQuestFile::setUuid(std::string sUuid) {
    m_sUuid = sUuid;
}

void ModelQuestFile::setQuestLocalId(int nVal) {
    m_nQuestLocalId = nVal;
}

int ModelQuestFile::getQuestLocalId() const {
    return m_nQuestLocalId;
}

const std::string &ModelQuestFile::getFileName() const {
    return m_sFileName;
}

void ModelQuestFile::setFileName(std::string sVal) {
    m_sFileName = sVal;
}

void ModelQuestFile::setFileSize(int nVal) {
    m_nFileSize = nVal;
}

int ModelQuestFile::getFileSize() const {
    return m_nFileSize;
}

const std::string &ModelQuestFile::getDatetime() const {
    return m_sDatetime;
}

void ModelQuestFile::setDatetime(std::string sVal) {
    m_sDatetime = sVal;
}

const std::string &ModelQuestFile::getFilePath() const {
    return m_sFilePath;
}

void ModelQuestFile::setFilePath(std::string sVal) {
    m_sFilePath = sVal;
}


void ModelQuestFile::copy(const ModelQuestFile &m) {
    this->setLocalId(m.getLocalId());
    this->setUuid(m.getUuid());
    this->setQuestLocalId(m.getQuestLocalId());
    this->setFileName(m.getFileName());
    this->setFileSize(m.getFileSize());
    this->setDatetime(m.getDatetime());
    this->setFilePath(m.getFilePath());
}

// ---------------------------------------------------------------------

ModelQuestFile *ModelQuestFile::clone() const {
    ModelQuestFile *pModel = new ModelQuestFile();
    pModel->copy(*this);
    return pModel;
}

// ---------------------------------------------------------------------

nlohmann::json ModelQuestFile::toJson() {
    nlohmann::json jsonQuestFile;
    jsonQuestFile["id"] = m_nLocalId; // deprecated
    jsonQuestFile["uuid"] = m_sUuid;
    jsonQuestFile["questid"] = m_nQuestLocalId;
    jsonQuestFile["filename"] = m_sFileName;
    jsonQuestFile["size"] = m_nFileSize;
    jsonQuestFile["dt"] = m_sDatetime;
    jsonQuestFile["filepath"] = m_sFilePath;
    return jsonQuestFile;
}
