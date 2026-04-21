/**********************************************************************************
 *                      Project
 *    __ _
 *   / _| |__   __ _       ___  ___ _ ____   _____ _ __
 *  | |_| '_ \ / _` | ___ / __|/ _ \ '__\ \ / / _ \ '__|
 *  |  _| | | | (_| ||___|\__ \  __/ |   \ V /  __/ |
 *  |_| |_| |_|\__, |     |___/\___|_|    \_/ \___|_|
 *                |_|
 *
 * MIT License
 *
 * Copyright (c) 2011-2026 FreeHackQuest <freehackquest@gmail.com>
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
 * Original repository: https://github.com/freehackquest/fhq-server
 *
 ***********************************************************************************/

#include "employ_web_server.h"
// #include <QSqlDatabase>
// #include <QSqlQuery>
// #include <QSqlRecord>
// #include <QFile>
// #include <QString>
// #include <QtCore>
#include <employ_database.h>
#include <employ_files.h>
#include <employ_notify.h>
#include <sys/stat.h>
#include <websocketserver.h>
#include <wsjcpp_hashes.h>
#include <wsjcpp_light_web_server.h>

#include "EventLoop.h"       // libhv
#include "HttpService.h"     // libhv
#include "WebSocketServer.h" // libhv
#include "hlog.h"            // libhv
#include "hssl.h"            // libhv
#include "htime.h"           // libhv

// ---------------------------------------------------------------------
// HttpHandlerWebUserFolder

class HttpHandlerWebUserFolder : public WsjcppLightWebHttpHandlerBase {
public:
  HttpHandlerWebUserFolder(const std::string &sWebFolder);
  virtual bool canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);
  virtual bool handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);

private:
  std::string TAG;
  std::string m_sWebFolder;
};

HttpHandlerWebUserFolder::HttpHandlerWebUserFolder(const std::string &sWebFolder)
  : WsjcppLightWebHttpHandlerBase("web-user-folder") {

  TAG = "HttpHandlerWebUserFolder";
  m_sWebFolder = sWebFolder;
}

bool HttpHandlerWebUserFolder::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
  std::string _tag = TAG + "-" + sWorkerId;
  // WsjcppLog::warn(_tag, "canHandle: " + pRequest->requestPath());
  std::string sRequestPath = pRequest->getRequestPath();

  if (sRequestPath == "") {
    sRequestPath = "/";
    WsjcppLog::warn(_tag, "Request path is empty");
  }

  if (sRequestPath == "/") {
    sRequestPath = "/index.html";
  }

  if (!WsjcppCore::dirExists(m_sWebFolder)) {
    WsjcppLog::warn(_tag, "Directory " + m_sWebFolder + " does not exists");
  }
  return true;
}

bool HttpHandlerWebUserFolder::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
  std::string _tag = TAG + "-" + sWorkerId;
  std::string sRequestPath = pRequest->getRequestPath();
  // WsjcppLog::warn(_tag, pRequest->requestPath());

  if (sRequestPath == "") {
    sRequestPath = "/";
  }

  std::string sFilePath = m_sWebFolder + sRequestPath; // TODO check /../ in path
  if (WsjcppCore::fileExists(sFilePath)) {
    WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
    resp.cacheSec(60).ok().sendFile(sFilePath);
  } else {
    std::string sFilePath = m_sWebFolder + "/index.html";
    WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
    resp.cacheSec(60).ok().sendFile(sFilePath);
  }
  return true;
}

// ---------------------------------------------------------------------
// HttpHandlerWebPublicFolder

class HttpHandlerWebPublicFolder : public WsjcppLightWebHttpHandlerBase {
public:
  HttpHandlerWebPublicFolder(const std::string &sWebFolder, const std::string &sFileStorage);
  virtual bool canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);
  virtual bool handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);

private:
  std::string TAG;
  std::string m_sWebPublicFolder;
  std::string m_sFileStorage;
};

