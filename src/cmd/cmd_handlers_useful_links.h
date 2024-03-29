// Copyright (c) 2015-2020 FreeHackQuest <freehackquest@gmail.com>

#ifndef CMD_HANDLERS_USEFUL_LINKS_H
#define CMD_HANDLERS_USEFUL_LINKS_H

#include <cmd_handlers.h>

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksList : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksList();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksRetrieve : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksRetrieve();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksAdd : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksAdd();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksDelete : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksDelete();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksUpdate : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksUpdate();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksUserFavoriteList : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksUserFavoriteList();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksUserFavorite : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksUserFavorite();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksUserUnfavorite : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksUserUnfavorite();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksClicked : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksClicked();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksCommentList : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksCommentList();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksCommentAdd : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksCommentAdd();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksCommentDelete : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksCommentDelete();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksTagList : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksTagList();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksTagAdd : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksTagAdd();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksTagDelete : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksTagDelete();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksUserPropose : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksUserPropose();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdHandlerUsefulLinksUserProposeApprove : public CmdHandlerBase {
public:
  CmdHandlerUsefulLinksUserProposeApprove();
  virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HANDLERS_USEFUL_LINKS_H
