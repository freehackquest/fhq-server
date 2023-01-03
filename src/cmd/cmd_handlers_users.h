// Copyright (c) 2015-2020 FreeHackQuest <freehackquest@gmail.com>

#pragma once

#include <cmd_handlers.h>

// ---------------------------------------------------------------------

class CmdHandlerUsersScoreboard : public CmdHandlerBase {
public:
  CmdHandlerUsersScoreboard();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerGetMap : public CmdHandlerBase {
public:
  CmdHandlerGetMap();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerLogin : public CmdHandlerBase {
public:
  CmdHandlerLogin();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerRegistration : public CmdHandlerBase {
public:
  CmdHandlerRegistration();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------
// User login by token

class CmdHandlerToken : public CmdHandlerBase {
public:
  CmdHandlerToken();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUpdateUserLocation : public CmdHandlerBase {
public:
  CmdHandlerUpdateUserLocation();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUserChangePassword : public CmdHandlerBase {
public:
  CmdHandlerUserChangePassword();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsersAdd : public CmdHandlerBase {
public:
  CmdHandlerUsersAdd();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUser : public CmdHandlerBase {
public:
  CmdHandlerUser();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsersInfo : public CmdHandlerBase {
public:
  CmdHandlerUsersInfo();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUserResetPassword : public CmdHandlerBase {
public:
  CmdHandlerUserResetPassword();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUserSkills : public CmdHandlerBase {
public:
  CmdHandlerUserSkills();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUserUpdate : public CmdHandlerBase {
public:
  CmdHandlerUserUpdate();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsersDelete : public CmdHandlerBase {
public:
  CmdHandlerUsersDelete();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsers : public CmdHandlerBase {
public:
  CmdHandlerUsers();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsersRegistration : public CmdHandlerBase {
public:
  CmdHandlerUsersRegistration();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsersRegistrationVerification : public CmdHandlerBase {
public:
  CmdHandlerUsersRegistrationVerification();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsersChangeEmail : public CmdHandlerBase {
public:
  CmdHandlerUsersChangeEmail();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsersChangeEmailVerification : public CmdHandlerBase {
public:
  CmdHandlerUsersChangeEmailVerification();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsersTokens : public CmdHandlerBase {
public:
  CmdHandlerUsersTokens();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsersTokensDelete : public CmdHandlerBase {
public:
  CmdHandlerUsersTokensDelete();
  virtual void handle(ModelRequest *pRequest);
};
