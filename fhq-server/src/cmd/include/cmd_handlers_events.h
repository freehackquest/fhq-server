#ifndef CMD_HADNLERS_EVENTS_H
#define CMD_HADNLERS_EVENTS_H

#include <cmd_handler_base.h>

/*!
 * This handler will be create public event
 * */

class CmdCreatePublicEventHandler : public ICmdHandler {

    public:
        CmdCreatePublicEventHandler();
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

REGISTRY_CMD(CmdCreatePublicEventHandler)

/*!
 * This handler will be delete public event
 * */

class CmdDeletePublicEventHandler : public ICmdHandler {

    public:
        CmdDeletePublicEventHandler();
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

REGISTRY_CMD(CmdDeletePublicEventHandler)

/*!
 * This handler will be retrun info about public event
 * */

class CmdGetPublicEventHandler : public ICmdHandler {

    public:
        CmdGetPublicEventHandler();
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

REGISTRY_CMD(CmdGetPublicEventHandler)

/*!
 * This handler will be retrun list of public events
 * */

class CmdPublicEventsListHandler : public ICmdHandler {

    public:
        CmdPublicEventsListHandler();
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

REGISTRY_CMD(CmdPublicEventsListHandler)


#endif // CMD_HADNLERS_EVENTS_H
