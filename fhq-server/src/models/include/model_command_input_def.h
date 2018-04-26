#ifndef MODEL_COMMAND_INPUT_DEF_H
#define MODEL_COMMAND_INPUT_DEF_H

#include <iwebsocketserver.h>
#include <cmd_input_def.h>

class ModelCommandInputDef {
    public:
        ModelCommandInputDef();

        CmdInputDef *requiredString(const std::string &sName, const std::string &sDescription);
        CmdInputDef *optionalString(const std::string &sName, const std::string &sDescription);
        CmdInputDef *defineBoolean(const std::string &sName, const std::string &sDescription);
        CmdInputDef *defineEnum(const std::string &sName, const std::string &sDescription);
        CmdInputDef *defineUUID(const std::string &sName, const std::string &sDescription);
        CmdInputDef *defineInteger(const std::string &sName, const std::string &sDescription);
        CmdInputDef *defineAny(const std::string &sName, const std::string &sDescription);

    private:
        std::map<std::string, CmdInputDef*> m_vInputsDef;
};

#endif // MODEL_COMMAND_INPUT_DEF_H
