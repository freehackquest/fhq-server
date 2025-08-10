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

#include "wsjcpp_export_libcli_web_js.h"

#include <algorithm>
#include <ctime>
#include <employees.h>
#include <fstream>
#include <iomanip>
#include <iomanip> // put_time
#include <iostream>
#include <memory>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <websocketserver.h>

namespace WsjCpp {

// ---------------------------------------------------------------------

ExportLibCliWebJS::ExportLibCliWebJS() {
  m_sAppName = "wjscpp";
  m_sPackageName = "libwjscpp-web-js";
  m_sLibraryName = "wjscpp";
  m_sAppName = "wjscpp";
  m_sAppVersion = "0.0.0";
  m_sAuthor = "wjscpp";
  m_sPrefixRepositoryURL = "https://github.com/wjscpp/";
  m_nCurrentTimeInSec = WsjcppCore::getCurrentTimeInSeconds();
  m_sCurrentDate = WsjcppCore::formatTimeUTC(m_nCurrentTimeInSec);

  m_vEvents.push_back("server");
  m_vEvents.push_back("notify");
  m_vEvents.push_back("chat");
  m_vEvents.push_back("connected");
  m_vEvents.push_back("reconnecting");
  m_vEvents.push_back("disconnected");
  m_vEvents.push_back("broken");
  m_vEvents.push_back("userdata");
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::setPackageName(const std::string &sPackageName) { m_sPackageName = sPackageName; }

// ---------------------------------------------------------------------

void ExportLibCliWebJS::setLibraryName(const std::string &sLibraryName) { m_sLibraryName = sLibraryName; }

// ---------------------------------------------------------------------

void ExportLibCliWebJS::setAppName(const std::string &sAppName) { m_sAppName = sAppName; }

// ---------------------------------------------------------------------

void ExportLibCliWebJS::setAppVersion(const std::string &sAppVersion) { m_sAppVersion = sAppVersion; }

// ---------------------------------------------------------------------

void ExportLibCliWebJS::setAuthor(const std::string &sAuthor) { m_sAuthor = sAuthor; }

// ---------------------------------------------------------------------

void ExportLibCliWebJS::setPrefixRepositoryURL(const std::string &sPrefixRepositoryURL) {
  m_sPrefixRepositoryURL = sPrefixRepositoryURL;
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::exportLib() {
  std::string sBasicDir = "./" + m_sPackageName;

  exportPrepareDirs(sBasicDir);
  exportLibCliWebJSFile(sBasicDir);
  exportLibCliWebServiceTSFile(sBasicDir);
  exportSampleHtmlFile(sBasicDir);
  exportPackageJson(sBasicDir);
  exportAPImd(sBasicDir);
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::exportPrepareDirs(const std::string &sBasicDir) {
  std::vector<std::string> vDirs;
  vDirs.push_back(sBasicDir);
  vDirs.push_back(sBasicDir + "/dist");

  for (int i = 0; i < vDirs.size(); i++) {
    std::string sDir = vDirs[i];
    if (!WsjcppCore::dirExists(sDir)) {
      std::cout << " * mkdir " << sDir << std::endl
                << (WsjcppCore::makeDir(sDir) ? "\t> OK" : "\t> FAILED") << std::endl;
    } else {
      std::cout << " * Directory already exists " << sDir << std::endl;
    }
  }
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::exportPackageJson(const std::string &sBasicDir) {
  std::ofstream packageJson;
  std::cout << " * write code to " << sBasicDir << "/package.json " << std::endl;
  packageJson.open(sBasicDir + "/package.json");

  // look here an example
  // https://github.com/jquery/jquery/blob/master/package.json
  packageJson << "{\n"
                 "  \"name\": \"" +
                     m_sPackageName +
                     "\",\n"
                     "  \"version\": \"" +
                     m_sAppVersion +
                     "\",\n"
                     "  \"description\": \"FreeHackQuest JavaScript Web Client "
                     "Library for " +
                     m_sAppName +
                     "\",\n"
                     "  \"main\": \"dist/" +
                     m_sPackageName +
                     ".js\",\n"
                     "  \"repository\": {\n"
                     "    \"type\": \"git\",\n"
                     "    \"url\": \"" +
                     m_sPrefixRepositoryURL + m_sPackageName +
                     ".git\"\n"
                     "  },\n"
                     "  \"keywords\": [\n"
                     "    \"browser\",\n"
                     "    \"freehackquest\",\n"
                     "    \"javascript\",\n"
                     "    \"fhq-server\",\n"
                     "    \"library\",\n"
                     "    \"client\",\n"
                     "    \"websocket\"\n"
                     "  ],\n"
                     "  \"bugs\": {\n"
                     "    \"url\": \"" +
                     m_sPrefixRepositoryURL + m_sPackageName +
                     "/issues\"\n"
                     "  },\n"
                     "  \"author\": \"" +
                     m_sAuthor +
                     "\",\n"
                     "  \"license\": \"MIT\",\n"
                     "  \"licenses\": [{\n"
                     "    \"type\": \"MIT\",\n"
                     "    \"url\": \"" +
                     m_sPrefixRepositoryURL + m_sPackageName +
                     "/blob/master/LICENSE\"\n"
                     "  }]\n"
                     "}\n";
  packageJson.close();
  std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::exportAPImd(const std::string &sBasicDir) {

  std::ofstream apimd;
  std::cout << " * write file to " + sBasicDir + "/API.md" << std::endl;
  apimd.open(sBasicDir + "/API.md");

  // TODO redesign
  apimd << "# " + m_sPackageName +
               "\n\n"
               " Automatically generated by "
        << m_sAppName
        << ". \n"
           " * Version: "
        << m_sAppVersion
        << "\n"
           " * Date: "
        << m_sCurrentDate
        << "\n\n"
           " Include script ```dist/freehackquest-libclient-web-js.js```\n"
           " Example connect:\n"
           "```\n";
  for (int i = 0; i < m_vEvents.size(); i++) {
    apimd << "fhq.bind('" + m_vEvents[i] + "', function(data) { console.log('" + m_vEvents[i] + "', data)})\n";
  }
  apimd << "fhq.bind('connected', function(data) { console.log('connected', "
           "data)})\n"
           "// connect\n"
           "fhq.init({'baseUrl': 'ws://localhost:1234/'})\n"
           "// disconnect\n"
           "fhq.deinit()\n"
           "```\n"
           "\n";

  std::map<std::string, CmdHandlerBase *>::iterator it = g_pCmdHandlers->begin();
  for (; it != g_pCmdHandlers->end(); ++it) {
    std::string sCmd = it->first;
    CmdHandlerBase *pCmdHandlerBase = it->second;

    apimd << "<details>\n"
             "<summary>"
          << sCmd << " - " << pCmdHandlerBase->description()
          << "</summary>\n\n"
             "## "
          << sCmd << "\n\n";

    if (pCmdHandlerBase->description() != "") {
      apimd << pCmdHandlerBase->description() << "\n\n";
    }
    apimd << "Access: unauthorized - **" << (pCmdHandlerBase->accessUnauthorized() ? "yes" : "no")
          << "**, "
             " user - **"
          << (pCmdHandlerBase->accessUser() ? "yes" : "no")
          << "**, "
             " admin - **"
          << (pCmdHandlerBase->accessAdmin() ? "yes" : "no")
          << "**\n"
             "\n";

    apimd << " #### Input params \n\n";

    std::string jsTemplate = "";

    std::vector<CmdInputDef> vVin = pCmdHandlerBase->inputs();
    for (int i = 0; i < vVin.size(); i++) {
      CmdInputDef inDef = vVin[i];
      std::string nameIn = std::string(inDef.getName());

      apimd << " * " << inDef.getName() << " - " << inDef.getType() << ", " << inDef.getRestrict() << "; "
            << inDef.getDescription() << "\n";

      if (jsTemplate != "") {
        jsTemplate += ",\n";
      }
      if (inDef.isInteger()) {
        int nVal = 0;
        if (inDef.getName() == "onpage") {
          nVal = 10;
        }
        jsTemplate += "    \"" + inDef.getName() + "\": " + std::to_string(nVal);
      } else {
        jsTemplate += "    \"" + inDef.getName() + "\": \"\"";
      }
    }
    WsjcppCore::replaceAll(sCmd, ".", "_");
    apimd << "\n\n"
             " #### example call method \n\n"
             "```\n"
             "fhq." +
                 sCmd + "({\n" + jsTemplate +
                 "\n}).done(function(r) {\n"
                 "    console.log('Success: ', r);\n"
                 "}).fail(function(err) {\n"
                 "    console.error('Error:', err);\n"
                 "});\n"
                 "```"
                 "\n\n"
                 "</details>"
                 "\n\n";
  }

  apimd.close();
  std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::exportSampleHtmlFile(const std::string &sBasicDir) {
  std::string sFilename = sBasicDir + "/sample.html";
  std::ofstream sample_html;
  std::cout << " * write code to " << sFilename << std::endl;
  sample_html.open(sFilename);
  sample_html << "<html>\r\n"
                 "<head>\r\n"
                 "    <script src='dist/" +
                     m_sPackageName +
                     ".js'></script>\r\n"
                     "    <script>\r\n"
                     "        function log(n) {\r\n"
                     "            console.log(n);\r\n"
                     "            var log = document.getElementById('log');\r\n"
                     "            log.innerHTML += '[' + new Date() + '] ' + n + "
                     "'\\r\\n';\r\n"
                     "        };\r\n"
                     "        document.addEventListener('DOMContentLoaded', function() "
                     "{\r\n"
                     "            fhq.bind('notify', function(data) {\r\n"
                     "                log('notify: ' + JSON.stringify(data))\r\n"
                     "            });\r\n"
                     "            fhq.bind('connected', function(data) {\r\n"
                     "                log('connected: ' + JSON.stringify(data))\r\n"
                     "                connecting_form.style.display = 'none';\r\n"
                     "                login_form.style.display = '';\r\n"
                     "                logout_form.style.display = 'none';\r\n"
                     "            });\r\n"
                     "            fhq.bind('disconnected', function(data) {\r\n"
                     "                log('disconnected: ' + JSON.stringify(data))\r\n"
                     "                connecting_form.style.display = '';\r\n"
                     "                login_form.style.display = 'none';\r\n"
                     "                logout_form.style.display = 'none';\r\n"
                     "            });\r\n"
                     "            fhq.bind('userdata', function(data) {\r\n"
                     "                log('userdata: ' + JSON.stringify(data))\r\n"
                     "                login_form.style.display = 'none';\r\n"
                     "                logout_form.style.display = '';\r\n"
                     "            });\r\n"
                     "        });\r\n"
                     "    </script>\r\n"
                     "</head>\r\n"
                     "<body>\r\n"
                     "<div id='connecting_form'>\r\n"
                     "    <h1>Connect to Server</h1>\r\n"
                     "    <input type='text' id='server_url'><br>\r\n"
                     "    <button id='btn_connect'>Connect</button>\r\n"
                     "</div>\r\n"
                     "<div id='login_form' style='display: none'>\r\n"
                     "    <h1>Login</h1>\r\n"
                     "    <input type='text' id='login'><br>\r\n"
                     "    <input type='password' id='password'><br>\r\n"
                     "    <button id='btn_login'>Login</button>\r\n"
                     "</div>\r\n"
                     "<div id='logout_form' style='display: none'>\r\n"
                     "    <h1>Connected</h1>\r\n"
                     "    <button id='btn_logout'>Logout</button>\r\n"
                     "</div>\r\n"
                     "<script>\r\n"
                     "    server_url.value = 'ws://' + window.location.hostname + "
                     "':1234/api-ws/'\r\n"
                     "    btn_login.onclick = function() {\r\n"
                     "        fhq.login({email: login.value, password: "
                     "password.value}).done(function(r) { \r\n"
                     "            log('Login success, token = ' + r.token);\r\n"
                     "        }).fail(function(e) {\r\n"
                     "            log('Login failed, error = ' + e.error);\r\n"
                     "        })\r\n"
                     "    }\r\n"
                     "    btn_logout.onclick = function() {\r\n"
                     "        fhq.deinit();\r\n"
                     "    }\r\n"
                     "    btn_connect.onclick = function() {\r\n"
                     "        fhq.init({'baseUrl': server_url.value});\r\n"
                     "    }\r\n"
                     "</script>\r\n"
                     "<pre id='log'></pre>\r\n"
                     "</body>\r\n"
                     "</html>\r\n";
  sample_html.close();
  std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::exportLibCliWebJSFile(const std::string &sBasicDir) {

  std::string sFilename = sBasicDir + "/dist/" + m_sPackageName + ".js";
  std::ofstream libwjscppcli_web_js_file;
  std::cout << " * write code to " << sFilename << std::endl;
  libwjscppcli_web_js_file.open(sFilename);

  libwjscppcli_web_js_file << "// This file was automatically generated by " << m_sAppName
                           << " (v" + m_sAppVersion + "), date: " << m_sCurrentDate
                           << "\r\n"
                              "window."
                           << m_sLibraryName << " = window." << m_sLibraryName
                           << " || (function() { \r\n"
                              "    var self = {};\r\n"
                              "    self.appName = '" +
                                  m_sAppName +
                                  "';\r\n"
                                  "    self.appVersion = '" +
                                  m_sAppVersion +
                                  "';\r\n"
                                  "    self.appBuildDate = '" +
                                  m_sCurrentDate +
                                  "';\r\n"
                                  "    var _lastm = 0;\r\n"
                                  "    var _listeners = {};\r\n"
                                  "    var _connectionState = '?';\r\n"
                                  "    var _tokenValue = '';\r\n"
                                  "    var _events = {\r\n";

  for (int i = 0; i < m_vEvents.size(); i++) {
    libwjscppcli_web_js_file << "        '" << m_vEvents[i] << "': [],\r\n";
  }

  libwjscppcli_web_js_file << "    };\r\n"
                              "    function _lm() { _lastm++; return 'm' + _lastm; };\r\n"
                              "    console.warn('" +
                                  m_sLibraryName + " (" + m_sAppVersion +
                                  ")');\r\n"
                                  "    self.promise = function() {\r\n"
                                  "        return {\r\n"
                                  "            completed: false, failed: false, successed: false, "
                                  "\r\n"
                                  "            done: function(callback) {\r\n"
                                  "                this.done_callback = callback;\r\n"
                                  "                if (this.completed && typeof this.done_callback "
                                  "=== 'function' && this.successed) {\r\n"
                                  "                    this.done_callback.apply(this, "
                                  "this.result_arguments);\r\n"
                                  "                }\r\n"
                                  "                return this;\r\n"
                                  "            },\r\n"
                                  "            fail: function(callback) {\r\n"
                                  "                this.fail_callback = callback;\r\n"
                                  "                if (this.completed && typeof this.fail_callback "
                                  "=== 'function' && this.failed) {\r\n"
                                  "                    this.fail_callback.apply(this, "
                                  "this.error_arguments);\r\n"
                                  "                }\r\n"
                                  "                return this;\r\n"
                                  "            },\r\n"
                                  "            resolve: function() {\r\n"
                                  "                if (!this.completed) {\r\n"
                                  "                    this.result_arguments = arguments; // [];\r\n"
                                  "                    if (typeof this.done_callback === "
                                  "'function') {\r\n"
                                  "                        this.done_callback.apply(this, "
                                  "this.result_arguments);\r\n"
                                  "                    }\r\n"
                                  "                }\r\n"
                                  "                this.successed = true;\r\n"
                                  "                this.completed = true;\r\n"
                                  "            },\r\n"
                                  "            reject: function() {\r\n"
                                  "                if (!this.completed) {\r\n"
                                  "                    this.error_arguments = arguments;\r\n"
                                  "                    if (typeof this.fail_callback === "
                                  "'function') {\r\n"
                                  "                        this.fail_callback.apply(this, "
                                  "this.error_arguments);\r\n"
                                  "                    }\r\n"
                                  "                }\r\n"
                                  "                this.failed = true;\r\n"
                                  "                this.completed = true;\r\n"
                                  "            }\r\n"
                                  "        }; // end of promise\r\n"
                                  "    };\r\n"
                                  "    self.waitAllPromises = function(arr_promise) {\r\n"
                                  "        var p = self.promise();\r\n"
                                  "        var max_len = arr_promise.length;\r\n"
                                  "        var result = [];\r\n"
                                  "        function cmpl(r) {\r\n"
                                  "            result.push(r);\r\n"
                                  "            if (result.length == max_len) {\r\n"
                                  "                p.resolve(result);\r\n"
                                  "            }\r\n"
                                  "        };\r\n"
                                  "        for (var i in arr_promise) {\r\n"
                                  "            arr_promise[i].done(cmpl).fail(cmpl);\r\n"
                                  "        }\r\n"
                                  "        return p;\r\n"
                                  "    };\r\n"
                                  "    self.setToken = function(token) {\r\n"
                                  "        var date = new Date( new Date().getTime() + (7 * 24 * 60 "
                                  "* 60 * 1000) ); // cookie on week\r\n"
                                  "        document.cookie = 'fhqtoken=' + "
                                  "encodeURIComponent(token) + '; path=/; "
                                  "expires='+date.toUTCString();\r\n"
                                  "    }\r\n"
                                  "    self.removeToken = function() {\r\n"
                                  "        _tokenValue = '';"
                                  "        document.cookie = 'fhqtoken=; path=/;';\r\n"
                                  "    }\r\n"
                                  "    self.getToken = function() {\r\n"
                                  "        var matches = document.cookie.match(new RegExp(\r\n"
                                  "            '(?:^|; )' + "
                                  "'fhqtoken'.replace(/([\\.$?*|{}\\(\\)\\[\\]\\\\\\/\\+^])/g, "
                                  "'\\\\$1') + '=([^;]*)'\r\n"
                                  "        ));\r\n"
                                  "        return matches ? decodeURIComponent(matches[1]) : '';\r\n"
                                  "    }\r\n"
                                  "    _tokenValue = self.getToken();\r\n"
                                  "    self.bind = function(name, f) { _events[name].push(f); }\r\n"
                                  "    self.unbind = function(name) { _events[name] = []; }\r\n"
                                  "    function _call(name, data) {\r\n"
                                  "        function __call(f, data) { setTimeout(function() { "
                                  "f(data) },1)}"
                                  "        for (var i = 0; i < _events[name].length; i++) {\r\n"
                                  "            __call(_events[name][i], data);\r\n"
                                  "        }\r\n"
                                  "    }\r\n"
                                  "    self.bind('server', function(response) { \r\n"
                                  "       console.warn('All: ', response);\r\n"
                                  "       if (response.app != self.appName) {\r\n"
                                  "           console.error('AppName: ' + response.app + ', but "
                                  "expected ' + self.appName);\r\n"
                                  "       }\r\n"
                                  "       if (response.version != self.appVersion) {\r\n"
                                  "           console.error('AppVersion: ' + response.version + ', "
                                  "but expected ' + self.appVersion);\r\n"
                                  "       }\r\n"
                                  "    }); \r\n"
                                  "    self.handleCommand = function(response) {\r\n"
                                  "       var lstn = _listeners[response.m];\r\n"
                                  "       if (lstn) {\r\n"
                                  "           setTimeout(function() {\r\n"
                                  "               if (response['error']) {\r\n"
                                  "                   lstn.reject(response);\r\n"
                                  "               } else {\r\n"
                                  "                   lstn.resolve(response);\r\n"
                                  "               }\r\n"
                                  "               delete _listeners[response.m];\r\n"
                                  "           },1);\r\n"
                                  "       } else if (_events[response.cmd]) {\r\n"
                                  "           _call(response.cmd, response);"
                                  "       } else {\r\n"
                                  "           console.error('Not found handler for [' + "
                                  "response.cmd + '/' + response.m + ']');\r\n"
                                  "       }\r\n"
                                  "   };\r\n"
                                  "   self.send = function(obj, def) {\r\n"
                                  "       obj.m = obj.m || _lm();\r\n"
                                  "       _listeners[obj.m] = def || self.promise();\r\n"
                                  "       try {\r\n"
                                  "           if (self.socket.readyState == 0) {\r\n"
                                  "               setTimeout(function() {\r\n"
                                  "                   self.send(obj, _listeners[obj.m]);\r\n"
                                  "               },1000);\r\n"
                                  "           } else {\r\n"
                                  "               self.socket.send(JSON.stringify(obj));\r\n"
                                  "           }\r\n"
                                  "       } catch(e) {\r\n"
                                  "           console.error(e);\r\n"
                                  "       }\r\n"
                                  "       return _listeners[obj.m];\r\n"
                                  "    }\r\n"
                                  "    self.init = function(initParams) {\r\n"
                                  "        if (!initParams.baseUrl) console.error('Expected baseUrl "
                                  "on initParams');\r\n"
                                  "        self.socket = new WebSocket(initParams.baseUrl);\r\n"
                                  "        self.socket.onopen = function() {\r\n"
                                  "           console.log('WS Opened');\r\n"
                                  "           _call('connected', {});\r\n"
                                  "           if (_tokenValue != '') self.token();\r\n"
                                  "        };\r\n"
                                  "        self.socket.onclose = function(event) {\r\n"
                                  "           console.log('Closed');\r\n"
                                  "           if (event.wasClean) {\r\n"
                                  "               _call('disconnected', {});\r\n"
                                  "           } else {\r\n"
                                  "               _call('broken', {});\r\n"
                                  "               setTimeout(function() {\r\n"
                                  "                   _call('reconnecting', {});\r\n"
                                  "                   self.init(initParams);\r\n"
                                  "               }, 10000);\r\n"
                                  "             // Try reconnect after 5 sec\r\n"
                                  "           }\r\n"
                                  "           console.log('Code: ' + event.code + ' Reason: ' + "
                                  "event.reason);\r\n"
                                  "       };\r\n"
                                  "       self.socket.onmessage = function(event) {\r\n"
                                  "           // console.log('Received: ' + event.data);\r\n"
                                  "           try {\r\n"
                                  "               var response = JSON.parse(event.data);\r\n"
                                  "               self.handleCommand(response);\r\n"
                                  "           } catch(e) {\r\n"
                                  "               console.error(e);\r\n"
                                  "           }\r\n"
                                  "           \r\n"
                                  "       };\r\n"
                                  "       self.socket.onerror = function(error) {\r\n"
                                  "           console.log('Error: ' + error.message);\r\n"
                                  "       };\r\n"
                                  "    }\r\n"
                                  "    self.deinit = function(initParams) {\r\n"
                                  "       self.removeToken();\r\n"
                                  "       self.socket.close();\r\n"
                                  "    }\r\n"
                                  "   self.userProfile = {bInitUserProfile: false}\r\n"
                                  "   self.updateUserProfileAsync = function() {\r\n"
                                  "       setTimeout(function() {\r\n"
                                  "           self.user().done(function(r) {\r\n"
                                  "               self.userProfile.bInitUserProfile == true;\r\n"
                                  "               self.userProfile.university = "
                                  "r.data.university;\r\n"
                                  "               self.userProfile.country = r.data.country;\r\n"
                                  "               self.userProfile.city = r.data.city;\r\n"
                                  "               self.userinfo = {};\r\n"
                                  "               self.userinfo.id = r.data.id;\r\n"
                                  "               self.userinfo.nick = r.data.nick;\r\n"
                                  "               self.userinfo.email = r.data.email;\r\n"
                                  "               self.userinfo.role = r.data.role;\r\n"
                                  "               self.userinfo.logo = r.data.logo;\r\n"
                                  "               _call('userdata', r.data);\r\n"
                                  "           }).fail(function() {\r\n"
                                  "               self.removeToken();\r\n"
                                  "               _call('userdata', {});\r\n"
                                  "           });\r\n"
                                  "       },10);\r\n"
                                  "   }\r\n";

  std::map<std::string, CmdHandlerBase *>::iterator it = g_pCmdHandlers->begin();
  for (; it != g_pCmdHandlers->end(); ++it) {
    std::string sCmd = it->first;
    CmdHandlerBase *pCmdHandlerBase = it->second;
    std::string sMethodName = sCmd;
    WsjcppCore::replaceAll(sMethodName, ".", "_");
    libwjscppcli_web_js_file << "    self." << sMethodName << " = function(params) {\r\n";

    libwjscppcli_web_js_file << "       // Access unauthorized: "
                             << (pCmdHandlerBase->accessUnauthorized() ? "yes" : "no")
                             << "\r\n"
                                "       // Access user: "
                             << (pCmdHandlerBase->accessUser() ? "yes" : "no")
                             << "\r\n"
                                "       // Access admin: "
                             << (pCmdHandlerBase->accessAdmin() ? "yes" : "no") << "\r\n";

    if (pCmdHandlerBase->activatedFromVersion() != "") {
      libwjscppcli_web_js_file << "       // Activated From Version: " << pCmdHandlerBase->activatedFromVersion()
                               << "\r\n";
    }

    if (pCmdHandlerBase->deprecatedFromVersion() != "") {
      libwjscppcli_web_js_file << "       // Deprecated From Version: " + pCmdHandlerBase->deprecatedFromVersion()
                               << "\r\n";
    }

    std::vector<CmdInputDef> vVin = pCmdHandlerBase->inputs();
    if (vVin.size() > 0) {
      libwjscppcli_web_js_file << "       // Input params:\r\n";
    }
    for (int i = 0; i < vVin.size(); i++) {
      CmdInputDef inDef = vVin[i];
      std::string nameIn = std::string(inDef.getName());
      libwjscppcli_web_js_file << "       // * " + nameIn + " - " + inDef.getType() + ", " + inDef.getRestrict() +
                                      " (" + inDef.getDescription() + ") \r\n";
    }

    libwjscppcli_web_js_file << "        params = params || {};\r\n"
                                "        params.cmd = '"
                             << sCmd << "';\r\n";
    // check required
    for (int i = 0; i < vVin.size(); i++) {
      CmdInputDef inDef = vVin[i];
      if (inDef.isRequired()) {
        std::string nameIn = std::string(vVin[i].getName());
        libwjscppcli_web_js_file << "        if (params['" + nameIn +
                                        "'] == undefined) {\r\n"
                                        "             console.error('Parameter \""
                                 << nameIn
                                 << "\" expected (lib)');\r\n"
                                    "        }\r\n";
      }
    }
    if (sCmd == "login") {
      libwjscppcli_web_js_file << "        var ret = self.promise()\r\n"
                                  "        self.send(params).done(function(r) {\r\n"
                                  "            _tokenValue = r.token;\r\n"
                                  "            console.log(_tokenValue);\r\n"
                                  "            self.userinfo = r.user;\r\n"
                                  "            self.setToken(_tokenValue);\r\n"
                                  "            self.updateUserProfileAsync();\r\n"
                                  "            ret.resolve(r);\r\n"
                                  "        }).fail(function(err) {\r\n"
                                  "            self.removeToken();\r\n"
                                  "            ret.reject(err);\r\n"
                                  "        })\r\n"
                                  "        return ret;\r\n";
    } else if (sCmd == "token") {
      libwjscppcli_web_js_file << "         if (_tokenValue != '') {\r\n"
                                  "             var ret = self.promise()\r\n"
                                  "             params.token = _tokenValue;\r\n"
                                  "             self.send(params).done(function(r) {\r\n"
                                  "                 self.updateUserProfileAsync();\r\n"
                                  "                 ret.resolve(r);\r\n"
                                  "             }).fail(function(err) {\r\n"
                                  "                 self.removeToken();\r\n"
                                  "                 _call('userdata', {});\r\n"
                                  "                 ret.reject(err);\r\n"
                                  "             })\r\n"
                                  "             return ret;\r\n"
                                  "         } else {\r\n"
                                  "             return self.send(params);\r\n"
                                  "         }\r\n";
    } else {
      libwjscppcli_web_js_file << "         return self.send(params);\r\n";
    }

    libwjscppcli_web_js_file << "    }\r\n\r\n";
  }

  libwjscppcli_web_js_file << "    return self;\r\n"
                              "})();\r\n";

  libwjscppcli_web_js_file.close();
  std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::exportLibCliWebServiceTSFile(const std::string &sBasicDir) {
  std::string sFilename = sBasicDir + "/dist/libfreehackquest-client-angular.service.ts";
  std::ofstream libwjscppcli_web_service_ts_file;
  std::cout << " * write code to " << sFilename << std::endl;
  libwjscppcli_web_service_ts_file.open(sFilename);

  libwjscppcli_web_service_ts_file << "// This file was automatically generated by " << m_sAppName
                                   << " (" + m_sAppVersion + "), date: " << m_sCurrentDate
                                   << "\r\n"
                                      "import { Injectable, EventEmitter } from '@angular/core';\r\n"
                                      "import { PlatformLocation } from '@angular/common';\r\n"
                                      "import { Observable } from 'rxjs';\r\n"
                                      "\r\n"
                                      "declare var FreeHackQuestClientConfig: any;\r\n"
                                      "\r\n"
                                      "@Injectable({providedIn: 'root'})\r\n"
                                      "export class FreeHackQuestClient {\r\n"
                                      "  private appName: string = '" +
                                          m_sAppName +
                                          "';\r\n"
                                          "  private appVersion: string = '" +
                                          m_sAppVersion +
                                          "';\r\n"
                                          "  private appBuildDate: string = '" +
                                          m_sCurrentDate +
                                          "';\r\n"
                                          "  isAuthorized: boolean = false;\r\n"
                                          "  connectionState: string = '';\r\n"
                                          "  serverHost: string = 'localhost';\r\n"
                                          "  currentProtocol: string = 'http:';\r\n"
                                          "  private _tokenName: string = '" +
                                          m_sLibraryName +
                                          "token';\r\n"
                                          "  userdata: any = {};\r\n"
                                          "  changedState = new EventEmitter<any>();\r\n"
                                          "  private _lastm: number = 0;\r\n"
                                          "  private _listeners: any = {};\r\n"
                                          "  private _connectionState: string = '?';\r\n"
                                          "  private _tokenValue: string = '';\r\n"
                                          "  private _socket: WebSocket = null;\r\n"
                                          "  private _events: any = {\r\n";

  for (int i = 0; i < m_vEvents.size(); i++) {
    libwjscppcli_web_service_ts_file << "    '" << m_vEvents[i] << "': [],\r\n";
  }

  libwjscppcli_web_service_ts_file << "  };\r\n"
                                      "  private _lastConnectionParams: any = null;\r\n"
                                      "\r\n"
                                      "  constructor(\r\n"
                                      "    private _location: PlatformLocation,\r\n"
                                      "  ) {\r\n"
                                      "    this.serverHost = this._location.hostname;\r\n"
                                      "    // this.serverHost = 'freehackquest.com';\r\n"
                                      "    this.currentProtocol = this._location.protocol;\r\n"
                                      "    console.warn('" +
                                          m_sLibraryName + " (" + m_sAppVersion +
                                          ")');\r\n"
                                          "    this._tokenValue = this.getToken();\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  setToken(token: string) {\r\n"
                                          "    var date = new Date( new Date().getTime() + (7 * 24 * 60 * "
                                          "60 * 1000) ); // cookie on week\r\n"
                                          "    document.cookie = this._tokenName + '=' + "
                                          "encodeURIComponent(token) + '; path=/; "
                                          "expires='+date.toUTCString();\r\n"
                                          "  }\r\n"
                                          "  \r\n"
                                          "  removeToken() {\r\n"
                                          "    this._tokenValue = '';\r\n"
                                          "    document.cookie = this._tokenName + '=; path=/;';\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  getToken() {\r\n"
                                          "    var matches = document.cookie.match(new RegExp(\r\n"
                                          "      '(?:^|; )' + "
                                          "this._tokenName.replace(/([\\.$?*|{}\\(\\)\\[\\]\\\\\\/\\+^])/g, "
                                          "'\\\\$1') + '=([^;]*)'\r\n"
                                          "    ));\r\n"
                                          "    return matches ? decodeURIComponent(matches[1]) : '';\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  bind(name: string, f: Function) {\r\n"
                                          "    this._events[name].push(f);\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  unbind(name: string) {\r\n"
                                          "    this._events[name] = [];\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  _call(name: string, data: any) {\r\n"
                                          "    function __call(f: any, data: any) {\r\n"
                                          "      setTimeout(function() { f(data) },1)\r\n"
                                          "    }\r\n"
                                          "    for (var i = 0; i < this._events[name].length; i++) {\r\n"
                                          "      __call(this._events[name][i], data);\r\n"
                                          "    }\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  private socketOnOpen() {\r\n"
                                          "    console.log('FreeHackQuestClient WS Opened');\r\n"
                                          "    this._call('connected', {});\r\n"
                                          "    if (this._tokenValue != '') this.token();\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  private socketOnClose(event: any) {\r\n"
                                          "    console.log('FreeHackQuestClient Closed');\r\n"
                                          "    if (event.wasClean) {\r\n"
                                          "        this._call('disconnected', {});\r\n"
                                          "    } else {\r\n"
                                          "        this._call('broken', {});\r\n"
                                          "        setTimeout(function() {\r\n"
                                          "            this._call('reconnecting', {});\r\n"
                                          "            if (this._lastConnectionParams != null) {\r\n"
                                          "                "
                                          "this.connectToServer(this._lastConnectionParams);\r\n"
                                          "            }\r\n"
                                          "        }, 10000);\r\n"
                                          "        // Try reconnect after 10 sec\r\n"
                                          "    }\r\n"
                                          "    console.log('Code: ' + event.code + ' Reason: ' + "
                                          "event.reason);\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  private _socketOnMessage(event: any) {\r\n"
                                          "    console.log('FreeHackQuestClient Received: ' + "
                                          "event.data);\r\n"
                                          "    try {\r\n"
                                          "        var response = JSON.parse(event.data);\r\n"
                                          "        // this._handleCommand(response);\r\n"
                                          "    } catch(e) {\r\n"
                                          "        console.error(e);\r\n"
                                          "    }\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  private _socketOnError(error: any) {\r\n"
                                          "    console.log('FreeHackQuestClient Socket Error: ', error);\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  wsConnectToServer(initParams: any) {\r\n"
                                          "    this._lastConnectionParams = null;\r\n"
                                          "    if (!initParams.baseUrl) console.error('Expected baseUrl on "
                                          "initParams');\r\n"
                                          "    this._lastConnectionParams = initParams;\r\n"
                                          "    this._wsSocket = new WebSocket(initParams.baseUrl);\r\n"
                                          "    this._wsSocket.onopen = (() => this.socketOnOpen());\r\n"
                                          "    this._wsSocket.onclose = ((event: any) => "
                                          "this.socketOnClose(event));\r\n"
                                          "    this._wsSocket.onmessage = ((event: any) => "
                                          "this._socketOnMessage(event));\r\n"
                                          "    this._wsSocket.onerror = ((error: any) => "
                                          "this._socketOnError(error));\r\n"
                                          "  }\r\n"
                                          "  \r\n"
                                          "  wsConnected() {\r\n"
                                          "    this.connectionState = 'OK';\r\n"
                                          "    this.changedState.emit();\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  wsWait() {\r\n"
                                          "    this.connectionState = 'WAIT';\r\n"
                                          "    this.changedState.emit();\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  wsBroken() {\r\n"
                                          "    this.connectionState = 'BROKEN';\r\n"
                                          "    this.changedState.emit();\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  wsHandleMessage(data: any) {\r\n"
                                          "    if (data['id']) {\r\n"
                                          "      const msgid = data['id'];\r\n"
                                          "      if (this.waitResponseById[msgid]) {\r\n"
                                          "        if (data['error']) {\r\n"
                                          "          this.waitResponseById[msgid].error(data['error'])\r\n"
                                          "        } else {\r\n"
                                          "          this.waitResponseById[msgid].next(data['result'])\r\n"
                                          "        }\r\n"
                                          "        this.waitResponseById[msgid].complete();\r\n"
                                          "        this.waitResponseById[msgid] = null\r\n"
                                          "      } else {\r\n"
                                          "        console.error('Not found observer for ', msgid)\r\n"
                                          "      }\r\n"
                                          "    } else {\r\n"
                                          "      console.error('Not found id in incoming message')\r\n"
                                          "    }\r\n"
                                          "  }\r\n"
                                          "  wsSendMessage(request: String): void {\r\n"
                                          "    if (this.connectionState == \"OK\") {\r\n"
                                          "      this._wsSocket.send(request)\r\n"
                                          "    } else { // if (this.connectionState == \"WAIT\")\r\n"
                                          "      setTimeout(() => {\r\n"
                                          "        this.wsSendMessage(request)\r\n"
                                          "      }, 1000)\r\n"
                                          "    }\r\n"
                                          "  }\r\n"
                                          "  wsRequest(method: String, params: any): Observable<any> {\r\n"
                                          "    this.messageId++\r\n"
                                          "    const msgid = \"m\" + this.messageId;\r\n"
                                          "    let request = params;\r\n"
                                          "    request[\"jsonrpc\"] = \"2.0\";\r\n"
                                          // "      \"id\": msgid,\r\n"
                                          "    request[\"m\"] = msgid;\r\n"
                                          "    request[\"cmd\"] = method;\r\n"
                                          "    request[\"params\"] = params;\r\n"
                                          "    \r\n"
                                          "    let ret = new Observable(observer => {\r\n"
                                          "      if (this.connectionState != \"OK\" && this.connectionState "
                                          "!= \"WAIT\") {\r\n"
                                          "        observer.error(\"Server not responding... (\" + "
                                          "this.connectionState + \")\")\r\n"
                                          "        observer.complete()\r\n"
                                          "        return\r\n"
                                          "      }\r\n"
                                          "      this.wsSendMessage(JSON.stringify(request))\r\n"
                                          "      this.waitResponseById[msgid] = observer\r\n"
                                          "    });\r\n"
                                          "    return ret\r\n"
                                          "  }\r\n"
                                          "\r\n"
                                          "  private _lm() { this._lastm++; return 'm' + this._lastm; };\r\n"
                                          "\r\n";

  libwjscppcli_web_service_ts_file << "    function _lm() { _lastm++; return 'm' + _lastm; };\r\n"
                                      "    self.setToken = function(token) {\r\n"
                                      "        var date = new Date( new Date().getTime() + (7 * 24 * 60 * "
                                      "60 * 1000) ); // cookie on week\r\n"
                                      "        document.cookie = 'fhqtoken=' + encodeURIComponent(token) + "
                                      "'; path=/; expires='+date.toUTCString();\r\n"
                                      "    }\r\n"
                                      "    self.removeToken = function() {\r\n"
                                      "        _tokenValue = '';"
                                      "        document.cookie = 'fhqtoken=; path=/;';\r\n"
                                      "    }\r\n"
                                      "    self.getToken = function() {\r\n"
                                      "        var matches = document.cookie.match(new RegExp(\r\n"
                                      "            '(?:^|; )' + "
                                      "'fhqtoken'.replace(/([\\.$?*|{}\\(\\)\\[\\]\\\\\\/\\+^])/g, "
                                      "'\\\\$1') + '=([^;]*)'\r\n"
                                      "        ));\r\n"
                                      "        return matches ? decodeURIComponent(matches[1]) : '';\r\n"
                                      "    }\r\n"
                                      "    _tokenValue = self.getToken();\r\n"
                                      "    self.bind = function(name, f) { _events[name].push(f); }\r\n"
                                      "    self.unbind = function(name) { _events[name] = []; }\r\n"
                                      "    function _call(name, data) {\r\n"
                                      "        function __call(f, data) { setTimeout(function() { f(data) "
                                      "},1)}"
                                      "        for (var i = 0; i < _events[name].length; i++) {\r\n"
                                      "            __call(_events[name][i], data);\r\n"
                                      "        }\r\n"
                                      "    }\r\n"
                                      "    self.bind('server', function(response) { \r\n"
                                      "       console.warn('All: ', response);\r\n"
                                      "       if (response.app != self.appName) {\r\n"
                                      "           console.error('AppName: ' + response.app + ', but "
                                      "expected ' + self.appName);\r\n"
                                      "       }\r\n"
                                      "       if (response.version != self.appVersion) {\r\n"
                                      "           console.error('AppVersion: ' + response.version + ', but "
                                      "expected ' + self.appVersion);\r\n"
                                      "       }\r\n"
                                      "    }); \r\n"
                                      "    self.handleCommand = function(response) {\r\n"
                                      "       var lstn = _listeners[response.m];\r\n"
                                      "       if (lstn) {\r\n"
                                      "           setTimeout(function() {\r\n"
                                      "               if (response['error']) {\r\n"
                                      "                   lstn.reject(response);\r\n"
                                      "               } else {\r\n"
                                      "                   lstn.resolve(response);\r\n"
                                      "               }\r\n"
                                      "               delete _listeners[response.m];\r\n"
                                      "           },1);\r\n"
                                      "       } else if (_events[response.cmd]) {\r\n"
                                      "           _call(response.cmd, response);"
                                      "       } else {\r\n"
                                      "           console.error('Not found handler for [' + response.cmd + "
                                      "'/' + response.m + ']');\r\n"
                                      "       }\r\n"
                                      "   };\r\n"
                                      "   self.send = function(obj, def) {\r\n"
                                      "       obj.m = obj.m || _lm();\r\n"
                                      "       _listeners[obj.m] = def || self.promise();\r\n"
                                      "       try {\r\n"
                                      "           if (self.socket.readyState == 0) {\r\n"
                                      "               setTimeout(function() {\r\n"
                                      "                   self.send(obj, _listeners[obj.m]);\r\n"
                                      "               },1000);\r\n"
                                      "           } else {\r\n"
                                      "               self.socket.send(JSON.stringify(obj));\r\n"
                                      "           }\r\n"
                                      "       } catch(e) {\r\n"
                                      "           console.error(e);\r\n"
                                      "       }\r\n"
                                      "       return _listeners[obj.m];\r\n"
                                      "    }\r\n"
                                      "    self.init = function(initParams) {\r\n"
                                      "        if (!initParams.baseUrl) console.error('Expected baseUrl on "
                                      "initParams');\r\n"
                                      "        self.socket = new WebSocket(initParams.baseUrl);\r\n"
                                      "        self.socket.onopen = function() {\r\n"
                                      "           console.log('WS Opened');\r\n"
                                      "           _call('connected', {});\r\n"
                                      "           if (_tokenValue != '') self.token();\r\n"
                                      "        };\r\n"
                                      "        self.socket.onclose = function(event) {\r\n"
                                      "           console.log('Closed');\r\n"
                                      "           if (event.wasClean) {\r\n"
                                      "               _call('disconnected', {});\r\n"
                                      "           } else {\r\n"
                                      "               _call('broken', {});\r\n"
                                      "               setTimeout(function() {\r\n"
                                      "                   _call('reconnecting', {});\r\n"
                                      "                   self.init(initParams);\r\n"
                                      "               }, 10000);\r\n"
                                      "             // Try reconnect after 5 sec\r\n"
                                      "           }\r\n"
                                      "           console.log('Code: ' + event.code + ' Reason: ' + "
                                      "event.reason);\r\n"
                                      "       };\r\n"
                                      "       self.socket.onmessage = function(event) {\r\n"
                                      "           // console.log('Received: ' + event.data);\r\n"
                                      "           try {\r\n"
                                      "               var response = JSON.parse(event.data);\r\n"
                                      "               self.handleCommand(response);\r\n"
                                      "           } catch(e) {\r\n"
                                      "               console.error(e);\r\n"
                                      "           }\r\n"
                                      "           \r\n"
                                      "       };\r\n"
                                      "       self.socket.onerror = function(error) {\r\n"
                                      "           console.log('Error: ' + error.message);\r\n"
                                      "       };\r\n"
                                      "    }\r\n"
                                      "    self.deinit = function(initParams) {\r\n"
                                      "       self.removeToken();\r\n"
                                      "       self.socket.close();\r\n"
                                      "    }\r\n"
                                      "   self.userProfile = {bInitUserProfile: false}\r\n"
                                      "   self.updateUserProfileAsync = function() {\r\n"
                                      "       setTimeout(function() {\r\n"
                                      "           self.user().done(function(r) {\r\n"
                                      "               self.userProfile.bInitUserProfile == true;\r\n"
                                      "               self.userProfile.university = r.data.university;\r\n"
                                      "               self.userProfile.country = r.data.country;\r\n"
                                      "               self.userProfile.city = r.data.city;\r\n"
                                      "               self.userinfo = {};\r\n"
                                      "               self.userinfo.id = r.data.id;\r\n"
                                      "               self.userinfo.nick = r.data.nick;\r\n"
                                      "               self.userinfo.email = r.data.email;\r\n"
                                      "               self.userinfo.role = r.data.role;\r\n"
                                      "               self.userinfo.logo = r.data.logo;\r\n"
                                      "               _call('userdata', r.data);\r\n"
                                      "           }).fail(function() {\r\n"
                                      "               self.removeToken();\r\n"
                                      "               _call('userdata', {});\r\n"
                                      "           });\r\n"
                                      "       },10);\r\n"
                                      "   }\r\n";

  std::map<std::string, CmdHandlerBase *>::iterator it = g_pCmdHandlers->begin();
  for (; it != g_pCmdHandlers->end(); ++it) {
    std::string sCmd = it->first;
    CmdHandlerBase *pCmdHandlerBase = it->second;
    WsjcppCore::replaceAll(sCmd, ".", "_");

    libwjscppcli_web_service_ts_file << "  " << sCmd << "(value: Object): Observable<any> {\r\n";

    libwjscppcli_web_service_ts_file << "    // Access unauthorized: "
                                     << (pCmdHandlerBase->accessUnauthorized() ? "yes" : "no")
                                     << "\r\n"
                                        "    // Access user: "
                                     << (pCmdHandlerBase->accessUser() ? "yes" : "no")
                                     << "\r\n"
                                        "    // Access admin: "
                                     << (pCmdHandlerBase->accessAdmin() ? "yes" : "no") << "\r\n";

    if (pCmdHandlerBase->activatedFromVersion() != "") {
      libwjscppcli_web_service_ts_file << "    // Activated From Version: " << pCmdHandlerBase->activatedFromVersion()
                                       << "\r\n";
    }

    if (pCmdHandlerBase->deprecatedFromVersion() != "") {
      libwjscppcli_web_service_ts_file << "    // Deprecated From Version: " + pCmdHandlerBase->deprecatedFromVersion()
                                       << "\r\n";
    }

    std::vector<CmdInputDef> vVin = pCmdHandlerBase->inputs();
    if (vVin.size() > 0) {
      libwjscppcli_web_service_ts_file << "    // Input params:\r\n";
    }
    for (int i = 0; i < vVin.size(); i++) {
      CmdInputDef inDef = vVin[i];
      std::string nameIn = std::string(inDef.getName());
      libwjscppcli_web_service_ts_file << "    // * " + nameIn + " - " + inDef.getType() + ", " + inDef.getRestrict() +
                                              " (" + inDef.getDescription() + ") \r\n";
    }

    // check required
    for (int i = 0; i < vVin.size(); i++) {
      CmdInputDef inDef = vVin[i];
      if (inDef.isRequired()) {
        std::string nameIn = std::string(vVin[i].getName());
        libwjscppcli_web_service_ts_file << "    if (value['" << nameIn
                                         << "'] == undefined) {\r\n"
                                            "      console.error('Parameter \""
                                         << nameIn
                                         << "\" expected (lib)');\r\n"
                                            "    }\r\n";
      }
    }
    if (sCmd == "login") {
      libwjscppcli_web_service_ts_file << "        var ret = self.promise()\r\n"
                                          "        self.send(params).done(function(r) {\r\n"
                                          "          _tokenValue = r.token;\r\n"
                                          "          console.log(_tokenValue);\r\n"
                                          "          self.userinfo = r.user;\r\n"
                                          "          self.setToken(_tokenValue);\r\n"
                                          "          self.updateUserProfileAsync();\r\n"
                                          "          ret.resolve(r);\r\n"
                                          "        }).fail(function(err) {\r\n"
                                          "          self.removeToken();\r\n"
                                          "          ret.reject(err);\r\n"
                                          "        })\r\n"
                                          "        return ret;\r\n";
    } else if (sCmd == "token") {
      libwjscppcli_web_service_ts_file << "         if (_tokenValue != '') {\r\n"
                                          "           var ret = self.promise()\r\n"
                                          "           params.token = _tokenValue;\r\n"
                                          "           self.send(params).done(function(r) {\r\n"
                                          "             self.updateUserProfileAsync();\r\n"
                                          "             ret.resolve(r);\r\n"
                                          "           }).fail(function(err) {\r\n"
                                          "             self.removeToken();\r\n"
                                          "             _call('userdata', {});\r\n"
                                          "             ret.reject(err);\r\n"
                                          "           })\r\n"
                                          "           return ret;\r\n"
                                          "         } else {\r\n"
                                          "           return self.send(params);\r\n"
                                          "         }\r\n";
    } else {
      libwjscppcli_web_service_ts_file << "    return this.wsRequest('" << sCmd << "', value);\r\n\r\n";
    }

    libwjscppcli_web_service_ts_file << "  }\r\n\r\n";
  }

  libwjscppcli_web_service_ts_file << "    return self;\r\n"
                                      "})();\r\n";

  libwjscppcli_web_service_ts_file.close();
  std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

} // namespace WsjCpp