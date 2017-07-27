#ifndef TASKS_H
#define TASKS_H

#include <add_public_events_task.h>
#include <update_user_location_task.h>
#include <update_max_score_game_task.h>

#include <QRunnable>
#include <QThreadPool>

static void Run_AddPublicEventsTask(IWebSocketServer *pWebSocketServer, QString type, QString message){
	AddPublicEventsTask *pAddPublicEventsTask = new AddPublicEventsTask(pWebSocketServer, type, message);
	QThreadPool::globalInstance()->start(pAddPublicEventsTask);
}

static void Run_UpdateUserLocationTask(IWebSocketServer *pWebSocketServer, int userid, QString lastip){
	UpdateUserLocationTask *pUpdateUserLocationTask = new UpdateUserLocationTask(pWebSocketServer, userid, lastip);
	QThreadPool::globalInstance()->start(pUpdateUserLocationTask);
}

static void Run_UpdateMaxScoreGameTask(IWebSocketServer *pWebSocketServer, int gameid){
	UpdateMaxScoreGameTask *pUpdateMaxScoreGameTask = new UpdateMaxScoreGameTask(pWebSocketServer, gameid);
	QThreadPool::globalInstance()->start(pUpdateMaxScoreGameTask);
}

#endif // TASKS_H
