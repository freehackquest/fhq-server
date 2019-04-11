#ifndef CMD_INPUT_DEF_H
#define CMD_INPUT_DEF_H

#include <json.hpp>
#include <QStringList>
#include <validators.h>

/*! 
 * Helper api methods description for input params in handlers
 * */
	
class CmdInputDef {
	public:
        CmdInputDef(const std::string &name);
		CmdInputDef();
		CmdInputDef & optional();
		CmdInputDef & required();
		CmdInputDef & string_();
		CmdInputDef & integer_();
        CmdInputDef & email_();
        CmdInputDef & any_();
		CmdInputDef & bool_();
		CmdInputDef & enum_(QStringList list);
        CmdInputDef & description(const std::string &sDescription);
		CmdInputDef & minval(int minval);
		CmdInputDef & maxval(int maxval);
        nlohmann::json toJson();
		
        const std::string &getType();
		const std::string &getType() const;
		const std::string &getName();
        const std::string &getName() const;
		const std::string &getRestrict();
		const std::string &getRestrict() const;
		const std::string &getDescription();
		const std::string &getDescription() const;

		bool isRequired();
		bool isInteger();
		bool isEnum(); // TODO: redesign to validators
		bool isString();
		bool isBool();
        bool isEmail(); // TODO: redesign to validators
        bool isAny();
		
		QStringList getEnumList(); // TODO: redesign to validators
		bool isMinVal(); // TODO: redesign to validators
		int getMinVal(); // TODO: redesign to validators
		bool isMaxVal(); // TODO: redesign to validators
		int getMaxVal(); // TODO: redesign to validators

		CmdInputDef &addValidator(ValidatorStringBase *pValidatorStringBase);
		
		const std::vector<ValidatorStringBase *> &listOfValidators();

	private:
        std::string m_sType;
        std::string m_sName;
        std::string m_sRestrict;
        std::string m_sDescription;
		QStringList m_lstEnums;
		int m_nMinVal;
		bool m_bSettedMinVal;
		int m_nMaxVal;
		bool m_bSettedMaxVal;

		std::string CMD_INPUT_DEF_TYPE_INTEGER = "integer";
		std::string CMD_INPUT_DEF_TYPE_STRING = "string";
		std::string CMD_INPUT_DEF_TYPE_BOOL = "boolean";
		std::string CMD_INPUT_DEF_TYPE_ENUM = "enum";
		std::string CMD_INPUT_DEF_TYPE_EMAIL = "email";
		std::string CMD_INPUT_DEF_TYPE_ANY = "any";

		std::vector<ValidatorStringBase *> m_vValidatorsString;
};

#endif // CMD_INPUT_DEF_H