HttpHandlerWebPublicFolder::HttpHandlerWebPublicFolder(
  const std::string &sWebPublicFolder, const std::string &sFileStorage
)
  : WsjcppLightWebHttpHandlerBase("web-public-folder") {

  TAG = "HttpHandlerWebPublicFolder";
  m_sWebPublicFolder = sWebPublicFolder;
  m_sFileStorage = sFileStorage;
}

bool HttpHandlerWebPublicFolder::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
  std::string _tag = TAG + "-" + sWorkerId;
  // WsjcppLog::warn(_tag, "canHandle: " + pRequest->requestPath());
  std::string sRequestPath = pRequest->getRequestPath();

  if (!WsjcppCore::dirExists(m_sWebPublicFolder)) {
    WsjcppLog::warn(_tag, "Directory '" + m_sWebPublicFolder + "' does not exists");
  }
  if (sRequestPath.rfind("/public/", 0) == 0) {
    return true;
  }
  return false;
}

bool HttpHandlerWebPublicFolder::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
  std::string _tag = TAG + "-" + sWorkerId;
  WsjcppLightWebHttpResponse resp(pRequest->getSockFd());

  std::string sRequestPath = pRequest->getRequestPath();
  sRequestPath = sRequestPath.substr(8); // remove '/public/'
  sRequestPath = WsjcppCore::doNormalizePath("/" + sRequestPath);

  // TODO redsign this hardcode
  std::string sFilename = "";
  // if (sRequestPath.rfind("/quests/", 0) == 0) {
  auto pEmployFiles = findWsjcppEmploy<EmployFiles>();
  ModelQuestFile model;
  if (pEmployFiles->findQuestFileByFilePath(sRequestPath, model)) {
    // std::string sMessageError = "This file not registered in the system '" +
    // sRequestPath + "'"; WsjcppLog::err(TAG, sMessageError);
    // resp.cacheSec(0).notFound().sendText("<h1>" + sMessageError + "</h1>");
    // return true;
    sFilename = model.getFileName();
    pEmployFiles->updateDownloadsCounter(model);
  }

  std::string sFilePath = "";

  if (sRequestPath.rfind("/games/", 0) == 0) { // hardcode
    // look to new folder file_storage

    // look in /public/games/18.png example
    sFilePath = m_sWebPublicFolder + sRequestPath;
    if (WsjcppCore::fileExists(sFilePath)) {
      resp.cacheSec(0).ok().sendFile(sFilePath, sFilename);
      return true;
    }

    // look in filestorage /file_storage/games/%uuid%/game.png example
    sFilePath = m_sFileStorage + sRequestPath;
    if (WsjcppCore::fileExists(sFilePath)) {
      resp.cacheSec(0).ok().sendFile(sFilePath, sFilename);
      return true;
    }

    std::string sMessageError = "File not found '" + sRequestPath + "'";
    WsjcppLog::err(TAG, sMessageError);
    resp.cacheSec(0).notFound().sendText("<h1>" + sMessageError + "</h1>");
    return true;
  } else {
    sFilePath = m_sWebPublicFolder + sRequestPath;
    if (!WsjcppCore::fileExists(sFilePath)) {
      std::string sMessageError = "File not found '" + sRequestPath + "'";
      WsjcppLog::err(TAG, sMessageError);
      resp.cacheSec(0).notFound().sendText("<h1>" + sMessageError + "</h1>");
      return true;
    }
  }

  resp.cacheSec(0).ok().sendFile(sFilePath, sFilename);
  return true;
}

// ---------------------------------------------------------------------
// HttpHandlerWebAdminFolder

class FhqHttpServer {
public:
  FhqHttpServer();
  hv::HttpService *getService();
  void setWebAdminFolder(const std::string &sWebAdminFolder);
  void setWebPublicFolder(const std::string &sWebPublicFolder);
  void setWebUserFolder(const std::string &sWebUserFolder);
  int httpApiV1GetPaths(HttpRequest *req, HttpResponse *resp);
  int handleGetRequest(HttpRequest *req, HttpResponse *resp);
  int httpApiV1MyIp(HttpRequest *req, HttpResponse *resp);

private:
  std::string TAG;
  hv::HttpService *m_pHttpService;
  std::string m_sWebAdminFolder;
  std::string m_sWebUserFolder;
  std::string m_sWebPublicFolder;
  std::string m_sApiPathPrefix;
  EmployDatabase *m_pEmployDatabase;
};

