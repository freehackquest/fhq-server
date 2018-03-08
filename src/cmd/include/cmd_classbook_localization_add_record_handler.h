#ifndef CMD_CLASSBOOK_LOCALIZATION_ADD_RECORD_H
#define CMD_CLASSBOOK_LOCALIZATION_ADD_RECORD_H

#include <iserver.h>
#include <cmd_handlers.h>

class CmdClassbookLocalizationAddRecordHandler : public ICmdHandler {

    public:
        CmdClassbookLocalizationAddRecordHandler();
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

#endif // CMD_CLASSBOOK_LOCALIZATION_ADD_RECORD_H

