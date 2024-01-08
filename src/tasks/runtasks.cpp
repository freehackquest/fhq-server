/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2024 FreeHackQuest <freehackquest@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

#include <runtasks.h>

#include <add_public_events_task.h>
#include <mail_send_task.h>
#include <notify_to_all_task.h>
#include <update_max_score_game_task.h>
#include <update_quest_solved_task.h>
#include <update_user_location_task.h>
#include <update_user_rating_task.h>
#include <wsjcpp_core.h>

#include <QRunnable>
#include <QThreadPool>
#include <lxd_async_operation_task.h>

void RunTasks::AddPublicEvents(const std::string &sType, const std::string &sMessage, const nlohmann::json &jsonMeta) {
  AddPublicEventsTask *pAddPublicEventsTask = new AddPublicEventsTask(sType, sMessage, jsonMeta);
  QThreadPool::globalInstance()->start(pAddPublicEventsTask);
  // TODO memory leak
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

void RunTasks::LXDAsyncOperation(void (*func)(const std::string &, std::string &, int &), const std::string &sName,
                                 const std::string &sCMD, ModelRequest *pRequest) {
  LXDAsyncOperationTask *pLXDAsyncTask = new LXDAsyncOperationTask(func, sName, sCMD, pRequest);
  QThreadPool::globalInstance()->start(pLXDAsyncTask);
}

void RunTasks::waitForDone() {
  while (!QThreadPool::globalInstance()->waitForDone(1000)) {
    WsjcppLog::info(std::string("RunTasks"), "Wait tasks for done...");
  }
}
