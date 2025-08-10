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
#include <cmd_handlers_mails.h>
#include <employ_database.h>
#include <employ_server_info.h>
#include <iostream>
#include <runtasks.h>

// ---------------------------------------------------------------------
// This handler will be return info about mail

REGISTRY_CMD(CmdHandlerMailInfo)

CmdHandlerMailInfo::CmdHandlerMailInfo() : CmdHandlerBase("mail_info", "This method Will be return info of mail") {
  TAG = "CmdHandlerMailInfo";

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);
}

// ---------------------------------------------------------------------

void CmdHandlerMailInfo::handle(ModelRequest *pRequest) {

  pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(501, "Not Implemented Yet"));
}

// ---------------------------------------------------------------------
// This handler will be send mail by admin

REGISTRY_CMD(CmdHandlerMailSend)

CmdHandlerMailSend::CmdHandlerMailSend() : CmdHandlerBase("mail_send", "Mail Send") {

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  // validation and description input fields
  requireStringParam("to", "E-mail of the recipient").addValidator(new WsjcppValidatorEmail());
  requireStringParam("subject", "Subject of the message");
  requireStringParam("body", "Body of the message");
}

// ---------------------------------------------------------------------

void CmdHandlerMailSend::handle(ModelRequest *pRequest) {
  nlohmann::json jsonResponse;

  std::string sEmail = pRequest->getInputString("to", "");
  std::string sSubject = pRequest->getInputString("subject", "");
  std::string sBody = pRequest->getInputString("body", "");

  RunTasks::MailSend(sEmail, sSubject, sBody);

  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// This handler will be list of mails

REGISTRY_CMD(CmdHandlerMailsList)

CmdHandlerMailsList::CmdHandlerMailsList() : CmdHandlerBase("mails_list", "Mails list") {
  TAG = "CmdHandlerMailsList";

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  optionalStringParam("filter_email", "Filter by email");
  optionalStringParam("filter_subject", "Filter by subject");
  optionalStringParam("filter_message", "Filter by message");
  optionalIntegerParam("onpage", "On page"); // TODO validator
  optionalIntegerParam("page", "page");      // TODO validator
}

// ---------------------------------------------------------------------

void CmdHandlerMailsList::handle(ModelRequest *pRequest) {
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  nlohmann::json jsonResponse;

  std::vector<std::string> vFilters;
  QMap<QString, QString> filter_values;

  int nPage = pRequest->getInputInteger("page", 0);
  int nOnPage = pRequest->getInputInteger("onpage", 5);
  int nCount = 0;

  std::string sFilterEmail = pRequest->getInputString("filter_email", "");
  WsjcppCore::trim(sFilterEmail);
  if (sFilterEmail != "") {
    vFilters.push_back("(ed.to_email LIKE :email)");
    filter_values[":email"] = "%" + QString::fromStdString(sFilterEmail) + "%";
  }

  std::string sFilterSubject = pRequest->getInputString("filter_subject", "");
  WsjcppCore::trim(sFilterSubject);
  if (sFilterSubject != "") {
    vFilters.push_back("(ed.subject LIKE :subject)");
    filter_values[":subject"] = "%" + QString::fromStdString(sFilterSubject) + "%";
  }

  std::string sFilterMessage = pRequest->getInputString("filter_message", "");
  WsjcppCore::trim(sFilterMessage);
  if (sFilterMessage != "") {
    vFilters.push_back("(ed.message LIKE :message)");
    filter_values[":message"] = "%" + QString::fromStdString(sFilterMessage) + "%";
  }

  std::string sWhere = "";
  for (int i = 0; i < vFilters.size(); i++) {
    if (sWhere.length() > 0) {
      sWhere += " AND ";
    }
    sWhere += vFilters[i];
  }
  if (sWhere.length() > 0) {
    sWhere = "WHERE " + sWhere;
  }

  QSqlDatabase db = *(pDatabase->database());

  // calculate count
  {
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) cnt FROM email_delivery ed " + QString::fromStdString(sWhere));
    foreach (QString key, filter_values.keys()) {
      query.bindValue(key, filter_values.value(key));
    }
    if (!query.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
      return;
    }
    if (query.next()) {
      QSqlRecord record = query.record();
      nCount = record.value("cnt").toInt();
    }
  }

  // emails
  nlohmann::json jsonEmails = nlohmann::json::array();
  {
    QSqlQuery query(db);
    query.prepare(
      "SELECT * FROM email_delivery ed " + QString::fromStdString(sWhere) + " ORDER BY ed.dt DESC LIMIT " +
      QString::number(nPage * nOnPage) + "," + QString::number(nOnPage)
    );
    foreach (QString key, filter_values.keys()) {
      query.bindValue(key, filter_values.value(key));
    }
    if (!query.exec()) {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, query.lastError().text().toStdString()));
      return;
    }
    while (query.next()) {
      QSqlRecord record = query.record();
      nlohmann::json jsonEmail;

      jsonEmail["id"] = record.value("id").toInt();
      jsonEmail["email"] = record.value("to_email").toString().toStdString();
      jsonEmail["subject"] = record.value("subject").toString().toHtmlEscaped().toStdString();
      jsonEmail["message"] = record.value("message").toString().toHtmlEscaped().toStdString();
      jsonEmail["priority"] = record.value("priority").toString().toHtmlEscaped().toStdString();
      jsonEmail["status"] = record.value("status").toString().toHtmlEscaped().toStdString();
      jsonEmail["dt"] = record.value("dt").toString().toHtmlEscaped().toStdString();
      jsonEmails.push_back(jsonEmail);
    }
  }

  jsonResponse["data"] = jsonEmails;
  jsonResponse["onpage"] = nOnPage;
  jsonResponse["page"] = nPage;
  jsonResponse["count"] = nCount;
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
