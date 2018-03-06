#ifndef CMD_GAME_UPDATE_HANDLER_H
#define CMD_GAME_UPDATE_HANDLER_H

#include <iserver.h>

class CmdGameUpdateHandler : public ICmdHandler {
	
    public:
        CmdGameUpdateHandler();
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

#endif // CMD_GAME_UPDATE_HANDLER_H
