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

#include <QSqlQuery>  // TODO deprecated
#include <QSqlRecord> // TODO deprecated
#include <QString>    // TODO deprecated
#include <QVariant>   // TODO deprecated
#include <QWebSocket>
#include <json.hpp>
#include <map>
#include <wsjcpp_validators.h>

/*!
 * WsjcppJsonRpc20Error -
 * */

class WsjcppJsonRpc20Error {
  public:
    // WsjcppJsonRpc20Error(int nErrorCode, const std::string &sErrorMessage);
    WsjcppJsonRpc20Error(int nErrorCode, const std::string &sErrorMessage,
                        const std::vector<std::pair<std::string, std::string>> &vErrorContext = {});
    int getErrorCode() const;
    std::string getErrorMessage() const;
    const std::vector<std::pair<std::string, std::string>> &getErrorContext() const;
    nlohmann::json toJson();

  private:
    std::string m_sErrorMessage;
    int m_nErrorCode;
    std::vector<std::pair<std::string, std::string>> m_vErrorContext;
};

/*!
 * WsjcppUserSession -
 * */

class WsjcppUserSession {
public:
  WsjcppUserSession();
  WsjcppUserSession(nlohmann::json const &obj);
  void fillFrom(nlohmann::json const &obj);

  // IUserToken
  bool isAdmin();
  bool isUser();
  bool isTester();
  bool hasRole();
  QString nick();
  void setNick(QString);
  QString email();
  int userid();
  std::string userUuid();
  // TODO json field for customization
private:
  std::string m_sRole;
  std::string m_sEmail;
  std::string m_sNick;
  int m_nUserID;
  std::string m_sUserUuid;
  std::string TAG;
};

/*!
 * WsjcppSocketClient -
 * */

class WsjcppSocketClient : public QObject {
private:
  Q_OBJECT

public:
  WsjcppSocketClient(QWebSocket *pSocket);
  ~WsjcppSocketClient();

private:
  std::string TAG;
  WsjcppUserSession *m_pUserSession;
  QWebSocket *m_pSocket;

private Q_SLOTS:
  void processTextMessage(const QString &message);
  void processBinaryMessage(QByteArray message);
  void socketDisconnected();
};

/*!
 * IWebSocketServer -
 * */

class IWebSocketServer {
public:
  virtual void sendMessage(QWebSocket *pClient, const nlohmann::json &jsonResponse) = 0;
  virtual void sendMessageError(QWebSocket *pClient, const std::string &sCmd, const std::string &sM,
                                WsjcppJsonRpc20Error error) = 0;
  virtual void sendToAll(const nlohmann::json &jsonMessage) = 0;
  virtual void sendToOne(QWebSocket *pClient, const nlohmann::json &jsonMessage) = 0;
  virtual int getConnectedUsers() = 0;
  virtual void setWsjcppUserSession(QWebSocket *pClient, WsjcppUserSession *pUserSession) = 0;
  virtual WsjcppUserSession *getWsjcppUserSession(QWebSocket *pClient) = 0;
};

/*!
 * CmdInputDef - helper api for define input params and descrip it for docs.
 * */

class CmdInputDef {
public:
  CmdInputDef(const std::string &sName, const std::string &sDescription);
  CmdInputDef();
  CmdInputDef &optional();
  CmdInputDef &required();
  CmdInputDef &string_();
  CmdInputDef &integer_();
  CmdInputDef &any_();
  CmdInputDef &bool_();
  CmdInputDef &description(const std::string &sDescription);
  CmdInputDef &minval(int minval);
  CmdInputDef &maxval(int maxval);
  nlohmann::json toJson();

  const std::string &getType();
  const std::string &getType() const;
  const std::string &getName();
  const std::string &getName() const;
  const std::string &getRestrict();
  const std::string &getRestrict() const;
  const std::string &getDescription();
  const std::string &getDescription() const;

  bool isRequired();
  bool isInteger();
  bool isString();
  bool isBool();
  bool isAny();

  bool isMinVal(); // TODO: redesign to validators
  int getMinVal(); // TODO: redesign to validators
  bool isMaxVal(); // TODO: redesign to validators
  int getMaxVal(); // TODO: redesign to validators

  CmdInputDef &addValidator(WsjcppValidatorStringBase *pValidatorStringBase);

  const std::vector<WsjcppValidatorStringBase *> &listOfValidators();

private:
  std::string m_sType;
  std::string m_sName;
  std::string m_sRestrict;
  std::string m_sDescription;
  int m_nMinVal;
  bool m_bSettedMinVal;
  int m_nMaxVal;
  bool m_bSettedMaxVal;

