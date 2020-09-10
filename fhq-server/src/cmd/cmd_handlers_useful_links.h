#ifndef CMD_HANDLERS_USEFUL_LINKS_H
#define CMD_HANDLERS_USEFUL_LINKS_H

#include <wsjcpp_jsonrpc20.h>

/*********************************************
 * Useful Links List
**********************************************/

class CmdHandlerUsefulLinksList : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerUsefulLinksRetrieve : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksRetrieve();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerUsefulLinksAdd : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksAdd();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

class CmdHandlerUsefulLinksDelete : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksDelete();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Update useful link

class CmdHandlerUsefulLinksUpdate : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksUpdate();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links list of favorite

class CmdHandlerUsefulLinksUserFavoriteList : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksUserFavoriteList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links add to favorite

class CmdHandlerUsefulLinksUserFavorite : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksUserFavorite();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links removed from favorite

class CmdHandlerUsefulLinksUserUnfavorite : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksUserUnfavorite();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links counter clicker

class CmdHandlerUsefulLinksClicked : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksClicked();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};


// ---------------------------------------------------------------------
// Useful Links list of comments

class CmdHandlerUsefulLinksCommentList : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksCommentList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links add comment

class CmdHandlerUsefulLinksCommentAdd : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksCommentAdd();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links remove comment

class CmdHandlerUsefulLinksCommentDelete : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksCommentDelete();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links List of tags

class CmdHandlerUsefulLinksTagList : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksTagList();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links add tag

class CmdHandlerUsefulLinksTagAdd : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksTagAdd();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links remove tag

class CmdHandlerUsefulLinksTagDelete : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksTagDelete();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links propose link by user

class CmdHandlerUsefulLinksUserPropose : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksUserPropose();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links Approve propose link by admin

class CmdHandlerUsefulLinksUserProposeApprove : public WsjcppJsonRpc20HandlerBase {
    public:
        CmdHandlerUsefulLinksUserProposeApprove();
        virtual void handle(WsjcppJsonRpc20Request *pRequest);
};

#endif // CMD_HANDLERS_USEFUL_LINKS_H
