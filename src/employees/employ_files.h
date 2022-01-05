#ifndef EMPLOY_FILES_H
#define EMPLOY_FILES_H

#include <employees.h>
#include <model_quest_file.h>

class EmployFiles : public WsjcppEmployBase {
    public:

        EmployFiles();
        static std::string name() { return "EmployFiles"; }
        virtual bool init();
        virtual bool deinit();

        int getCountOfFiles();
        int getSizeOfFiles();

        bool findQuestFile(int nLocalId, ModelQuestFile &m);
        bool findQuestFileByFilePath(const std::string &sFilePath, ModelQuestFile &m);
        bool findQuestFileByUuid(const std::string &sUuid, ModelQuestFile &m);
        
        std::vector<ModelQuestFile*> findFilesByQuestId(int nQuestId);
        void updateDownloadsCounter(const ModelQuestFile &m);

    private:
        std::string TAG;
        std::mutex m_mtxFiles;
        std::vector<ModelQuestFile*> m_vCacheQuestFiles;
        int m_nSizeOfFiles;
        int m_nAllDownloadsCounter;
        std::map<std::string, ModelQuestFile*> m_mapCacheQuestFilesByUuid;
        std::map<std::string, ModelQuestFile*> m_mapCacheQuestFilesByFilePath;
};

#endif // EMPLOY_FILES_H
