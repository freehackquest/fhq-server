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

class CmdHandlerUsersScoreboard : public CmdHandlerBase {
public:
  CmdHandlerUsersScoreboard();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerGetMap : public CmdHandlerBase {
public:
  CmdHandlerGetMap();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerLogin : public CmdHandlerBase {
public:
  CmdHandlerLogin();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerRegistration : public CmdHandlerBase {
public:
  CmdHandlerRegistration();
  virtual void handle(ModelRequest *pRequest);
};

// User login by token
class CmdHandlerToken : public CmdHandlerBase {
  public:
    CmdHandlerToken();
    virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUpdateUserLocation : public CmdHandlerBase {
public:
  CmdHandlerUpdateUserLocation();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUserChangePassword : public CmdHandlerBase {
public:
  CmdHandlerUserChangePassword();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsersAdd : public CmdHandlerBase {
public:
  CmdHandlerUsersAdd();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUser : public CmdHandlerBase {
public:
  CmdHandlerUser();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsersInfo : public CmdHandlerBase {
public:
  CmdHandlerUsersInfo();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUserResetPassword : public CmdHandlerBase {
public:
  CmdHandlerUserResetPassword();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUserSkills : public CmdHandlerBase {
public:
  CmdHandlerUserSkills();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUserUpdate : public CmdHandlerBase {
public:
  CmdHandlerUserUpdate();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsersDelete : public CmdHandlerBase {
public:
  CmdHandlerUsersDelete();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsers : public CmdHandlerBase {
public:
  CmdHandlerUsers();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsersRegistration : public CmdHandlerBase {
public:
  CmdHandlerUsersRegistration();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsersRegistrationVerification : public CmdHandlerBase {
public:
  CmdHandlerUsersRegistrationVerification();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsersChangeEmail : public CmdHandlerBase {
public:
  CmdHandlerUsersChangeEmail();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsersChangeEmailVerification : public CmdHandlerBase {
public:
  CmdHandlerUsersChangeEmailVerification();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsersTokens : public CmdHandlerBase {
public:
  CmdHandlerUsersTokens();
  virtual void handle(ModelRequest *pRequest);
};

class CmdHandlerUsersTokensDelete : public CmdHandlerBase {
public:
  CmdHandlerUsersTokensDelete();
  virtual void handle(ModelRequest *pRequest);
};
