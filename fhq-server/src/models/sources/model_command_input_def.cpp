#include <model_command_input_def.h>

// ---------------------------------------------------------------------

ModelCommandInputDef::ModelCommandInputDef(){

}

// ---------------------------------------------------------------------

CmdInputDef &ModelCommandInputDef::requiredString(const std::string &sName, const std::string &sDescription){
    CmdInputDef pStringDef(sName);
    pStringDef.string_().description(sDescription).required();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef>(sName,pStringDef));
    return m_vInputsDef[sName];;
}

// ---------------------------------------------------------------------

CmdInputDef &ModelCommandInputDef::optionalString(const std::string &sName, const std::string &sDescription){
    CmdInputDef pStringDef(sName);
    pStringDef.string_().description(sDescription).optional();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef>(sName,pStringDef));
    return m_vInputsDef[sName];
}

// ---------------------------------------------------------------------

CmdInputDef &ModelCommandInputDef::requiredBoolean(const std::string &sName, const std::string &sDescription){
    CmdInputDef pBooleanDef(sName);
    pBooleanDef.bool_().description(sDescription).required();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef>(sName,pBooleanDef));
    return m_vInputsDef[sName];
}

// ---------------------------------------------------------------------

CmdInputDef &ModelCommandInputDef::optionalBoolean(const std::string &sName, const std::string &sDescription){
    CmdInputDef pBooleanDef(sName);
    pBooleanDef.bool_().description(sDescription).optional();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef>(sName,pBooleanDef));
    return m_vInputsDef[sName];
}

// ---------------------------------------------------------------------

CmdInputDef &ModelCommandInputDef::requiredEnum(const std::string &sName, const std::string &sDescription){
    CmdInputDef pEnumDef(sName);
    pEnumDef.enum_(QStringList()).description(sDescription).required();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef>(sName,pEnumDef));
    return m_vInputsDef[sName];
}

// ---------------------------------------------------------------------

CmdInputDef &ModelCommandInputDef::optionalEnum(const std::string &sName, const std::string &sDescription){
    CmdInputDef pEnumDef(sName);
    pEnumDef.enum_(QStringList()).description(sDescription).optional();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef>(sName,pEnumDef));
    return m_vInputsDef[sName];
}

// ---------------------------------------------------------------------

CmdInputDef &ModelCommandInputDef::requiredUUID(const std::string &sName, const std::string &sDescription){
    CmdInputDef pUuidDef(sName);
    pUuidDef.uuid_().description(sDescription).required();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef>(sName,pUuidDef));
    return m_vInputsDef[sName];
}

// ---------------------------------------------------------------------

CmdInputDef &ModelCommandInputDef::optionalUUID(const std::string &sName, const std::string &sDescription){
    CmdInputDef pUuidDef(sName);
    pUuidDef.uuid_().description(sDescription).optional();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef>(sName,pUuidDef));
    return m_vInputsDef[sName];
}

// ---------------------------------------------------------------------

CmdInputDef &ModelCommandInputDef::requiredEmail(const std::string &sName, const std::string &sDescription){
    CmdInputDef def(sName);
    def.email_().description(sDescription).required();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef>(sName,def));
    return m_vInputsDef[sName];
}

// ---------------------------------------------------------------------

CmdInputDef &ModelCommandInputDef::optionalEmail(const std::string &sName, const std::string &sDescription){
    CmdInputDef def(sName);
    def.email_().description(sDescription).optional();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef>(sName,def));
    return m_vInputsDef[sName];
}

// ---------------------------------------------------------------------

CmdInputDef &ModelCommandInputDef::requiredInteger(const std::string &sName, const std::string &sDescription){
    CmdInputDef pIntegerDef(sName);
    pIntegerDef.integer_().description(sDescription).required();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef>(sName,pIntegerDef));
    return m_vInputsDef[sName];
}

// ---------------------------------------------------------------------

CmdInputDef &ModelCommandInputDef::optionalInteger(const std::string &sName, const std::string &sDescription){
    CmdInputDef pIntegerDef(sName);
    pIntegerDef.integer_().description(sDescription).optional();
    m_vInputsDef.insert(std::pair<std::string, CmdInputDef>(sName,pIntegerDef));
    return m_vInputsDef[sName];
}

// ---------------------------------------------------------------------

void ModelCommandInputDef::push_back(const CmdInputDef &cmdInputDef){
	std::string sName = cmdInputDef.getName();
	m_vInputsDef.insert(std::pair<std::string, CmdInputDef>(sName,cmdInputDef));
}
