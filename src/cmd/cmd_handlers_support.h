#ifndef CMD_HADNLERS_SUPPORT_H
#define CMD_HADNLERS_SUPPORT_H

#include <cmd_handlers.h>

// ---------------------------------------------------------------------

class CmdHandlerFeedbackAdd : public CmdHandlerBase {

public:
  CmdHandlerFeedbackAdd();
  virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HADNLERS_SUPPORT_H
