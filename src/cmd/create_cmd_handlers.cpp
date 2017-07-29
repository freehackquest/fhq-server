#include "create_cmd_handlers.h"
#include <log.h>
#include "headers/cmd_addhint_handler.h"
#include "headers/cmd_answerlist_handler.h"
#include "headers/cmd_api_handler.h"
#include "headers/cmd_deletepublicevent_handler.h"
#include "headers/cmd_createpublicevent_handler.h"
#include "headers/cmd_createquest_handler.h"
#include "headers/cmd_getpublicevent_handler.h"
#include "headers/cmd_classbook_handler.h"
#include "headers/cmd_deletehint_handler.h"
#include "headers/cmd_deletequest_handler.h"
#include "headers/cmd_getmap_handler.h"
#include "headers/cmd_getpublicinfo_handler.h"
#include "headers/cmd_games_handler.h"
#include "headers/cmd_hello_handler.h"
#include "headers/cmd_hints_handler.h"
#include "headers/cmd_login_handler.h"
#include "headers/cmd_publiceventslist_handler.h"
#include "headers/cmd_scoreboard_handler.h"
#include "headers/cmd_sendchatmessage_handler.h"
#include "headers/cmd_send_letters_to_subscribers_handler.h"
#include "headers/cmd_serverinfo_handler.h"
#include "headers/cmd_update_user_location_handler.h"
#include "headers/cmd_user_handler.h"
#include "headers/cmd_users_handler.h"
#include "headers/cmd_writeups_handler.h"

void create_cmd_handlers(QMap<QString, ICmdHandler *> &pHandlers){
	QString TAG = "create_cmd_handlers";
	QVector<ICmdHandler *> v;

	v.push_back(new CmdAddHintHandler());
	v.push_back(new CmdAnswerListHandler());
	v.push_back(new CmdApiHandler());
	v.push_back(new CmdCreatePublicEventHandler());
	v.push_back(new CmdCreateQuestHandler());
	v.push_back(new CmdDeletePublicEventHandler());
	v.push_back(new CmdGetPublicEventHandler());
	v.push_back(new CmdClassbookHandler());
	v.push_back(new CmdDeleteHintHandler());
	v.push_back(new CmdDeleteQuestHandler());
	v.push_back(new CmdGamesHandler());
	v.push_back(new CmdGetPublicInfoHandler());
	v.push_back(new CmdGetMapHandler());
	v.push_back(new CmdHelloHandler());
	v.push_back(new CmdHintsHandler());
	v.push_back(new CmdLoginHandler());
	v.push_back(new CmdPublicEventsListHandler());
	v.push_back(new CmdScoreboardHandler());
	v.push_back(new CmdSendChatMessageHandler());
	v.push_back(new CmdSendLettersToSubscribersHandler());
	v.push_back(new CmdServerInfoHandler());
	v.push_back(new CmdUpdateUserLocationHandler());
	v.push_back(new CmdUserHandler());
	v.push_back(new CmdUsersHandler());
	v.push_back(new CmdWriteUpsHandler());

	for(int i = 0; i < v.size(); i++){
		QString cmd = v[i]->cmd();
		if(pHandlers.contains(cmd)){
			Log::warn(TAG, "cmd_handler for command " + cmd + " - already registered and will be skipped");
		}else{
			pHandlers[cmd] = v[i];
		}
	}
}
