#ifndef CMD_HADNLERS_CLASSBOOK_H
#define CMD_HADNLERS_CLASSBOOK_H

#include <cmd_handlers.h>
#include <QFile> // TODO redesign to c++ file

/*!
 * This handler will be add classbook record
 * */
class CmdClassbookAddRecordHandler : public CmdHandlerBase {
    public:
        CmdClassbookAddRecordHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookAddRecordHandler)

/*!
 * This handler will be delete classbook record
 * */
class CmdClassbookDeleteRecordHandler : public CmdHandlerBase {
    public:
        CmdClassbookDeleteRecordHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookDeleteRecordHandler)

/*!
 * This handler will be export classbook record
 * */
class CmdClassbookExportHandler : public CmdHandlerBase {
    public:
        CmdClassbookExportHandler();
        virtual void handle(ModelRequest *pRequest);

    private:
        void createHtml(QFile *file, const std::string &sLang, QSqlQuery query);
        void createMD(QFile *file, const std::string &sLang, QSqlQuery query);
};

REGISTRY_CMD(CmdClassbookExportHandler)

/*!
 * This handler will be return classbook record info
 * */
class CmdClassbookInfoHandler : public CmdHandlerBase {
    public:
        CmdClassbookInfoHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookInfoHandler)

/*!
 * This handler will be return classbook records list
 * */
class CmdClassbookListHandler : public CmdHandlerBase {
    public:
        CmdClassbookListHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookListHandler)

/*!
 * This handler will be update classbook record
 * */
class CmdClassbookUpdateRecordHandler : public CmdHandlerBase {
    public:
        CmdClassbookUpdateRecordHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookUpdateRecordHandler)

/*!
 * This handler will be add classbook localization record
 * */

class CmdClassbookLocalizationAddRecordHandler : public CmdHandlerBase {
    public:
        CmdClassbookLocalizationAddRecordHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookLocalizationAddRecordHandler)

/*!
 * This handler will be delete classbook localization record
 * */


class CmdClassbookLocalizationDeleteRecordHandler : public CmdHandlerBase {
    public:
        CmdClassbookLocalizationDeleteRecordHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookLocalizationDeleteRecordHandler)

/*!
 * This handler will be info classbook localization record
 * */

class CmdClassbookLocalizationInfoHandler : public CmdHandlerBase {
    public:
        CmdClassbookLocalizationInfoHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookLocalizationInfoHandler)


/*!
 * This handler will be update classbook localization record
 * */

class CmdClassbookLocalizationUpdateRecordHandler : public CmdHandlerBase {
    public:
        CmdClassbookLocalizationUpdateRecordHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookLocalizationUpdateRecordHandler)


/*!
 * This handler will be update classbook localization record
 * */

// class CmdClassbookLocalizationUpdateRecordHandler : public CmdHandlerBase {
//     public:
//         CmdClassbookLocalizationUpdateRecordHandler();
//         virtual void handle(ModelRequest *pRequest);
// };

// REGISTRY_CMD(CmdClassbookLocalizationUpdateRecordHandler)

/*!
 * This handler will be add classbook proposal record
 * */

class CmdClassbookProposalAddRecordHandler : public CmdHandlerBase {
    public:
        CmdClassbookProposalAddRecordHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookProposalAddRecordHandler)

/*!
 * This handler will be delete classbook proposal record
 * */

class CmdClassbookProposalDeleteRecordHandler : public CmdHandlerBase {
    public:
        CmdClassbookProposalDeleteRecordHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookProposalDeleteRecordHandler)

/*!
 * This handler will be info classbook proposal record
 * */


class CmdClassbookProposalInfoHandler : public CmdHandlerBase {
    public:
        CmdClassbookProposalInfoHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookProposalInfoHandler)

/*!
 * This handler will be list classbook proposal record
 * */


class CmdClassbookProposalListHandler : public CmdHandlerBase {
    public:
        CmdClassbookProposalListHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookProposalListHandler)

/*!
 * This handler will be prepare classbook proposal record
 * */


class CmdClassbookProposalPrepareMergeRecordHandler : public CmdHandlerBase {
    public:
        CmdClassbookProposalPrepareMergeRecordHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookProposalPrepareMergeRecordHandler)

/*!
 * This handler will be approve classbook proposal record
 * */


class CmdClassbookProposalApproveHandler : public CmdHandlerBase {
    public:
        CmdClassbookProposalApproveHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookProposalApproveHandler)

/*!
 * This handler will be update classbook proposal record
 * */


class CmdClassbookProposalUpdateHandler : public CmdHandlerBase {
    public:
        CmdClassbookProposalUpdateHandler();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdClassbookProposalUpdateHandler)

/*!
 * This handler will be return classbook content (duplicate handler ? )
 * */

class CmdClassbookHandler : public CmdHandlerBase {
    public:
        CmdClassbookHandler();
        virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HADNLERS_CLASSBOOK_H
