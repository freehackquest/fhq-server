#include "create_cmd_handlers.h"
#include <log.h>
#include <cmd_addhint_handler.h>
#include <cmd_answerlist_handler.h>
#include <cmd_api_handler.h>
#include <cmd_deletepublicevent_handler.h>
#include <cmd_createpublicevent_handler.h>
#include <cmd_getpublicevent_handler.h>
#include <cmd_classbook_get_info_handler.h>
#include <cmd_classbook_get_list_handler.h>
#include <cmd_classbook_handler.h>
#include <cmd_deletehint_handler.h>
#include <cmd_deletequest_handler.h>
#include <cmd_feedback_add_handler.h>
#include <cmd_getmap_handler.h>
#include <cmd_getpublicinfo_handler.h>
#include <cmd_game_create_handler.h>
#include <cmd_game_delete_handler.h>
#include <cmd_game_export_handler.h>
#include <cmd_game_import_handler.h>
#include <cmd_game_info_handler.h>
#include <cmd_game_update_handler.h>
#include <cmd_game_update_logo_handler.h>
#include <cmd_games_handler.h>
#include <cmd_hello_handler.h>
#include <cmd_hints_handler.h>
#include <cmd_login_handler.h>
#include <cmd_publiceventslist_handler.h>
#include <cmd_quest_handler.h>
#include <cmd_quest_create_handler.h>
#include <cmd_quest_pass_handler.h>
#include <cmd_quests_subjects_handler.h>
#include <cmd_quests_handler.h>
#include <cmd_quest_statistics_handler.h>
#include <cmd_registration_handler.h>
#include <cmd_scoreboard_handler.h>
#include <cmd_sendchatmessage_handler.h>
#include <cmd_send_letters_to_subscribers_handler.h>
#include <cmd_serverinfo_handler.h>
#include <cmd_serversettings_handler.h>
#include <cmd_token_handler.h>
#include <cmd_updatequest_handler.h>
#include <cmd_update_user_location_handler.h>
#include <cmd_update_server_settings_handler.h>
#include <cmd_user_handler.h>
#include <cmd_users_handler.h>
#include <cmd_user_skills_handler.h>
#include <cmd_user_update_handler.h>
#include <cmd_user_change_password_handler.h>
#include <cmd_writeups_handler.h>

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
    v.push_back(new CmdClassbookGetInfoHandler());
    v.push_back(new CmdClassbookGetListHandler());
	v.push_back(new CmdClassbookHandler());
	v.push_back(new CmdDeleteHintHandler());
	v.push_back(new CmdDeleteQuestHandler());
    v.push_back(new CmdFeedbackAddHandler());
    v.push_back(new CmdGameCreateHandler());
    v.push_back(new CmdGameDeleteHandler());
    v.push_back(new CmdGameExportHandler());
    v.push_back(new CmdGameImportHandler());
    v.push_back(new CmdGameInfoHandler());
    v.push_back(new CmdGameUpdateHandler());
    v.push_back(new CmdGameUpdateLogoHandler());
	v.push_back(new CmdGamesHandler());
	v.push_back(new CmdGetPublicInfoHandler());
	v.push_back(new CmdGetMapHandler());
	v.push_back(new CmdHelloHandler());
	v.push_back(new CmdHintsHandler());
	v.push_back(new CmdLoginHandler());
	v.push_back(new CmdPublicEventsListHandler());
	v.push_back(new CmdQuestHandler());
    v.push_back(new CmdQuestPassHandler());
    v.push_back(new CmdQuestsSubjectsHandler());
    v.push_back(new CmdQuestStatisticsHandler());
    v.push_back(new CmdQuestsHandler());
    v.push_back(new CmdRegistrationHandler());
	v.push_back(new CmdScoreboardHandler());
	v.push_back(new CmdSendChatMessageHandler());
	v.push_back(new CmdSendLettersToSubscribersHandler());
	v.push_back(new CmdServerInfoHandler());
    v.push_back(new CmdServerSettingsHandler());
    v.push_back(new CmdTokenHandler());
	v.push_back(new CmdUpdateQuestHandler());
    v.push_back(new CmdUpdateUserLocationHandler());
    v.push_back(new CmdUpdateServerSettingsHandler());
    v.push_back(new CmdUsersHandler());
    v.push_back(new CmdUserHandler());
    v.push_back(new CmdUserChangePasswordHandler());
	v.push_back(new CmdUserSkillsHandler());
    v.push_back(new CmdUserUpdateHandler());
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
