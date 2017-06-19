#ifndef CMD_INPUT_DEF_H
#define CMD_INPUT_DEF_H

#include <QString>
#include <QJsonObject>
#include <QStringList>

/*! 
 * Helper api methods description for input params in handlers
 * */

class CmdInputDef {
	public:
		CmdInputDef(QString name);
		CmdInputDef();
		CmdInputDef & optional();
		CmdInputDef & required();
		CmdInputDef & string_();
		CmdInputDef & integer_();
		CmdInputDef & bool_();
		CmdInputDef & enum_(QStringList list);
		CmdInputDef & description(QString s);
		QJsonObject toJson();
		
	private:
		QString m_sType;
		QString m_sName;
		QString m_sRestrict;
		QString m_sDescription;
		QStringList m_lstEnums;
};

#endif // CMD_INPUT_DEF_H
