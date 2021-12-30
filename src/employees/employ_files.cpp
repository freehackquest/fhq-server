#include <employ_files.h>
#include <employ_database.h>
#include <employ_notify.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <sys/stat.h>


REGISTRY_WJSCPP_EMPLOY(EmployFiles)

// ---------------------------------------------------------------------

EmployFiles::EmployFiles()
    : WsjcppEmployBase(EmployFiles::name(), { EmployDatabase::name(), EmployGlobalSettings::name(), EmployNotify::name() }) {
    TAG = EmployFiles::name();
}

// ---------------------------------------------------------------------

bool EmployFiles::init() {
    // TODO mutex
    // check the access to games folder
    EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
    std::string sPublicFolder = pGlobalSettings->get("web_public_folder").getDirPathValue();
    std::string sBasePath = WsjcppCore::doNormalizePath(sPublicFolder + "/quests");
    if (!WsjcppCore::dirExists(sBasePath)) {
        WsjcppCore::makeDir(sBasePath);
    }
    std::string targetTestFile = WsjcppCore::doNormalizePath(sBasePath + "/test");

    // test create file
    if (!WsjcppCore::createEmptyFile(targetTestFile)) {
        WsjcppLog::err(TAG, "Cannot access to write " + targetTestFile);
        return false;
    }
    if (!WsjcppCore::removeFile(targetTestFile)) {
        WsjcppLog::err(TAG, "Could not delete file " + targetTestFile);
        return false;
    }

    // load list of games to cache
    EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
    QSqlDatabase db = *(pDatabase->database());
    m_nSizeOfFiles = 0;
    QSqlQuery query(db);
    query.prepare("SELECT * FROM quests_files");

    if (!query.exec()) {
        WsjcppLog::err(TAG, query.lastError().text().toStdString());
        return false;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        auto pModelQuestFile = new ModelQuestFile();
        pModelQuestFile->setLocalId(record.value("id").toInt());
        std::string sUuid = record.value("uuid").toString().toStdString();
        pModelQuestFile->setUuid(sUuid);
        pModelQuestFile->setQuestLocalId(record.value("questid").toInt());
        pModelQuestFile->setFileName(record.value("filename").toString().toStdString());
        int nFileSize = record.value("size").toString().toInt();
        // TODO check max size
        m_nSizeOfFiles += nFileSize;
        pModelQuestFile->setFileSize(nFileSize);
        pModelQuestFile->setDatetime(record.value("dt").toString().toStdString());
        std::string sFilePath = record.value("filepath").toString().toStdString();
        sFilePath = WsjcppCore::doNormalizePath("/" + sFilePath);
        pModelQuestFile->setFilePath(sFilePath);
        // TODO must be cached all fields

        if (m_mapCacheQuestFilesByUuid.count(sUuid)) {
            WsjcppLog::err(TAG, "Inconsistent list quests in database by uuid: " + sUuid);
            return false;
        }
        m_mapCacheQuestFilesByUuid.insert(std::pair<std::string, ModelQuestFile*>(sUuid, pModelQuestFile));

        if (m_mapCacheQuestFilesByFilePath.count(sFilePath)) {
            WsjcppLog::err(TAG, "Inconsistent list quests in database by filepath: " + sFilePath);
            return false;
        }
        WsjcppLog::info(TAG, "Registering... file " + sFilePath);
        m_mapCacheQuestFilesByFilePath.insert(std::pair<std::string, ModelQuestFile*>(sFilePath, pModelQuestFile));

        // check filepath
        if (!WsjcppCore::fileExists(sPublicFolder + "/" + sFilePath)) {
            WsjcppLog::err(TAG, "Not found file by path: " + sFilePath + "\r\n Please try fix it and try again start server");
            return false;
        }

        m_vCacheQuestFiles.push_back(pModelQuestFile);
    }

    return true;
}

bool EmployFiles::deinit() {
    // TODO
    return true;
}

int EmployFiles::getCountOfFiles() {
    return m_vCacheQuestFiles.size();
}

int EmployFiles::getSizeOfFiles() {
    return m_nSizeOfFiles;
}

bool EmployFiles::findQuestFile(int nLocalId, ModelQuestFile &modelGame) {
    // TODO mutex
    for (int i = 0; i < m_vCacheQuestFiles.size(); i++) { // TODO create map with index
        if (m_vCacheQuestFiles[i]->getLocalId() == nLocalId) {
            modelGame.copy(*m_vCacheQuestFiles[i]);
            return true;
        }
    }
    return false;
}

bool EmployFiles::findQuestFileByFilePath(const std::string &sFilePath, ModelQuestFile &m) {
    
    std::cout << "m_mapCacheQuestFilesByFilePath.size(): " << m_mapCacheQuestFilesByFilePath.size() << std::endl;
    std::cout << "m_vCacheQuestFiles.size(): " << m_vCacheQuestFiles.size() << std::endl;

    if (!m_mapCacheQuestFilesByFilePath.count(sFilePath)) {
        return false;
    }
    m.copy(*m_mapCacheQuestFilesByFilePath.at(sFilePath));
    return true;
}

bool EmployFiles::findQuestFileByUuid(const std::string &sUuid, ModelQuestFile &m) {
    if (!m_mapCacheQuestFilesByUuid.count(sUuid)) {
        return false;
    }
    m.copy(*m_mapCacheQuestFilesByUuid.at(sUuid));
    return true;
}