FhqHttpServer::FhqHttpServer() {
  TAG = "FhqHttpServer";
  m_pEmployDatabase = findWsjcppEmploy<EmployDatabase>();

  // {
  //     logger_t* pLogger = hv_default_logger();
  //     // logger_set_max_filesize(pLogger, 102400);
  //     std::string sLogDirPath = m_pConfig->getWorkDir() + "/hv_logs";
  //     if (!WsjcppCore::dirExists(sLogDirPath)) {
  //         WsjcppCore::makeDir(sLogDirPath);
  //     }
  //     std::string sLogFilePath = sLogDirPath + "/http_" + WsjcppCore::getCurrentTimeForFilename() + ".log";
  //     logger_set_file(pLogger, sLogFilePath.c_str());
  // }

  m_sApiPathPrefix = "/api/v1/";
  m_pHttpService = new HttpService();

  // static files
  // m_pHttpService->document_root = "./html";

  m_pHttpService->GET(
    "*", std::bind(&FhqHttpServer::handleGetRequest, this, std::placeholders::_1, std::placeholders::_2)
  );
}

hv::HttpService *FhqHttpServer::getService() { return m_pHttpService; }

void FhqHttpServer::setWebAdminFolder(const std::string &sWebAdminFolder) {
  m_sWebAdminFolder = sWebAdminFolder;
  WsjcppLog::info(TAG, "m_sWebAdminFolder = " + m_sWebAdminFolder);
  m_sWebAdminFolder = WsjcppCore::doNormalizePath(m_sWebAdminFolder);
}

void FhqHttpServer::setWebPublicFolder(const std::string &sWebPublicFolder) {
  m_sWebPublicFolder = sWebPublicFolder;
  WsjcppLog::info(TAG, "m_sWebPublicFolder" + m_sWebPublicFolder);
  m_sWebPublicFolder = WsjcppCore::doNormalizePath(m_sWebPublicFolder);
}

void FhqHttpServer::setWebUserFolder(const std::string &sWebUserFolder) {
  m_sWebUserFolder = sWebUserFolder;
  WsjcppLog::info(TAG, "m_sWebUserFolder" + m_sWebUserFolder);
  m_sWebUserFolder = WsjcppCore::doNormalizePath(m_sWebUserFolder);
}

int FhqHttpServer::httpApiV1GetPaths(HttpRequest *req, HttpResponse *resp) {
  return resp->Json(m_pHttpService->Paths());
}

