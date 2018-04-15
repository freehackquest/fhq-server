#ifndef CMD_HADNLERS_MAILS_H
#define CMD_HADNLERS_MAILS_H

#include <cmd_handlers.h>

/*!
 * This handler will be return info about mail
 * */

class CmdHandlerMailInfo : public CmdHandlerBase {

    public:
        CmdHandlerMailInfo();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerMailInfo)

/*!
 * This handler will be send mail by admin
 * */

class CmdHandlerMailSend : public CmdHandlerBase {

    public:
        CmdHandlerMailSend();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerMailSend)

/*!
 * This handler will be list of mails
 * */

class CmdHandlerMailsList : public CmdHandlerBase {

    public:
        CmdHandlerMailsList();
        virtual void handle(ModelRequest *pRequest);
};

REGISTRY_CMD(CmdHandlerMailsList)

#endif // CMD_HADNLERS_MAILS_H
