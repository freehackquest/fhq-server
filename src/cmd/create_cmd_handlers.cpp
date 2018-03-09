#include "create_cmd_handlers.h"
#include <log.h>
#include <cmd_addhint_handler.h>
#include <cmd_answerlist_handler.h>
#include <cmd_api_handler.h>
#include <cmd_deletepublicevent_handler.h>
#include <cmd_createpublicevent_handler.h>
#include <cmd_getpublicevent_handler.h>
#include <cmd_deletehint_handler.h>
#include <cmd_feedback_add_handler.h>
#include <cmd_getmap_handler.h>
#include <cmd_public_info_handler.h>
#include <cmd_hints_handler.h>
#include <cmd_login_handler.h>
#include <cmd_publiceventslist_handler.h>
#include <cmd_registration_handler.h>
#include <cmd_scoreboard_handler.h>
#include <cmd_sendchatmessage_handler.h>
#include <cmd_token_handler.h>
#include <cmd_writeups_handler.h>

void create_cmd_handlers(QMap<std::string, ICmdHandler *> &pHandlers){
	QString TAG = "create_cmd_handlers";
	QVector<ICmdHandler *> v;


	v.push_back(new CmdAddHintHandler());
	v.push_back(new CmdAnswerListHandler());
	v.push_back(new CmdApiHandler());
	v.push_back(new CmdCreatePublicEventHandler());
	v.push_back(new CmdDeletePublicEventHandler());
	v.push_back(new CmdGetPublicEventHandler());
	v.push_back(new CmdDeleteHintHandler());
    v.push_back(new CmdFeedbackAddHandler());
	v.push_back(new CmdGetMapHandler());
	v.push_back(new CmdHintsHandler());
	v.push_back(new CmdLoginHandler());
	v.push_back(new CmdPublicEventsListHandler());
    v.push_back(new CmdPublicInfoHandler());
    v.push_back(new CmdRegistrationHandler());
	v.push_back(new CmdScoreboardHandler());
	v.push_back(new CmdSendChatMessageHandler());
    v.push_back(new CmdTokenHandler());
	v.push_back(new CmdWriteUpsHandler());

	for(int i = 0; i < v.size(); i++){
        std::string cmd = v[i]->cmd();
		if(pHandlers.contains(cmd)){
            Log::warn(TAG, "cmd_handler for command " + QString(cmd.c_str()) + " - already registered and will be skipped");
		}else{
			pHandlers[cmd] = v[i];
		}
	}
}
