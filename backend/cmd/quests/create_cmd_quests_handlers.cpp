#include "create_cmd_quests_handlers.h"
#include <cmd_quest_proposal_handler.h>
#include <log.h>


void create_cmd_quests_handlers(QMap<std::string, ICmdHandler *> &pHandlers){
    QString TAG = "create_cmd_quests_handlers";
	QVector<ICmdHandler *> v;

    v.push_back(new CmdQuestProposalHandler());

	for(int i = 0; i < v.size(); i++){
        std::string cmd = v[i]->cmd();
		if(pHandlers.contains(cmd)){
            Log::warn(TAG, "cmd_handler for command " + QString(cmd.c_str()) + " - already registered and will be skipped");
		}else{
			pHandlers[cmd] = v[i];
		}
	}
}
