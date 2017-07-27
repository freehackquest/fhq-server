#ifndef CMD_INPUT_DEF_H
#define CMD_INPUT_DEF_H

#include <QString>
#include <QJsonObject>
#include <QStringList>

/*! 
 * Helper api methods description for input params in handlers
 * */

static const QString CMD_INPUT_DEF_TYPE_STRING = "string";
static const QString CMD_INPUT_DEF_TYPE_BOOL = "boolean";
static const QString CMD_INPUT_DEF_TYPE_ENUM = "enum";
static const QString CMD_INPUT_DEF_TYPE_UUID = "uuid";
static const QString CMD_INPUT_DEF_TYPE_INTEGER = "integer";
	
class CmdInputDef {
	public:
		CmdInputDef(QString name);
		CmdInputDef();
		CmdInputDef & optional();
		CmdInputDef & required();
		CmdInputDef & string_();
		CmdInputDef & integer_();
		CmdInputDef & bool_();
		CmdInputDef & uuid_();
		CmdInputDef & enum_(QStringList list);
		CmdInputDef & description(QString s);
		QJsonObject toJson();
		
		QString getName();
		bool isRequired();
		bool isInteger();
		bool isEnum();
		bool isString();
		bool isUUID();
		bool isBool();
		
		QStringList getEnumList();
		
	private:
		QString m_sType;
		QString m_sName;
		QString m_sRestrict;
		QString m_sDescription;
		QStringList m_lstEnums;
};

#endif // CMD_INPUT_DEF_H
