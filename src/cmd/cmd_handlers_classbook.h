#ifndef CMD_HADNLERS_CLASSBOOK_H
#define CMD_HADNLERS_CLASSBOOK_H

#include <QFile> // TODO redesign to c++ file
#include <cmd_handlers.h>

// ---------------------------------------------------------------------

class CmdClassbookAddRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookAddRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookDeleteRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookDeleteRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookExportHandler : public CmdHandlerBase {
public:
  CmdClassbookExportHandler();
  virtual void handle(ModelRequest *pRequest);

private:
  bool createHtml(QFile *file, const std::string &sLang, QSqlQuery query, ModelRequest *pRequest);
  bool createMD(QFile *file, const std::string &sLang, QSqlQuery query, ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookInfoHandler : public CmdHandlerBase {
public:
  CmdClassbookInfoHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookListHandler : public CmdHandlerBase {
public:
  CmdClassbookListHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookUpdateRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookUpdateRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookLocalizationAddRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookLocalizationAddRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookLocalizationDeleteRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookLocalizationDeleteRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookLocalizationInfoHandler : public CmdHandlerBase {
public:
  CmdClassbookLocalizationInfoHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookLocalizationUpdateRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookLocalizationUpdateRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookProposalAddRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalAddRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookProposalDeleteRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalDeleteRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookProposalInfoHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalInfoHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookProposalListHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalListHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookProposalPrepareMergeRecordHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalPrepareMergeRecordHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookProposalApproveHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalApproveHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookProposalUpdateHandler : public CmdHandlerBase {
public:
  CmdClassbookProposalUpdateHandler();
  virtual void handle(ModelRequest *pRequest);
};

// ---------------------------------------------------------------------

class CmdClassbookHandler : public CmdHandlerBase {
public:
  CmdClassbookHandler();
  virtual void handle(ModelRequest *pRequest);
};

#endif // CMD_HADNLERS_CLASSBOOK_H
