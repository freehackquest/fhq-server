#include "create_cmd_users_handlers.h"
#include <log.h>
#include <cmd_update_user_location_handler.h>
#include <cmd_user_handler.h>
#include <cmd_user_reset_password_handler.h>
#include <cmd_users_handler.h>
#include <cmd_user_skills_handler.h>
#include <cmd_user_update_handler.h>
#include <cmd_user_change_password_handler.h>

void create_cmd_users_handlers(QMap<std::string, ICmdHandler *> &pHandlers){
    QString TAG = "create_cmd_users_handlers";
	QVector<ICmdHandler *> v;

    v.push_back(new CmdUpdateUserLocationHandler());
    v.push_back(new CmdUsersHandler());
    v.push_back(new CmdUserHandler());
    v.push_back(new CmdUserResetPasswordHandler());
    v.push_back(new CmdUserChangePasswordHandler());
	v.push_back(new CmdUserSkillsHandler());
    v.push_back(new CmdUserUpdateHandler());

	for(int i = 0; i < v.size(); i++){
        std::string cmd = v[i]->cmd();
		if(pHandlers.contains(cmd)){
            Log::warn(TAG, "cmd_handler for command " + QString(cmd.c_str()) + " - already registered and will be skipped");
		}else{
			pHandlers[cmd] = v[i];
		}
	}
}
