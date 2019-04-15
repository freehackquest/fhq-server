#ifndef CMD_HADNLERS_H
#define CMD_HADNLERS_H

#include <map>
#include <utils_logger.h>
#include <model_request.h>
#include <validators_base.h>

/*! 
 * CmdInputDef - helper api for define input params and descrip it for docs.
 * */
	
class CmdInputDef {
	public:
        CmdInputDef(const std::string &sName, const std::string &sDescription);
		CmdInputDef();
		CmdInputDef & optional();
		CmdInputDef & required();
		CmdInputDef & string_();
		CmdInputDef & integer_();
        CmdInputDef & any_();
		CmdInputDef & bool_();
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
		bool isString();
		bool isBool();
        bool isAny();

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
		int m_nMinVal;
		bool m_bSettedMinVal;
		int m_nMaxVal;
		bool m_bSettedMaxVal;

		std::string CMD_INPUT_DEF_TYPE_INTEGER = "integer";
		std::string CMD_INPUT_DEF_TYPE_STRING = "string";
		std::string CMD_INPUT_DEF_TYPE_BOOL = "boolean";
		std::string CMD_INPUT_DEF_TYPE_ANY = "any";

		std::vector<ValidatorStringBase *> m_vValidatorsString;
};

// ---------------------------------------------------------------------

class CmdHandlerBase {

    public:
        CmdHandlerBase(const std::string &sCmd, const std::string &sDescription);
        virtual std::string cmd();
        virtual std::string description();
        std::string activatedFromVersion();
        std::string deprecatedFromVersion();
        bool accessUnauthorized();
        bool accessUser();
        bool accessAdmin();

        virtual const std::vector<CmdInputDef> &inputs();
        virtual void handle(ModelRequest *pRequest) = 0;

        // virtual void success(nlohmann::json jsonResponse);
        // virtual void error(int nCode, const std::string &sErrorMessage);
        
    protected:
        void setAccessUnauthorized(bool bAccess);
        void setAccessUser(bool bAccess);
        void setAccessAdmin(bool bAccess);
        void setActivatedFromVersion(const std::string &sActivatedFromVersion);
        void setDeprecatedFromVersion(const std::string &sDeprecatedFromVersion);

        CmdInputDef &requireStringParam(const std::string &sName, const std::string &sDescription);
        CmdInputDef &optionalStringParam(const std::string &sName, const std::string &sDescription);
        CmdInputDef &requireIntegerParam(const std::string &sName, const std::string &sDescription);
        CmdInputDef &optionalIntegerParam(const std::string &sName, const std::string &sDescription);
        CmdInputDef &requireBooleanParam(const std::string &sName, const std::string &sDescription);
        CmdInputDef &optionalBooleanParam(const std::string &sName, const std::string &sDescription);

        std::string TAG;
        std::string m_sCmd;
        std::string m_sDescription;

    private:
        std::vector<CmdInputDef> m_vInputs; // TODO redesign to map
        // std::map<std::string, CmdInputDef*> *m_vCmdInputDefs;
        std::string m_sActivatedFromVersion;
        std::string m_sDeprecatedFromVersion;
        bool m_bAccessUnauthorized;
        bool m_bAccessUser;
        bool m_bAccessAdmin;
};

extern std::map<std::string, CmdHandlerBase*> *g_pCmdHandlers;

class CmdHandlers {
    public:
        static void initGlobalVariables();
        static void addHandler(const std::string &sName, CmdHandlerBase* pCmdHandler);
        static CmdHandlerBase *findCmdHandler(const std::string &sCmd);
};

// RegistryCmdHandler
#define REGISTRY_CMD( classname ) \
    static classname * pRegistry ## classname = new classname(); \


#endif // CMD_HADNLERS_H
