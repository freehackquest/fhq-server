#include <runtasks.h>
#include <add_public_events_task.h>
#include <mail_send_task.h>
#include <update_max_score_game_task.h>
#include <update_quest_solved_task.h>
#include <update_user_location_task.h>
#include <update_user_rating_task.h>

#include <QRunnable>
#include <QThreadPool>

void RunTasks::AddPublicEvents(QString type, QString message){
    AddPublicEventsTask *pAddPublicEventsTask = new AddPublicEventsTask(type, message);
	QThreadPool::globalInstance()->start(pAddPublicEventsTask);
}

void RunTasks::UpdateMaxScoreGame(int gameid){
    UpdateMaxScoreGameTask *pUpdateMaxScoreGameTask = new UpdateMaxScoreGameTask(gameid);
    QThreadPool::globalInstance()->start(pUpdateMaxScoreGameTask);
}

void RunTasks::UpdateQuestSolved(int nQuestID){
    UpdateQuestSolvedTask *pUpdateQuestSolvedTask = new UpdateQuestSolvedTask(nQuestID);
    QThreadPool::globalInstance()->start(pUpdateQuestSolvedTask);
}

void RunTasks::UpdateUserLocation(int userid, QString lastip){
    UpdateUserLocationTask *pUpdateUserLocationTask = new UpdateUserLocationTask(userid, lastip);
	QThreadPool::globalInstance()->start(pUpdateUserLocationTask);
}

void RunTasks::UpdateUserRating(int nUserID){
    UpdateUserRatingTask *pUpdateUserRatingTask = new UpdateUserRatingTask(nUserID);
    QThreadPool::globalInstance()->start(pUpdateUserRatingTask);
}

void RunTasks::MailSend(IWebSocketServer *pWebSocketServer,  QString to, QString subject, QString content){
    MailSendTask *pMailSendTask = new MailSendTask(pWebSocketServer, to, subject, content);
    QThreadPool::globalInstance()->start(pMailSendTask);
}


