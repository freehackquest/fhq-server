#ifndef CMD_HADNLERS_SERVER_H
#define CMD_HADNLERS_SERVER_H

#include <cmd_handlers.h>

// ---------------------------------------------------------------------

class CmdHandlerPublicInfo : public CmdHandlerBase {

public:
  CmdHandlerPublicInfo();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerServerInfo : public CmdHandlerBase {

public:
  CmdHandlerServerInfo();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerServerSettings : public CmdHandlerBase {

public:
  CmdHandlerServerSettings();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerServerSettingsUpdate : public CmdHandlerBase {

public:
  CmdHandlerServerSettingsUpdate();
  virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HADNLERS_SERVER_H
