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

class CmdHandlerUsefulLinksList : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksList();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksRetrieve : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksRetrieve();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksAdd : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksAdd();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksDelete : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksDelete();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksUpdate : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksUpdate();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksUserFavoriteList : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksUserFavoriteList();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksUserFavorite : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksUserFavorite();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksUserUnfavorite : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksUserUnfavorite();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksClicked : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksClicked();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksCommentList : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksCommentList();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksCommentAdd : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksCommentAdd();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksCommentDelete : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksCommentDelete();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksTagList : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksTagList();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksTagAdd : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksTagAdd();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksTagDelete : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksTagDelete();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksUserPropose : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksUserPropose();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsefulLinksUserProposeApprove : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksUserProposeApprove();
  virtual void handle(ModelRequest *pRequest);
};
