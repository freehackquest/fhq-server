#include <runtasks.h>
#include "headers/add_public_events_task.h"
#include "headers/update_user_location_task.h"
#include "headers/update_max_score_game_task.h"

#include <QRunnable>
#include <QThreadPool>

void RunTasks::AddPublicEvents(IWebSocketServer *pWebSocketServer, QString type, QString message){
	AddPublicEventsTask *pAddPublicEventsTask = new AddPublicEventsTask(pWebSocketServer, type, message);
	QThreadPool::globalInstance()->start(pAddPublicEventsTask);
}

void RunTasks::UpdateUserLocation(IWebSocketServer *pWebSocketServer, int userid, QString lastip){
	UpdateUserLocationTask *pUpdateUserLocationTask = new UpdateUserLocationTask(pWebSocketServer, userid, lastip);
	QThreadPool::globalInstance()->start(pUpdateUserLocationTask);
}

void RunTasks::UpdateMaxScoreGame(IWebSocketServer *pWebSocketServer, int gameid){
	UpdateMaxScoreGameTask *pUpdateMaxScoreGameTask = new UpdateMaxScoreGameTask(pWebSocketServer, gameid);
	QThreadPool::globalInstance()->start(pUpdateMaxScoreGameTask);
}
