#ifndef CMD_HADNLERS_CLASSBOOK_H
#define CMD_HADNLERS_CLASSBOOK_H

#include <wsjcpp_jsonrpc20.h>
#include <QFile> // TODO redesign to c++ file
#include <QSqlQuery> // TODO deprecated
#include <QSqlRecord> // TODO deprecated
#include <QString> // TODO deprecated
#include <QVariant> // TODO deprecated

/*!
 * This handler will be add classbook record
 * */
class CmdClassbookAddRecordHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookAddRecordHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

/*!
 * This handler will be delete classbook record
 * */
class CmdClassbookDeleteRecordHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookDeleteRecordHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookDeleteRecordHandler)

/*!
 * This handler will be export classbook record
 * */
class CmdClassbookExportHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookExportHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);

    private:
        void createHtml(QFile *file, const std::string &sLang, QSqlQuery query);
        void createMD(QFile *file, const std::string &sLang, QSqlQuery query);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookExportHandler)

/*!
 * This handler will be return classbook record info
 * */
class CmdClassbookInfoHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookInfoHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookInfoHandler)

/*!
 * This handler will be return classbook records list
 * */
class CmdClassbookListHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookListHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookListHandler)

/*!
 * This handler will be update classbook record
 * */
class CmdClassbookUpdateRecordHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookUpdateRecordHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookUpdateRecordHandler)

/*!
 * This handler will be add classbook localization record
 * */

class CmdClassbookLocalizationAddRecordHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookLocalizationAddRecordHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookLocalizationAddRecordHandler)

/*!
 * This handler will be delete classbook localization record
 * */


class CmdClassbookLocalizationDeleteRecordHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookLocalizationDeleteRecordHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookLocalizationDeleteRecordHandler)

/*!
 * This handler will be info classbook localization record
 * */

class CmdClassbookLocalizationInfoHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookLocalizationInfoHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookLocalizationInfoHandler)


/*!
 * This handler will be update classbook localization record
 * */

class CmdClassbookLocalizationUpdateRecordHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookLocalizationUpdateRecordHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookLocalizationUpdateRecordHandler)


/*!
 * This handler will be update classbook localization record
 * */

// class CmdClassbookLocalizationUpdateRecordHandler : public WsjcppJsonRpc20HandlerBase {
//     public:
//         CmdClassbookLocalizationUpdateRecordHandler();
//         virtual void handle(WsjcppJsonRpc20Request *pRequest);
// };

// REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookLocalizationUpdateRecordHandler)

/*!
 * This handler will be add classbook proposal record
 * */

class CmdClassbookProposalAddRecordHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookProposalAddRecordHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookProposalAddRecordHandler)

/*!
 * This handler will be delete classbook proposal record
 * */

class CmdClassbookProposalDeleteRecordHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookProposalDeleteRecordHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookProposalDeleteRecordHandler)

/*!
 * This handler will be info classbook proposal record
 * */


class CmdClassbookProposalInfoHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookProposalInfoHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookProposalInfoHandler)

/*!
 * This handler will be list classbook proposal record
 * */


class CmdClassbookProposalListHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookProposalListHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookProposalListHandler)

/*!
 * This handler will be prepare classbook proposal record
 * */


class CmdClassbookProposalPrepareMergeRecordHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookProposalPrepareMergeRecordHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookProposalPrepareMergeRecordHandler)

/*!
 * This handler will be approve classbook proposal record
 * */


class CmdClassbookProposalApproveHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookProposalApproveHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookProposalApproveHandler)

/*!
 * This handler will be update classbook proposal record
 * */


class CmdClassbookProposalUpdateHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookProposalUpdateHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

REGISTRY_WSJCPP_JSONRPC20_HANDLER(CmdClassbookProposalUpdateHandler)

/*!
 * This handler will be return classbook content (duplicate handler ? )
 * */

class CmdClassbookHandler : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdClassbookHandler();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

#endif // CMD_HADNLERS_CLASSBOOK_H
