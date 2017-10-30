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
static const QString CMD_INPUT_DEF_TYPE_ANY = "any";
	
class CmdInputDef {
	public:
		CmdInputDef(QString name);
		CmdInputDef();
		CmdInputDef & optional();
		CmdInputDef & required();
		CmdInputDef & string_();
		CmdInputDef & integer_();
        CmdInputDef & any_();
		CmdInputDef & bool_();
		CmdInputDef & uuid_();
		CmdInputDef & enum_(QStringList list);
		CmdInputDef & description(QString s);
		CmdInputDef & minval(int minval);
		CmdInputDef & maxval(int maxval);
		QJsonObject toJson();
		
		QString getName();
		bool isRequired();
		bool isInteger();
		bool isEnum();
		bool isString();
		bool isUUID();
		bool isBool();
        bool isAny();
		
		QStringList getEnumList();
		bool isMinVal();
		int getMinVal();
		bool isMaxVal();
		int getMaxVal();
		
	private:
		QString m_sType;
		QString m_sName;
		QString m_sRestrict;
		QString m_sDescription;
		QStringList m_lstEnums;
		int m_nMinVal;
		bool m_bSettedMinVal;
		int m_nMaxVal;
		bool m_bSettedMaxVal;
};

#endif // CMD_INPUT_DEF_H
