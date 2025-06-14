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

#include <cmd_handlers.h>

class CmdHandlerQuests : public CmdHandlerBase {
public:
  CmdHandlerQuests();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerQuest : public CmdHandlerBase {
public:
  CmdHandlerQuest();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerQuestPass : public CmdHandlerBase {
public:
  CmdHandlerQuestPass();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerQuestsAdd : public CmdHandlerBase {
public:
  CmdHandlerQuestsAdd();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerQuestDelete : public CmdHandlerBase {
public:
  CmdHandlerQuestDelete();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerQuestProposal : public CmdHandlerBase {
public:
  CmdHandlerQuestProposal();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerQuestStatistics : public CmdHandlerBase {
public:
  CmdHandlerQuestStatistics();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerQuestUpdate : public CmdHandlerBase {
public:
  CmdHandlerQuestUpdate();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerQuestsSubjects : public CmdHandlerBase {
public:
  CmdHandlerQuestsSubjects();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerAddHint : public CmdHandlerBase {
public:
  CmdHandlerAddHint();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerAnswerList : public CmdHandlerBase {
public:
  CmdHandlerAnswerList();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerQuestsUserAnswerList : public CmdHandlerBase {
public:
  CmdHandlerQuestsUserAnswerList();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerDeleteHint : public CmdHandlerBase {
public:
  CmdHandlerDeleteHint();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerHints : public CmdHandlerBase {
public:
  CmdHandlerHints();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerQuestsProposalList : public CmdHandlerBase {
public:
  CmdHandlerQuestsProposalList();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerQuestsFilesUpload : public CmdHandlerBase {
public:
  CmdHandlerQuestsFilesUpload();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerQuestsFilesDelete : public CmdHandlerBase {
public:
  CmdHandlerQuestsFilesDelete();
  virtual void handle(ModelRequest *pRequest);
};
