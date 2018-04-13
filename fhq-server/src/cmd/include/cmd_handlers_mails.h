#ifndef CMD_HADNLERS_MAILS_H
#define CMD_HADNLERS_MAILS_H

#include <cmd_handler_base.h>

/*!
 * This handler will be return info about mail
 * */

class CmdHandlerMailInfo : public ICmdHandler {

    public:
        CmdHandlerMailInfo();
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

REGISTRY_CMD(CmdHandlerMailInfo)

/*!
 * This handler will be send mail by admin
 * */

class CmdHandlerMailSend : public ICmdHandler {

    public:
        CmdHandlerMailSend();
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

REGISTRY_CMD(CmdHandlerMailSend)

/*!
 * This handler will be list of mails
 * */

class CmdHandlerMailsList : public ICmdHandler {

    public:
        CmdHandlerMailsList();
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

REGISTRY_CMD(CmdHandlerMailsList)

#endif // CMD_HADNLERS_MAILS_H
