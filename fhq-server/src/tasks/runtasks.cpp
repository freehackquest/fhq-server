#include <runtasks.h>
#include <fallen.h>
#include <add_public_events_task.h>
#include <mail_send_task.h>
#include <notify_to_all_task.h>
#include <update_max_score_game_task.h>
#include <update_quest_solved_task.h>
#include <update_user_location_task.h>
#include <update_user_rating_task.h>

#include <QRunnable>
#include <QThreadPool>
#include <lxd_async_operation_task.h>

void RunTasks::AddPublicEvents(const std::string &type, const std::string &message) {
    AddPublicEventsTask *pAddPublicEventsTask = new AddPublicEventsTask(type, message);
    QThreadPool::globalInstance()->start(pAddPublicEventsTask);
}

void RunTasks::UpdateMaxScoreGame(int gameid) {
    UpdateMaxScoreGameTask *pUpdateMaxScoreGameTask = new UpdateMaxScoreGameTask(gameid);
    QThreadPool::globalInstance()->start(pUpdateMaxScoreGameTask);
}

void RunTasks::UpdateQuestSolved(int nQuestID) {
    UpdateQuestSolvedTask *pUpdateQuestSolvedTask = new UpdateQuestSolvedTask(nQuestID);
    QThreadPool::globalInstance()->start(pUpdateQuestSolvedTask);
}

void RunTasks::UpdateUserLocation(int userid, const std::string &sLastip) {
    UpdateUserLocationTask *pUpdateUserLocationTask = new UpdateUserLocationTask(userid, sLastip);
    QThreadPool::globalInstance()->start(pUpdateUserLocationTask);
}

void RunTasks::UpdateUserRating(int nUserID) {
    UpdateUserRatingTask *pUpdateUserRatingTask = new UpdateUserRatingTask(nUserID);
    QThreadPool::globalInstance()->start(pUpdateUserRatingTask);
}

void RunTasks::MailSend(const std::string &sTo, const std::string &sSubject, const std::string &sContent) {
    MailSendTask *pMailSendTask = new MailSendTask(sTo, sSubject, sContent);
    QThreadPool::globalInstance()->start(pMailSendTask);
}

void RunTasks::NotifyToAll(const nlohmann::json &jsonMessage) {
    NotifyToAllTask *pNotifyToAllTask = new NotifyToAllTask(jsonMessage);
    QThreadPool::globalInstance()->start(pNotifyToAllTask);
}

void RunTasks::LXDAsyncOperation(void (*func)(const std::string &, std::string &, int &),
                                 const std::string &sName, const std::string &sCMD, ModelRequest *pRequest) {
    LXDAsyncOperationTask *pLXDAsyncTask = new LXDAsyncOperationTask(func, sName, sCMD, pRequest);
    QThreadPool::globalInstance()->start(pLXDAsyncTask);
}

void RunTasks::waitForDone() {
    while (!QThreadPool::globalInstance()->waitForDone(1000)) {
        WsjcppLog::info(std::string("RunTasks"), "Wait tasks for done...");
    }
}



