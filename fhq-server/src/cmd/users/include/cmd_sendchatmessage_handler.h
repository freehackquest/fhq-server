#ifndef CMD_SENDCHATMESSAGE_HANDLER_H
#define CMD_SENDCHATMESSAGE_HANDLER_H

#include <iserver.h>
#include <cmd_handlers.h>

class CmdSendChatMessageHandler : public ICmdHandler {
	
	public:
		CmdSendChatMessageHandler();
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

REGISTRY_CMD(CmdSendChatMessageHandler)

#endif // CMD_SENDCHATMESSAGE_HANDLER_H
