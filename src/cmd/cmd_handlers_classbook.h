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

#pragma once

#include <QFile> // TODO redesign to c++ file
#include <cmd_handlers.h>

class CmdClassbookAddRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookAddRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookDeleteRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookDeleteRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookExportHandler : public CmdHandlerBase {
public:
  CmdClassbookExportHandler();
  virtual void handle(ModelRequest *pRequest);

private:
  bool createHtml(QFile *file, const std::string &sLang, QSqlQuery query, ModelRequest *pRequest);
  bool createMD(QFile *file, const std::string &sLang, QSqlQuery query, ModelRequest *pRequest);
};

class CmdClassbookInfoHandler : public CmdHandlerBase {
public:
  CmdClassbookInfoHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookListHandler : public CmdHandlerBase {
public:
  CmdClassbookListHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookUpdateRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookUpdateRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookLocalizationAddRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookLocalizationAddRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookLocalizationDeleteRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookLocalizationDeleteRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookLocalizationInfoHandler : public CmdHandlerBase {
public:
  CmdClassbookLocalizationInfoHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookLocalizationUpdateRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookLocalizationUpdateRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookProposalAddRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalAddRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookProposalDeleteRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalDeleteRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookProposalInfoHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalInfoHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookProposalListHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalListHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookProposalPrepareMergeRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalPrepareMergeRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookProposalApproveHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalApproveHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookProposalUpdateHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalUpdateHandler();
  virtual void handle(ModelRequest *pRequest);
};

class CmdClassbookHandler : public CmdHandlerBase {
public:
  CmdClassbookHandler();
  virtual void handle(ModelRequest *pRequest);
};
