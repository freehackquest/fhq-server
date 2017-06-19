#ifndef CMD_INPUT_DEF_H
#define CMD_INPUT_DEF_H

#include <QString>
#include <QJsonObject>

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

	private:
		QString m_sType;
		QString m_sName;
		QString m_sRestrict;
};

#endif // CMD_INPUT_DEF_H
