#ifndef CMD_USER_RESET_PASSWORD_REGISTRATION_HANDLER_H
#define CMD_USER_RESET_PASSWORD_REGISTRATION_HANDLER_H

#include <iserver.h>

class CmdUserResetPasswordHandler : public ICmdHandler {
	
	public:
        CmdUserResetPasswordHandler();
        virtual std::string cmd();
		virtual bool accessUnauthorized();
		virtual bool accessUser();
		virtual bool accessTester();
		virtual bool accessAdmin();
        virtual const std::vector<CmdInputDef> &inputs();
		virtual QString description();
		virtual QStringList errors();
        virtual void handle(ModelRequest *pRequest);

	private:
        std::vector<CmdInputDef> m_vInputs;
		QString TAG;
};

#endif // CMD_USER_RESET_PASSWORD_REGISTRATION_HANDLER_H
