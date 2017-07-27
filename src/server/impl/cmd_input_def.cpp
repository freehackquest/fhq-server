#include "../headers/cmd_input_def.h"
#include <QJsonDocument>
#include <QJsonObject>

CmdInputDef::CmdInputDef(QString name){
	m_sName = name;
}

// ---------------------------------------------------------------------

CmdInputDef::CmdInputDef(){
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::optional(){
	m_sRestrict = "optional";
	return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::required(){
	m_sRestrict = "required";
	return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::string_(){
	m_sType = CMD_INPUT_DEF_TYPE_STRING;
	return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::enum_(QStringList list){
	m_sType = CMD_INPUT_DEF_TYPE_ENUM;
	m_lstEnums = list;
	return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::integer_(){
	m_sType = CMD_INPUT_DEF_TYPE_INTEGER;
	return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::uuid_(){
	m_sType = CMD_INPUT_DEF_TYPE_UUID;
	return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::bool_(){
	m_sType = CMD_INPUT_DEF_TYPE_BOOL;
	return *this;
}

// ---------------------------------------------------------------------

CmdInputDef & CmdInputDef::description(QString s){
	m_sDescription = s;
	return *this;
}

// ---------------------------------------------------------------------

QJsonObject CmdInputDef::toJson(){
	QJsonObject obj;
	obj["name"] = m_sName;
	obj["type"] = m_sType;
	obj["restrict"] = m_sRestrict;
	obj["description"] = m_sDescription;
	// TODO enum
	return obj;
}

// ---------------------------------------------------------------------

QString CmdInputDef::getName(){
	return m_sName;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isRequired(){
	return m_sRestrict == "required";
}

// ---------------------------------------------------------------------

bool CmdInputDef::isInteger(){
	return m_sType == CMD_INPUT_DEF_TYPE_INTEGER;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isEnum(){
	return m_sType == CMD_INPUT_DEF_TYPE_ENUM;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isString(){
	return m_sType == CMD_INPUT_DEF_TYPE_STRING;
}
		
// ---------------------------------------------------------------------

bool CmdInputDef::isUUID(){
	return m_sType == CMD_INPUT_DEF_TYPE_UUID;
}

// ---------------------------------------------------------------------

bool CmdInputDef::isBool(){
	return m_sType == CMD_INPUT_DEF_TYPE_BOOL;
}

// ---------------------------------------------------------------------

QStringList CmdInputDef::getEnumList(){
	return m_lstEnums;
}

// ---------------------------------------------------------------------
