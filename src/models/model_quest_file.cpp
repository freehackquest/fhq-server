#include <model_quest_file.h>
#include <iostream>
#include <algorithm>
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------

ModelQuestFile::ModelQuestFile() {
    TAG = "ModelQuestFile";
    m_nLocalId = 0;
    m_nQuestLocalId = 0;
    m_nDownloadsCounter = 0;
    m_nFileSize = 0;
    m_sFileHumanSize = "";
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

void ModelQuestFile::setDownloadsCounter(int nVal) {
    m_nDownloadsCounter = nVal;
}

int ModelQuestFile::getDownloadsCounter() const {
    return m_nDownloadsCounter;
}

const std::string &ModelQuestFile::getFileName() const {
    return m_sFileName;
}

void ModelQuestFile::setFileName(std::string sVal) {
    m_sFileName = sVal;
}

void ModelQuestFile::setFileSize(int nVal) {
    m_nFileSize = nVal;
    m_sFileHumanSize = WsjcppCore::getHumanSizeBytes(nVal);
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

void ModelQuestFile::setMd5(std::string sVal) {
    m_sMd5 = sVal;
}

const std::string &ModelQuestFile::getMd5() const {
    return m_sMd5;
}

void ModelQuestFile::copy(const ModelQuestFile &m) {
    this->setLocalId(m.getLocalId());
    this->setUuid(m.getUuid());
    this->setQuestLocalId(m.getQuestLocalId());
    this->setDownloadsCounter(m.getDownloadsCounter());
    this->setFileName(m.getFileName());
    this->setFileSize(m.getFileSize());
    this->setDatetime(m.getDatetime());
    this->setFilePath(m.getFilePath());
    this->setMd5(m.getMd5());
}

ModelQuestFile *ModelQuestFile::clone() const {
    ModelQuestFile *pModel = new ModelQuestFile();
    pModel->copy(*this);
    return pModel;
}

nlohmann::json ModelQuestFile::toJson() {
    nlohmann::json jsonQuestFile;
    jsonQuestFile["id"] = m_nLocalId; // deprecated
    jsonQuestFile["uuid"] = m_sUuid;
    jsonQuestFile["questid"] = m_nQuestLocalId;
    jsonQuestFile["downloads_counter"] = m_nDownloadsCounter;
    jsonQuestFile["filename"] = m_sFileName;
    jsonQuestFile["size"] = m_nFileSize;
    jsonQuestFile["human_size"] = m_sFileHumanSize;
    jsonQuestFile["dt"] = m_sDatetime;
    jsonQuestFile["filepath"] = m_sFilePath;
    jsonQuestFile["md5"] = m_sMd5;
    return jsonQuestFile;
}
