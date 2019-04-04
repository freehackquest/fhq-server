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

class CmdHandlerUsefulLinksUpdate : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksUpdate();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksStar : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksStar();
        virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksUnstar : public CmdHandlerBase {
    public:
        CmdHandlerUsefulLinksUnstar();
        virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HANDLERS_USEFUL_LINKS_H
