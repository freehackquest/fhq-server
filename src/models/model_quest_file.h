#ifndef MODEL_QUEST_FILE_H
#define MODEL_QUEST_FILE_H

#include <string>
#include <json.hpp>
#include <validators.h>

class ModelQuestFile {
    public:

        ModelQuestFile();
        ~ModelQuestFile();
        
        int getLocalId() const;
        void setLocalId(int nVal);
        const std::string &getUuid() const;
        void setUuid(std::string sUuid);
        void setQuestLocalId(int nVal);
        int getQuestLocalId() const ;
        const std::string &getFileName() const;
        void setFileName(std::string sVal);
        void setFileSize(int nVal);
        int getFileSize() const ;
        const std::string &getDatetime() const;
        void setDatetime(std::string sVal);
        const std::string &getFilePath() const;
        void setFilePath(std::string sVal);

        void copy(const ModelQuestFile &ModelQuestFile);
        ModelQuestFile *clone() const;
        nlohmann::json toJson();

    private:
        std::string TAG;
        int m_nLocalId; // deprecated
        std::string m_sUuid;
        int m_nQuestLocalId; // deprecated
        std::string m_sFileName;
        int m_nFileSize; // deprecated
        std::string m_sDatetime;
        std::string m_sFilePath;
};

#endif // MODEL_QUEST_FILE_H
