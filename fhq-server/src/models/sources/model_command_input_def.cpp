#include <model_command_input_def.h>

// ---------------------------------------------------------------------

ModelCommandInputDef::ModelCommandInputDef(){

}

// ---------------------------------------------------------------------

CmdInputDef *ModelCommandInputDef::requiredString(const std::string &sName, const std::string &sDescription){
    CmdInputDef *pStringDef = new CmdInputDef(sName);
    pStringDef->string_().description(sDescription).required();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef*>(sName,pStringDef));
    return pStringDef;
}

// ---------------------------------------------------------------------

CmdInputDef *ModelCommandInputDef::optionalString(const std::string &sName, const std::string &sDescription){
    CmdInputDef *pStringDef = new CmdInputDef(sName);
    pStringDef->string_().description(sDescription).optional();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef*>(sName,pStringDef));
    return pStringDef;
}

// ---------------------------------------------------------------------

CmdInputDef *ModelCommandInputDef::defineBoolean(const std::string &sName, const std::string &sDescription){
    CmdInputDef *pBooleanDef = new CmdInputDef(sName);
    pBooleanDef->bool_().description(sDescription);
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef*>(sName,pBooleanDef));
    return pBooleanDef;
}

// ---------------------------------------------------------------------

CmdInputDef *ModelCommandInputDef::defineEnum(const std::string &sName, const std::string &sDescription){
    CmdInputDef *pEnumDef = new CmdInputDef(sName);
    pEnumDef->enum_(QStringList()).description(sDescription);
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef*>(sName,pEnumDef));
    return pEnumDef;
}

// ---------------------------------------------------------------------

CmdInputDef *ModelCommandInputDef::defineUUID(const std::string &sName, const std::string &sDescription){
    CmdInputDef *pUuidDef = new CmdInputDef(sName);
    pUuidDef->uuid_().description(sDescription);
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef*>(sName,pUuidDef));
    return pUuidDef;
}

// ---------------------------------------------------------------------

CmdInputDef *ModelCommandInputDef::defineInteger(const std::string &sName, const std::string &sDescription){
    CmdInputDef *pIntegerDef = new CmdInputDef(sName);
    pIntegerDef->integer_().description(sDescription);
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef*>(sName,pIntegerDef));
    return pIntegerDef;
}

// ---------------------------------------------------------------------

CmdInputDef *ModelCommandInputDef::defineAny(const std::string &sName, const std::string &sDescription){
    CmdInputDef *pAnyDef = new CmdInputDef(sName);
    pAnyDef->any_().description(sDescription);
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef*>(sName,pAnyDef));
    return pAnyDef;
}

// ---------------------------------------------------------------------