int FhqHttpServer::handleGetRequest(HttpRequest *req, HttpResponse *resp) {
  std::string sOriginalRequestPath = req->path;
  WsjcppLog::info(TAG, "Request path: " + req->path);
  std::string sRequestPath;
  std::string sGetParams;

  // remove get params from path
  std::size_t nFoundGetParams = sOriginalRequestPath.rfind("?");
  if (nFoundGetParams != std::string::npos) {
    sRequestPath = sOriginalRequestPath.substr(0, nFoundGetParams);
    sGetParams = sOriginalRequestPath.substr(nFoundGetParams);
  } else {
    sRequestPath = sOriginalRequestPath;
  }
  sRequestPath = WsjcppCore::doNormalizePath(sRequestPath);

  if (sRequestPath == "/admin") {
    return resp->Redirect("/admin/" + sGetParams);
  }

  if (m_sWebAdminFolder != "" && sRequestPath == "/admin/") {
    std::string sFilePath = WsjcppCore::doNormalizePath(m_sWebAdminFolder + "/index.html");
    if (!WsjcppCore::fileExists(sFilePath)) {
      return 404; // Not found
    }
    return resp->File(sFilePath.c_str());
  }

  if (m_sWebAdminFolder != "" && sRequestPath.rfind("/admin/", 0) == 0) {
    std::string sAdminSubpathFile = sRequestPath.substr(strlen("/admin/"));
    std::string sFilePath = WsjcppCore::doNormalizePath(m_sWebAdminFolder + "/" + sAdminSubpathFile);
    if (sFilePath.rfind(m_sWebAdminFolder, 0) != 0) {
      return 403;
    }
    // TODO from resources
    if (!WsjcppCore::fileExists(sFilePath)) {
      return 404; // Not found
    }
    return resp->File(sFilePath.c_str());
  }

  // public folder
  if (m_sWebPublicFolder != "" && sRequestPath.rfind("/public/", 0) == 0) {
    std::string sPublicSubpathFile = sRequestPath.substr(strlen("/public"));
    std::string sFilePath = WsjcppCore::doNormalizePath(m_sWebPublicFolder + "/" + sPublicSubpathFile);
    if (sFilePath.rfind(m_sWebPublicFolder, 0) != 0) {
      return 403;
    }

    // special counters for quests files
    if (sRequestPath.rfind("/public/quests/", 0) == 0) {
      ModelQuestFile model;
      auto pEmployFiles = findWsjcppEmploy<EmployFiles>();
      if (!pEmployFiles->findQuestFileByFilePath(sPublicSubpathFile, model)) {
        return 404;
      }
      // std::string sMessageError = "This file not registered in the system '" +
      // sPublicSubpathFile + "'"; WsjcppLog::err(TAG, sMessageError);
      // resp.cacheSec(0).notFound().sendText("<h1>" + sMessageError + "</h1>");
      // return true;
      if (!WsjcppCore::fileExists(sFilePath)) {
        return 404; // Not found
      }
      std::string sFileName = model.getFileName();
      pEmployFiles->updateDownloadsCounter(model);
      int res = resp->File(sFilePath.c_str());
      resp->SetContentTypeByFilename(sFileName.c_str());
      resp->SetHeader("Content-Disposition", "attachment; filename=\"" + sFileName + "\"");
      return res;
    }

    if (!WsjcppCore::fileExists(sFilePath)) {
      return 404; // Not found
    }
    // TODO make a cache for default user icon
    return resp->File(sFilePath.c_str());
  }

  // single app
  if (m_sWebUserFolder != "") {
    std::string sIndexPath = WsjcppCore::doNormalizePath(m_sWebUserFolder + "/index.html");
    if (sRequestPath == "/") {
      sRequestPath = "/index.html";
    }
    std::string sFilePath = WsjcppCore::doNormalizePath(m_sWebUserFolder + "/" + sRequestPath);
    if (sFilePath.rfind(m_sWebUserFolder, 0) != 0) {
      return 403;
    }
    // Try TODO from resources
    if (!WsjcppCore::fileExists(sFilePath)) {
      return resp->File(sIndexPath.c_str());
    }
    return resp->File(sFilePath.c_str());
  }

  // WsjcppLog::info(TAG, "sRequestPath = " + sRequestPath);

  // if (sRequestPath.rfind(m_sTeamLogoPrefix, 0) == 0) {
  //     std::string sTeamId = sRequestPath.substr(m_nTeamLogoPrefixLength, sRequestPath.length() -
  //     m_nTeamLogoPrefixLength); Ctf01dTeamLogo *pLogo = m_pTeamLogos->findTeamLogo(sTeamId); if (pLogo == nullptr) {
  //         return 404;
  //     }
  //     resp->Data(
  //         pLogo->pBuffer,
  //         pLogo->nBufferSize,
  //         true // nocopy
  //     );
  //     resp->SetContentTypeByFilename(pLogo->sFilename.c_str());
  //     return 200;
  // }

  if (sRequestPath.rfind(m_sApiPathPrefix, 0) == 0) {
    if (sRequestPath == "/api/v1/myip") {
      return this->httpApiV1MyIp(req, resp);
    }
    return this->httpApiV1GetPaths(req, resp);
  }

  if (sRequestPath == "/") {
    sRequestPath = "/index.html";
  }
  return 404; // Not found
}