  std::string CMD_INPUT_DEF_TYPE_INTEGER = "integer";
  std::string CMD_INPUT_DEF_TYPE_STRING = "string";
  std::string CMD_INPUT_DEF_TYPE_BOOL = "boolean";
  std::string CMD_INPUT_DEF_TYPE_ANY = "any";

  std::vector<WsjcppValidatorStringBase *> m_vValidatorsString;
};

class ModelRequest {
public:
  ModelRequest(QWebSocket *pClient, IWebSocketServer *pWebSocketServer, nlohmann::json &jsonRequest_);
  QWebSocket *client();
  std::string getIpAddress();
  IWebSocketServer *server();
  WsjcppUserSession *getUserSession();
  bool isAdmin();
  bool isUser();
  bool isUnauthorized();
  // TODO set input defs

  const nlohmann::json &jsonRequest(); // TODO deprecated
  bool hasInputParam(const std::string &sParamName);
  std::string getInputString(const std::string &sParamName, const std::string &sDefaultValue);
  int getInputInteger(const std::string &sParamName, int defaultValue);

  std::string m();
  bool hasM();
  std::string command();
  bool hasCommand();
  void sendMessageError(const std::string &cmd, WsjcppJsonRpc20Error error);
  void sendMessageSuccess(const std::string &cmd, nlohmann::json &jsonResponse);
  void sendResponse(nlohmann::json &jsonResult);

  // bool validateInputParameters(Error &error, CmdHandlerBase *pCmdHandler);
private:
  std::string TAG;
  QWebSocket *m_pClient;
  IWebSocketServer *m_pServer;
  WsjcppUserSession *m_pWsjcppUserSession;
  nlohmann::json m_jsonRequest;
  std::string m_sMessageId;
  std::string m_sCommand;
};

/*!
 * Api handler Base
 * */

class CmdHandlerBase { // TODO rename to WJSCppHandler

public:
  CmdHandlerBase(const std::string &sCmd, const std::string &sDescription);
  virtual std::string cmd();
  virtual std::string description();
  std::string activatedFromVersion();
  std::string deprecatedFromVersion();
  bool accessUnauthorized();
  bool accessUser();
  bool accessAdmin();
  bool checkAccess(ModelRequest *pRequest);

  virtual const std::vector<CmdInputDef> &inputs();
  virtual void handle(ModelRequest *pRequest) = 0;

  // virtual void success(nlohmann::json jsonResponse);
  // virtual void error(int nCode, const std::string &sErrorMessage);

protected:
  void setAccessUnauthorized(bool bAccess);
  void setAccessUser(bool bAccess);
  void setAccessAdmin(bool bAccess);
  void setActivatedFromVersion(const std::string &sActivatedFromVersion);
  void setDeprecatedFromVersion(const std::string &sDeprecatedFromVersion);

  CmdInputDef &requireStringParam(const std::string &sName, const std::string &sDescription);
  CmdInputDef &optionalStringParam(const std::string &sName, const std::string &sDescription);
  CmdInputDef &requireIntegerParam(const std::string &sName, const std::string &sDescription);
  CmdInputDef &optionalIntegerParam(const std::string &sName, const std::string &sDescription);
  CmdInputDef &requireBooleanParam(const std::string &sName, const std::string &sDescription);
  CmdInputDef &optionalBooleanParam(const std::string &sName, const std::string &sDescription);

  std::string TAG;
  std::string m_sCmd;
  std::string m_sDescription;

private:
  std::vector<CmdInputDef> m_vInputs; // TODO redesign to map
  // std::map<std::string, CmdInputDef*> *m_vCmdInputDefs;
  std::string m_sActivatedFromVersion;
  std::string m_sDeprecatedFromVersion;
  bool m_bAccessUnauthorized;
  bool m_bAccessUser;
  bool m_bAccessAdmin;
};

extern std::map<std::string, CmdHandlerBase *> *g_pCmdHandlers;

/*!
 * Global collection with handlers
 * */

class CmdHandlers {
public:
  static void initGlobalVariables();
  static void addHandler(const std::string &sName, CmdHandlerBase *pCmdHandler);
  static CmdHandlerBase *findCmdHandler(const std::string &sCmd);
};

// RegistryCmdHandler
#define REGISTRY_CMD(classname) static classname *pRegistry##classname = new classname();

// Registry Wsjcpp JsonRpc20 Handler
#define REGISTRY_WSJCPP_JSONRPC20_HANDLER(classname)                                                                   \
  static classname *pRegistryWsjcppJsonRpc20##classname = new classname();

/*!
 * This handler will be return list of handlers - publish api interfaces
 * */

class WJSCppCmdHandlerServerApi : public CmdHandlerBase {

public:
  WJSCppCmdHandlerServerApi();
  virtual void handle(ModelRequest *pRequest);
};
