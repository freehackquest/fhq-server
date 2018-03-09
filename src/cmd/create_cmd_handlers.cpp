#include "create_cmd_handlers.h"
#include <log.h>
#include <cmd_addhint_handler.h>
#include <cmd_answerlist_handler.h>
#include <cmd_deletehint_handler.h>
#include <cmd_hints_handler.h>
#include <cmd_writeups_handler.h>

void create_cmd_handlers(QMap<std::string, ICmdHandler *> &pHandlers){
	QString TAG = "create_cmd_handlers";
	QVector<ICmdHandler *> v;

    // quests
	v.push_back(new CmdAddHintHandler());
    v.push_back(new CmdDeleteHintHandler());
    v.push_back(new CmdHintsHandler());
    v.push_back(new CmdWriteUpsHandler());
	v.push_back(new CmdAnswerListHandler());




	for(int i = 0; i < v.size(); i++){
        std::string cmd = v[i]->cmd();
		if(pHandlers.contains(cmd)){
            Log::warn(TAG, "cmd_handler for command " + QString(cmd.c_str()) + " - already registered and will be skipped");
		}else{
			pHandlers[cmd] = v[i];
		}
	}
}
