#ifndef CMD_HANDLERS_USEFUL_LINKS_H
#define CMD_HANDLERS_USEFUL_LINKS_H

#include <cmd_handlers.h>

/*********************************************
 * Useful Links List
**********************************************/

class CmdHandlerUsefulLinksList : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksList();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksRetrieve : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksRetrieve();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksAdd : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksAdd();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksDelete : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksDelete();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Update useful link

class CmdHandlerUsefulLinksUpdate : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksUpdate();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links list of favorite

class CmdHandlerUsefulLinksUserFavoriteList : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksUserFavoriteList();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links add to favorite

class CmdHandlerUsefulLinksUserFavorite : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksUserFavorite();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links removed from favorite

class CmdHandlerUsefulLinksUserUnfavorite : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksUserUnfavorite();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links counter clicker

class CmdHandlerUsefulLinksClicked : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksClicked();
        virtual void handle(ModelRequest *pRequest);
};


// ---------------------------------------------------------------------
// Useful Links list of comments

class CmdHandlerUsefulLinksCommentList : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksCommentList();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links add comment

class CmdHandlerUsefulLinksCommentAdd : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksCommentAdd();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links remove comment

class CmdHandlerUsefulLinksCommentDelete : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksCommentDelete();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links List of tags

class CmdHandlerUsefulLinksTagList : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksTagList();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links add tag

class CmdHandlerUsefulLinksTagAdd : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksTagAdd();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links remove tag

class CmdHandlerUsefulLinksTagDelete : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksTagDelete();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links propose link by user

class CmdHandlerUsefulLinksUserPropose : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksUserPropose();
        virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// Useful Links Approve propose link by admin

class CmdHandlerUsefulLinksUserProposeApprove : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksUserProposeApprove();
        virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HANDLERS_USEFUL_LINKS_H
