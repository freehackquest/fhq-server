#include "create_cmd_handlers.h"
#include <log.h>
#include <cmd_addhint_handler.h>
#include <cmd_answerlist_handler.h>
#include <cmd_deletepublicevent_handler.h>
#include <cmd_createpublicevent_handler.h>
#include <cmd_getpublicevent_handler.h>
#include <cmd_deletehint_handler.h>
#include <cmd_feedback_add_handler.h>
#include <cmd_hints_handler.h>
#include <cmd_publiceventslist_handler.h>
#include <cmd_writeups_handler.h>

void create_cmd_handlers(QMap<std::string, ICmdHandler *> &pHandlers){
	QString TAG = "create_cmd_handlers";
	QVector<ICmdHandler *> v;

    // quests / hints
	v.push_back(new CmdAddHintHandler());
    v.push_back(new CmdDeleteHintHandler());
    v.push_back(new CmdHintsHandler());

    // quests / writeups
    v.push_back(new CmdWriteUpsHandler());

    // quests / admin
	v.push_back(new CmdAnswerListHandler());

    // events
	v.push_back(new CmdCreatePublicEventHandler());
	v.push_back(new CmdDeletePublicEventHandler());
    v.push_back(new CmdPublicEventsListHandler());
    v.push_back(new CmdGetPublicEventHandler());

    // support
    v.push_back(new CmdFeedbackAddHandler());

	for(int i = 0; i < v.size(); i++){
        std::string cmd = v[i]->cmd();
		if(pHandlers.contains(cmd)){
            Log::warn(TAG, "cmd_handler for command " + QString(cmd.c_str()) + " - already registered and will be skipped");
		}else{
			pHandlers[cmd] = v[i];
		}
	}
}
