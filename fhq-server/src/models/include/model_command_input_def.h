#ifndef MODEL_COMMAND_INPUT_DEF_H
#define MODEL_COMMAND_INPUT_DEF_H

#include <iwebsocketserver.h>
#include <cmd_input_def.h>

class ModelCommandInputDef {
    public:
        ModelCommandInputDef();

		// string
        CmdInputDef &requiredString(const std::string &sName, const std::string &sDescription);
        CmdInputDef &optionalString(const std::string &sName, const std::string &sDescription);
        
        // boolean
        CmdInputDef &requiredBoolean(const std::string &sName, const std::string &sDescription);
        CmdInputDef &optionalBoolean(const std::string &sName, const std::string &sDescription);
        
        // enum
        CmdInputDef &requiredEnum(const std::string &sName, const std::string &sDescription);
        CmdInputDef &optionalEnum(const std::string &sName, const std::string &sDescription);
        
        // uuid
        CmdInputDef &requiredUUID(const std::string &sName, const std::string &sDescription);
        CmdInputDef &optionalUUID(const std::string &sName, const std::string &sDescription);
        
        // email
        CmdInputDef &requiredEmail(const std::string &sName, const std::string &sDescription);
        CmdInputDef &optionalEmail(const std::string &sName, const std::string &sDescription);
        
        // integer
        CmdInputDef &requiredInteger(const std::string &sName, const std::string &sDescription);
        CmdInputDef &optionalInteger(const std::string &sName, const std::string &sDescription);
        
        void push_back(const CmdInputDef &cmdInputDef);

    private:
        std::map<std::string, CmdInputDef> m_vInputsDef;
};

#endif // MODEL_COMMAND_INPUT_DEF_H
