#include "create_cmd_handlers.h"
#include <log.h>
#include <cmd_addhint_handler.h>
#include <cmd_answerlist_handler.h>
#include <cmd_api_handler.h>
#include <cmd_deletepublicevent_handler.h>
#include <cmd_createpublicevent_handler.h>
#include <cmd_getpublicevent_handler.h>
#include <cmd_classbook_add_record_handler.h>
#include <cmd_classbook_delete_record_handler.h>
#include <cmd_classbook_export_handler.h>
#include <cmd_classbook_info_handler.h>
#include <cmd_classbook_list_handler.h>
#include <cmd_classbook_search_handler.h>
#include <cmd_classbook_update_record_handler.h>
#include <cmd_classbook_localization_add_record_handler.h>
#include <cmd_classbook_localization_delete_record_handler.h>
#include <cmd_classbook_localization_info_handler.h>
#include <cmd_classbook_localization_update_record_handler.h>
#include <cmd_classbook_proposal_add_record_handler.h>
#include <cmd_classbook_proposal_delete_record_handler.h>
#include <cmd_classbook_proposal_info_handler.h>
#include <cmd_classbook_proposal_list_handler.h>
#include <cmd_classbook_handler.h>
#include <cmd_deletehint_handler.h>
#include <cmd_feedback_add_handler.h>
#include <cmd_getmap_handler.h>
#include <cmd_public_info_handler.h>
#include <cmd_hints_handler.h>
#include <cmd_login_handler.h>
#include <cmd_publiceventslist_handler.h>
#include <cmd_quest_delete_handler.h>
#include <cmd_quest_handler.h>
#include <cmd_quest_create_handler.h>
#include <cmd_quest_pass_handler.h>
#include <cmd_quests_subjects_handler.h>
#include <cmd_quests_handler.h>
#include <cmd_quest_statistics_handler.h>
#include <cmd_quest_update_handler.h>
#include <cmd_registration_handler.h>
#include <cmd_scoreboard_handler.h>
#include <cmd_sendchatmessage_handler.h>
#include <cmd_server_info_handler.h>
#include <cmd_server_settings_handler.h>
#include <cmd_server_settings_update_handler.h>
#include <cmd_token_handler.h>
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
    v.push_back(new CmdClassbookAddRecordHandler());
    v.push_back(new CmdClassbookDeleteRecordHandler());
    v.push_back(new CmdClassbookExportHandler());
    v.push_back(new CmdClassbookInfoHandler());
    v.push_back(new CmdClassbookListHandler());
    v.push_back(new CmdClassbookSearchHandler());
    v.push_back(new CmdClassbookUpdateRecordHandler());
    v.push_back(new CmdClassbookLocalizationAddRecordHandler());
    v.push_back(new CmdClassbookLocalizationDeleteRecordHandler());
    v.push_back(new CmdClassbookLocalizationInfoHandler());
    v.push_back(new CmdClassbookLocalizationUpdateRecordHandler());
    v.push_back(new CmdClassbookProposalAddRecordHandler());
    v.push_back(new CmdClassbookProposalDeleteRecordHandler());
    v.push_back(new CmdClassbookProposalInfoHandler());
    v.push_back(new CmdClassbookProposalListHandler());
	v.push_back(new CmdClassbookHandler());
	v.push_back(new CmdDeleteHintHandler());
    v.push_back(new CmdFeedbackAddHandler());
	v.push_back(new CmdGetMapHandler());
	v.push_back(new CmdHintsHandler());
	v.push_back(new CmdLoginHandler());
	v.push_back(new CmdPublicEventsListHandler());
    v.push_back(new CmdPublicInfoHandler());
    v.push_back(new CmdQuestDeleteHandler());
	v.push_back(new CmdQuestHandler());
    v.push_back(new CmdQuestPassHandler());
    v.push_back(new CmdQuestsSubjectsHandler());
    v.push_back(new CmdQuestStatisticsHandler());
    v.push_back(new CmdQuestUpdateHandler());
    v.push_back(new CmdQuestsHandler());
    v.push_back(new CmdRegistrationHandler());
	v.push_back(new CmdScoreboardHandler());
	v.push_back(new CmdSendChatMessageHandler());
	v.push_back(new CmdServerInfoHandler());
    v.push_back(new CmdServerSettingsHandler());
    v.push_back(new CmdServerSettingsUpdateHandler());
    v.push_back(new CmdTokenHandler());
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