// int FhqHttpServer::httpApiV1Scoreboard(HttpRequest* req, HttpResponse* resp) {
//     m_pTeamLogos->updateLastWriteTime();
//     nlohmann::json jsonScoreboard = m_pConfig->scoreboard()->toJson();
//     m_pTeamLogos->updateScorebordJson(jsonScoreboard);
//     std::string sScoreboardJson = jsonScoreboard.dump();
//     resp->Data(
//         (void *)(sScoreboardJson.c_str()),
//         sScoreboardJson.length(),
//         false // nocopy - force copy
//     );
//     resp->SetContentTypeByFilename("scoreboard.json");
//     return 200;
// }

int FhqHttpServer::httpApiV1MyIp(HttpRequest *req, HttpResponse *resp) {
  resp->json["myip"] = req->client_addr.ip;
  return 200;
}

// ---------------------------------------------------------------------
// HttpHandlerWebAdminFolder

class HttpHandlerWebAdminFolder : public WsjcppLightWebHttpHandlerBase {
public:
  HttpHandlerWebAdminFolder(const std::string &sWebFolder);
  virtual bool canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);
  virtual bool handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);

private:
  std::string TAG;
  std::string m_sWebFolder;
};

HttpHandlerWebAdminFolder::HttpHandlerWebAdminFolder(const std::string &sWebFolder)
  : WsjcppLightWebHttpHandlerBase("web-admin-folder") {

  TAG = "HttpHandlerWebAdminFolder";
  m_sWebFolder = sWebFolder;
}

bool HttpHandlerWebAdminFolder::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
  std::string _tag = TAG + "-" + sWorkerId;
  // WsjcppLog::warn(_tag, "canHandle: " + pRequest->requestPath());
  std::string sRequestPath = pRequest->getRequestPath();

  if (sRequestPath == "") {
    WsjcppLog::err(_tag, "Request path is empty");
    return false;
  }

  if (sRequestPath == "/admin") {
    sRequestPath = "/admin/";
  }

  if (sRequestPath == "/admin/") {
    sRequestPath = "/admin/index.html";
  }

  std::string s = "/admin/";
  if (sRequestPath.rfind("/admin/", 0) != 0) {
    return false;
  }

  if (!WsjcppCore::dirExists(m_sWebFolder)) {
    WsjcppLog::warn(_tag, "Directory " + m_sWebFolder + " does not exists");
  }
  sRequestPath = sRequestPath.substr(6); // remove /admin

  std::string sFilePath = m_sWebFolder + sRequestPath; // TODO check /../ in path
  // WsjcppLog::warn(_tag, "Response Resources " + sFilePath);
  if (!WsjcppCore::fileExists(sFilePath)) { // TODO check the file exists not dir
    return false;
  }
  return true;
}

bool HttpHandlerWebAdminFolder::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
  std::string _tag = TAG + "-" + sWorkerId;
  std::string sRequestPath = pRequest->getRequestPath();
  // WsjcppLog::warn(_tag, pRequest->requestPath());

  if (sRequestPath == "/admin") {
    sRequestPath = "/admin/";
  }

  if (sRequestPath == "/admin/") {
    sRequestPath = "/admin/index.html";
  }

  sRequestPath = sRequestPath.substr(6); // remove /admin

  std::string sFilePath = m_sWebFolder + sRequestPath; // TODO check /../ in path
  if (!WsjcppCore::fileExists(sFilePath)) {
    // std::string sResPath = "html" + sRequestPath;
    /*if (ResourcesManager::has(sResPath)) {
        // WsjcppLog::warn(_tag, "Response Resources " + sResPath);
        // ResourceFile *pFile = ResourcesManager::get(sResPath);
        // pRequest->responseBuffer(sResPath, pFile->buffer(),
    pFile->bufferSize()); return true;
    }*/
    return false;
  }

  // TODO 1. if file exists and last date change more that in cache so need
  // update
  // TODO 2. if file not exists but in in resources - response them
  WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
  // WsjcppLog::warn(_tag, "Response File " + sFilePath);
  resp.cacheSec(60).ok().sendFile(sFilePath);
  return true;
}

// ---------------------------------------------------------------------
// EmployWebServer

