#ifndef CMD_HADNLERS_CLASSBOOK_H
#define CMD_HADNLERS_CLASSBOOK_H

#include <cmd_handler_base.h>
#include <QFile>

/*!
 * This handler will be add classbook record
 * */
class CmdClassbookAddRecordHandler : public ICmdHandler {

    public:
        CmdClassbookAddRecordHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookAddRecordHandler)

/*!
 * This handler will be delete classbook record
 * */
class CmdClassbookDeleteRecordHandler : public ICmdHandler {

    public:
        CmdClassbookDeleteRecordHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookDeleteRecordHandler)

/*!
 * This handler will be export classbook record
 * */
class CmdClassbookExportHandler : public ICmdHandler {

    public:
        CmdClassbookExportHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;

        void createHtml(QFile *file, QString lang, QSqlQuery query);
        void createMD(QFile *file , QString lang, QSqlQuery query);
};

REGISTRY_CMD(CmdClassbookExportHandler)

/*!
 * This handler will be return classbook record info
 * */
class CmdClassbookInfoHandler : public ICmdHandler {

    public:
        CmdClassbookInfoHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookInfoHandler)

/*!
 * This handler will be return classbook records list
 * */
class CmdClassbookListHandler : public ICmdHandler {

    public:
        CmdClassbookListHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookListHandler)

/*!
 * This handler will be update classbook record
 * */
class CmdClassbookUpdateRecordHandler : public ICmdHandler {

    public:
        CmdClassbookUpdateRecordHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookUpdateRecordHandler)

/*!
 * This handler will be add classbook localization record
 * */

class CmdClassbookLocalizationAddRecordHandler : public ICmdHandler {

    public:
        CmdClassbookLocalizationAddRecordHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookLocalizationAddRecordHandler)

/*!
 * This handler will be delete classbook localization record
 * */


class CmdClassbookLocalizationDeleteRecordHandler : public ICmdHandler {

    public:
        CmdClassbookLocalizationDeleteRecordHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookLocalizationDeleteRecordHandler)

/*!
 * This handler will be info classbook localization record
 * */

class CmdClassbookLocalizationInfoHandler : public ICmdHandler {

    public:
        CmdClassbookLocalizationInfoHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookLocalizationInfoHandler)


/*!
 * This handler will be update classbook localization record
 * */

class CmdClassbookLocalizationUpdateRecordHandler : public ICmdHandler {

    public:
        CmdClassbookLocalizationUpdateRecordHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookLocalizationUpdateRecordHandler)

/*!
 * This handler will be add classbook proposal record
 * */

class CmdClassbookProposalAddRecordHandler : public ICmdHandler {

    public:
        CmdClassbookProposalAddRecordHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookProposalAddRecordHandler)

/*!
 * This handler will be delete classbook proposal record
 * */

class CmdClassbookProposalDeleteRecordHandler : public ICmdHandler {

    public:
        CmdClassbookProposalDeleteRecordHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookProposalDeleteRecordHandler)

/*!
 * This handler will be info classbook proposal record
 * */


class CmdClassbookProposalInfoHandler : public ICmdHandler {

    public:
        CmdClassbookProposalInfoHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookProposalInfoHandler)

/*!
 * This handler will be list classbook proposal record
 * */


class CmdClassbookProposalListHandler : public ICmdHandler {

    public:
        CmdClassbookProposalListHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookProposalListHandler)

/*!
 * This handler will be prepare classbook proposal record
 * */


class CmdClassbookProposalPrepareMergeRecordHandler : public ICmdHandler {

    public:
        CmdClassbookProposalPrepareMergeRecordHandler();
        virtual std::string cmd();
        virtual std::string description();
        virtual const ModelCommandAccess &access();
        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest);

    private:
        QString TAG;
        ModelCommandAccess m_modelCommandAccess;
        std::vector<CmdInputDef> m_vInputs;
};

REGISTRY_CMD(CmdClassbookProposalPrepareMergeRecordHandler)


#endif // CMD_HADNLERS_CLASSBOOK_H
