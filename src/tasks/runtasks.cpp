#include <runtasks.h>
#include <add_public_events_task.h>
#include <mail_send_task.h>
#include <update_max_score_game_task.h>
#include <update_quest_solved_task.h>
#include <update_user_location_task.h>
#include <update_user_rating_task.h>

#include <QRunnable>
#include <QThreadPool>

void RunTasks::AddPublicEvents(IWebSocketServer *pWebSocketServer, QString type, QString message){
	AddPublicEventsTask *pAddPublicEventsTask = new AddPublicEventsTask(pWebSocketServer, type, message);
	QThreadPool::globalInstance()->start(pAddPublicEventsTask);
}

void RunTasks::UpdateMaxScoreGame(IWebSocketServer *pWebSocketServer, int gameid){
    UpdateMaxScoreGameTask *pUpdateMaxScoreGameTask = new UpdateMaxScoreGameTask(pWebSocketServer, gameid);
    QThreadPool::globalInstance()->start(pUpdateMaxScoreGameTask);
}

void RunTasks::UpdateQuestSolved(IWebSocketServer *pWebSocketServer, int nQuestID){
    UpdateQuestSolvedTask *pUpdateQuestSolvedTask = new UpdateQuestSolvedTask(pWebSocketServer, nQuestID);
    QThreadPool::globalInstance()->start(pUpdateQuestSolvedTask);
}

void RunTasks::UpdateUserLocation(IWebSocketServer *pWebSocketServer, int userid, QString lastip){
	UpdateUserLocationTask *pUpdateUserLocationTask = new UpdateUserLocationTask(pWebSocketServer, userid, lastip);
	QThreadPool::globalInstance()->start(pUpdateUserLocationTask);
}

void RunTasks::UpdateUserRating(IWebSocketServer *pWebSocketServer, int nUserID){
    UpdateUserRatingTask *pUpdateUserRatingTask = new UpdateUserRatingTask(pWebSocketServer, nUserID);
    QThreadPool::globalInstance()->start(pUpdateUserRatingTask);
}

void RunTasks::MailSend(IWebSocketServer *pWebSocketServer,  QString to, QString subject, QString content){
    MailSendTask *pMailSendTask = new MailSendTask(pWebSocketServer, to, subject, content);
    QThreadPool::globalInstance()->start(pMailSendTask);
}


