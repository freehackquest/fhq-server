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

#include <QtCore>
#include <cmd_handlers_support.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <iostream>
#include <runtasks.h>

// ---------------------------------------------------------------------
// This handler for add support

REGISTRY_CMD(CmdHandlerFeedbackAdd)

CmdHandlerFeedbackAdd::CmdHandlerFeedbackAdd() : CmdHandlerBase("feedback_add", "Create the feedback") {

  setAccessUnauthorized(true);
  setAccessUser(true);
  setAccessAdmin(true);

  // validation and description input fields
  requireStringParam("from", "From user").addValidator(new WsjcppValidatorEmail());
  requireStringParam("text", "Text of feedback");
  requireStringParam("type", "Type of feedback"); // TODO validator
}

// ---------------------------------------------------------------------

void CmdHandlerFeedbackAdd::handle(ModelRequest *pRequest) {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  nlohmann::json jsonResponse;

  QSqlDatabase db = *(pDatabase->database());

  int nUserID = 0;
  std::string sEmail = pRequest->getInputString("from", "");
  sEmail = WsjcppCore::trim(sEmail);
  std::string sText = pRequest->getInputString("text", "");
  sText = WsjcppCore::trim(sText);
  std::string sType = pRequest->getInputString("type", "");
  sType = WsjcppCore::trim(sType);

  WsjcppUserSession *pUserSession = pRequest->getUserSession();
  if (pUserSession != NULL) {
    sEmail = pUserSession->email().toStdString();
    nUserID = pUserSession->userid();
  }
  EmployGlobalSettings *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

  QSqlQuery query(db);
  query.prepare("INSERT INTO feedback(`type`, `from`, `text`, `userid`, `dt`) "
                "VALUES(:type,:from,:text,:userid,NOW());");
  query.bindValue(":type", QString::fromStdString(sType));
  query.bindValue(":from", QString::fromStdString(sEmail));
  query.bindValue(":text", QString::fromStdString(sText));
  query.bindValue(":userid", nUserID);
  if (!query.exec()) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
    return;
  }

  nlohmann::json jsonMeta;
  RunTasks::AddPublicEvents("users", "Added feedback", jsonMeta);

  // TODO move to EmployMails
  std::string sMailToAdmin = pGlobalSettings->get("mail_system_message_admin_email").getStringValue();
  std::string sSubject = "Feedback (freehackquest.com)";
  std::string sContext = "Feedback \n"
                         "Type: " +
                         sType +
                         "\n"
                         "From: " +
                         sEmail +
                         "\n"
                         "Text: \n" +
                         sText + "\n";

  RunTasks::MailSend(sMailToAdmin, sSubject, sContext);

  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
