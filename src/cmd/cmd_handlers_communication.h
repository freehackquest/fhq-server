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

#pragma once

#include <cmd_handlers.h>

class CmdHandlerChatSendMessage : public CmdHandlerBase {
public:
  CmdHandlerChatSendMessage();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatSendMessage_new : public CmdHandlerBase {
public:
  CmdHandlerChatSendMessage_new();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatLastestMessages : public CmdHandlerBase {
public:
  CmdHandlerChatLastestMessages();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatReadMessage : public CmdHandlerBase {
public:
  CmdHandlerChatReadMessage();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatShowDialogs : public CmdHandlerBase {
public:
  CmdHandlerChatShowDialogs();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatEditMessage : public CmdHandlerBase {
public:
  CmdHandlerChatEditMessage();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatDeleteMessage : public CmdHandlerBase {
public:
  CmdHandlerChatDeleteMessage();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatAddToChat : public CmdHandlerBase {
public:
  CmdHandlerChatAddToChat();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatDeleteFromChat : public CmdHandlerBase {
public:
  CmdHandlerChatDeleteFromChat();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatChangeOwner : public CmdHandlerBase {
public:
  CmdHandlerChatChangeOwner();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatAddToBlackList : public CmdHandlerBase {
public:
  CmdHandlerChatAddToBlackList();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatDeleteFromBlackList : public CmdHandlerBase {
public:
  CmdHandlerChatDeleteFromBlackList();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerChatCreateGroupChat : public CmdHandlerBase {
public:
  CmdHandlerChatCreateGroupChat();
  virtual void handle(ModelRequest *pRequest);
};
