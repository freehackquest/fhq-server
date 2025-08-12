/**********************************************************************************
 *          Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * Copyright (c) 2011-2025 FreeHackQuest <freehackquest@gmail.com>
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
#include <cmd_handlers_events.h>
#include <employ_database.h>
#include <employ_public_events.h>
#include <employ_server_info.h>
#include <iostream>
#include <runtasks.h>
#include <validators.h>

// ---------------------------------------------------------------------
// This handler will be create public event

REGISTRY_CMD(CmdHandlerEventAdd)

CmdHandlerEventAdd::CmdHandlerEventAdd() : CmdHandlerBase("createpublicevent", "Create the public event") {

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  // validation and description input fields
  requireStringParam("type", "Type of event").addValidator(new ValidatorEventType());
  requireStringParam("message", "Message");
}

void CmdHandlerEventAdd::handle(ModelRequest *pRequest) {
  auto *pEvents = findWsjcppEmploy<EmployPublicEvents>();

  ModelPublicEvent eventInfo;
  eventInfo.setType(pRequest->getInputString("type", ""));
  eventInfo.setMessage(pRequest->getInputString("message", ""));
  eventInfo.setMeta("{}");

  std::string sErrorMessage;
  if (!pEvents->addPublicEvent(eventInfo, sErrorMessage)) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, sErrorMessage));
    return;
  }

  // TODO return full info
  nlohmann::json jsonResponse;
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// This handler will be delete public event

REGISTRY_CMD(CmdHandlerEventDelete)

CmdHandlerEventDelete::CmdHandlerEventDelete() : CmdHandlerBase("deletepublicevent", "Delete public event") {

  setAccessUnauthorized(false);
  setAccessUser(false);
  setAccessAdmin(true);

  requireIntegerParam("eventid", "Event ID");
}

void CmdHandlerEventDelete::handle(ModelRequest *pRequest) {
  auto *pEvents = findWsjcppEmploy<EmployPublicEvents>();

  int nEventId = pRequest->getInputInteger("eventid", -1);
  std::string sErrorMessage;
  if (!pEvents->removePublicEvent(nEventId, sErrorMessage)) {
    if (sErrorMessage == "NOT_FOUND") {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(404, "Event not found"));
      return;
    }
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, sErrorMessage));
    return;
  }

  nlohmann::json jsonResponse;
  jsonResponse["eventid"] = nEventId;
  nlohmann::json jsonEvent;
  jsonResponse["data"] = jsonEvent;
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// This handler will be retrun info about public event

REGISTRY_CMD(CmdHandlerEventInfo)

CmdHandlerEventInfo::CmdHandlerEventInfo() : CmdHandlerBase("getpublicevent", "Return public event info by id") {

  setAccessUnauthorized(true);
  setAccessUser(true);
  setAccessAdmin(true);

  // validation and description input fields
  requireIntegerParam("eventid", "Event id");
}

void CmdHandlerEventInfo::handle(ModelRequest *pRequest) {
  auto *pEvents = findWsjcppEmploy<EmployPublicEvents>();

  nlohmann::json jsonResponse;
  int nEventId = pRequest->getInputInteger("eventid", -1);
  jsonResponse["eventid"] = nEventId;

  ModelPublicEvent eventInfo;
  std::string sErrorMessage = "";
  if (!pEvents->findPublicEvent(nEventId, eventInfo, sErrorMessage)) {
    if (sErrorMessage == "NOT_FOUND") {
      pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(404, "Event not found"));
      return;
    }
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, sErrorMessage));
    return;
  }
  nlohmann::json jsonEvent;

  jsonEvent["dt"] = eventInfo.getDateTime();
  jsonEvent["id"] = eventInfo.getLocalId();
  jsonEvent["type"] = eventInfo.getType();       // TODO htmlspecialchars
  jsonEvent["message"] = eventInfo.getMessage(); // TODO htmlspecialchars
  jsonEvent["meta"] = eventInfo.getMeta();

  jsonResponse["data"] = jsonEvent;
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}

// ---------------------------------------------------------------------
// This handler will be return list of public events

REGISTRY_CMD(CmdHandlerEventsList)

CmdHandlerEventsList::CmdHandlerEventsList() : CmdHandlerBase("publiceventslist", "Return list of public events") {

  setAccessUnauthorized(true);
  setAccessUser(true);
  setAccessAdmin(true);

  requireIntegerParam("page", "Number of page");              // TODO validator
  requireIntegerParam("onpage", "How much rows in one page"); // TODO validator
  optionalStringParam("type", "Filter by type events");       // TODO validator
  optionalStringParam("search", "Filter by message event");   // TODO validator
}

// ---------------------------------------------------------------------

void CmdHandlerEventsList::handle(ModelRequest *pRequest) {
  auto *pEvents = findWsjcppEmploy<EmployPublicEvents>();

  nlohmann::json jsonResponse;
  int nPage = pRequest->getInputInteger("page", 0);
  jsonResponse["page"] = nPage;

  int nOnPage = pRequest->getInputInteger("onpage", 10);
  jsonResponse["onpage"] = nOnPage;

  std::string sType = pRequest->getInputString("type", "");
  std::string sSearch = pRequest->getInputString("search", "");

  int nRecordsFound = 0;

  std::vector<ModelPublicEvent> eventList;
  std::string sErrorMessage;
  if (!pEvents->findPublicEvents(eventList, nPage, nOnPage, sType, sSearch, nRecordsFound, sErrorMessage)) {
    pRequest->sendMessageError(cmd(), WsjcppJsonRpc20Error(500, sErrorMessage));
    return;
  }
  jsonResponse["count"] = nRecordsFound;

  // data
  nlohmann::json jsonPublicEventsList = nlohmann::json::array();

  for (int i = 0; i < eventList.size(); i++) {
    nlohmann::json jsonEvent;
    jsonEvent["dt"] = eventList[i].getDateTime();
    jsonEvent["id"] = eventList[i].getLocalId();
    jsonEvent["type"] = eventList[i].getType();       // TODO htmlspecialchars
    jsonEvent["message"] = eventList[i].getMessage(); // TODO htmlspecialchars
    std::string sMeta = eventList[i].getMeta();
    if (nlohmann::json::accept(sMeta)) {
      jsonEvent["meta"] = nlohmann::json::parse(sMeta);
    } else {
      jsonEvent["meta"] = "";
    }
    jsonPublicEventsList.push_back(jsonEvent);
  }

  jsonResponse["data"] = jsonPublicEventsList;
  pRequest->sendMessageSuccess(cmd(), jsonResponse);
}
