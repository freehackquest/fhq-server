#include "create_cmd_handlers.h"
#include "headers/addhint.h"
#include "headers/answerlist.h"
#include "headers/classbook.h"
#include "headers/deletehint.h"
#include "headers/getmap.h"
#include "headers/getpublicinfo.h"
#include "headers/hello.h"
#include "headers/hints.h"
#include "headers/login.h"
#include "headers/scoreboard.h"
#include "headers/sendchatmessage.h"
#include "headers/send_letters_to_subscribers.h"
#include "headers/cmd_serverinfo_handler.h"
#include "headers/update_user_location.h"
#include "headers/user.h"
#include "headers/users.h"
#include "headers/writeups.h"

void create_cmd_handlers(QMap<QString, ICmdHandler *> &pHandlers){
	QVector<ICmdHandler *> v;

	v.push_back(new CmdAddHintHandler());
	v.push_back(new CmdAnswerListHandler());
	v.push_back(new CmdClassbookHandler());
	v.push_back(new CmdDeleteHintHandler());
	v.push_back(new CmdGetMapHandler());
	v.push_back(new CmdGetPublicInfoHandler());
	v.push_back(new CmdHelloHandler());
	v.push_back(new CmdHintsHandler());
	v.push_back(new CmdLoginHandler());
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
			qDebug() << "[WARNING] cmd_handler for command " << cmd << " - already registered and will be skipped";	
		}else{
			pHandlers[cmd] = v[i];
		}
	}
}