REGISTRY_WJSCPP_EMPLOY(EmployWebServer)

WsjcppLightWebServer g_httpServer;

EmployWebServer::EmployWebServer()
  : WsjcppEmployBase(
      EmployWebServer::name(), {EmployDatabase::name(), EmployGlobalSettings::name(), EmployNotify::name()}
    ) {
  TAG = EmployWebServer::name();
}

bool EmployWebServer::init() { return true; }

bool EmployWebServer::deinit() { return true; }

int EmployWebServer::start(QCoreApplication *pQtApp) {

  auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();
  // auto *pGlobalSettings = findWsjcppEmploy<EmployGlobalSettings>();

  pGlobalSettings->registrySetting("web_server", "web_admin_folder")
    .dirPath("/usr/share/fhq-server/web-admin")
    .inFile();
  pGlobalSettings->registrySetting("web_server", "web_user_folder").dirPath("/usr/share/fhq-server/web-user").inFile();
  pGlobalSettings->registrySetting("web_server", "web_public_folder")
    .dirPath("/usr/share/fhq-server/fhq-web-public")
    .inFile();
  pGlobalSettings->registrySetting("web_server", "web_public_folder_url")
    .string("http://localhost:7080/public/")
    .inFile();

  WsjcppEmployees::init({"start_server"});

  WsjcppLog::info(TAG, "Init handlers");
  CmdHandlers::init();

  QThreadPool::globalInstance()->setMaxThreadCount(5);
  WebSocketServer *pServer = new WebSocketServer(); // here will be init settings
  if (pServer->isFailed()) {
    WsjcppLog::err(TAG, "Could not start server");
    return -1;
  }

  QObject::connect(pServer, &WebSocketServer::closed, pQtApp, &QCoreApplication::quit);
  EmployDatabase *pDatabase = findWsjcppEmploy<EmployDatabase>();
  // TODO redesign to check config
  QSqlDatabase *db = pDatabase->database();
  if (!db->open()) {
    return -1;
  }

  // TODO move inside server start
  // start web server
  int nWebPort = pGlobalSettings->get("web_port").getNumberValue();
  int nWebMaxThreads = pGlobalSettings->get("web_max_threads").getNumberValue();
  std::string sWebAdminFolder = pGlobalSettings->get("web_admin_folder").getDirPathValue();
  std::string sWebUserFolder = pGlobalSettings->get("web_user_folder").getDirPathValue();
  std::string sWebPublicFolder =
    pGlobalSettings->get("web_public_folder").getDirPathValue(); // TODO must be declared in server
  std::string sFileStorage = pGlobalSettings->get("file_storage").getDirPathValue();
  std::string sWebPublicFolderUrl =
    pGlobalSettings->get("web_public_folder_url").getStringValue(); // TODO must be declared in server

  WsjcppLog::info(
    TAG,
    "Starting web-server on " + std::to_string(nWebPort) + " with " + std::to_string(nWebMaxThreads) + " worker threads"
  );

  g_httpServer.addHandler(new HttpHandlerWebAdminFolder(sWebAdminFolder));
  g_httpServer.addHandler(new HttpHandlerWebPublicFolder(sWebPublicFolder, sFileStorage));
  g_httpServer.addHandler(new HttpHandlerWebUserFolder(sWebUserFolder));

  g_httpServer.setPort(nWebPort);
  g_httpServer.setMaxWorkers(nWebMaxThreads);
  g_httpServer.start(); // no block this thread

  // start migration to new http server
  FhqHttpServer httpServer;
  httpServer.setWebAdminFolder(sWebAdminFolder);
  httpServer.setWebPublicFolder(sWebPublicFolder);
  httpServer.setWebUserFolder(sWebUserFolder);
  hv::HttpService *pRouter = httpServer.getService();
  hv::HttpServer server(pRouter);
  server.setPort(nWebPort + 1);
  server.setThreadNum(nWebMaxThreads);
  // server.run(); // wait
  server.start(); // no wait

  return pQtApp->exec(); // need for a treadpool and for websockets
}
