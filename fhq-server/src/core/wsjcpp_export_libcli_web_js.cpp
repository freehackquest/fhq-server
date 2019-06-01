#include "wsjcpp_export_libcli_web_js.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <websocketserver.h>
#include <wjscpp_employees.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <iomanip> // put_time
#include <ctime>
#include <sstream>
#include <fallen.h>

namespace WsjCpp {

// ---------------------------------------------------------------------

class JSCodeLine {
    
    JSCodeLine *m_pParent;
    std::string m_sLine;
    std::vector<JSCodeLine *> m_vLines;

public:
    
    JSCodeLine() {
        m_pParent = nullptr;
        m_sLine = "";
    }

    JSCodeLine(JSCodeLine *parent, const std::string &sLine) {
        m_pParent = parent;
        m_sLine = sLine;
    }

    ~JSCodeLine() {
        if (m_pParent == nullptr) {
            std::cout << "destruct root \n";
        } else {
            std::cout << "destruct something else [" << m_sLine << "]\n";
        }
    }

    JSCodeLine *addLine(const std::string &sLine) {
        JSCodeLine *pJSCodeLine = new JSCodeLine(this,sLine);
        m_vLines.push_back(pJSCodeLine);
        return pJSCodeLine;
    }

    JSCodeLine *getParent() {
        return m_pParent;
    }

    std::string getLine() {
        return m_sLine;
    }

    JSCodeLine *findRoot() {
        if (m_pParent == nullptr) {
            return this;
        }
        return m_pParent->findRoot();
    }

    void print(std::ofstream &__init__, std::string intent = "") {
        if (m_pParent != nullptr) {
            __init__ << intent << m_sLine << std::endl;
            intent += "  ";
        }
        for (int i = 0; i < m_vLines.size(); i++) {
            m_vLines[i]->print(__init__, intent);
        }
    }
};

// ---------------------------------------------------------------------

class JSCodeBuilder {

private:
    JSCodeLine *m_pCurr = nullptr;

public:
    JSCodeBuilder() {
        m_pCurr = new JSCodeLine();
    }

    ~JSCodeBuilder() {
        // std::cout << "destruct something else [" << m_pCurr->getLine() << "]\n";
    }

    JSCodeBuilder &add(const std::string &sLine) {
        m_pCurr->addLine(sLine);
        return *this;
    }
    JSCodeBuilder &sub(const std::string &sLine) {
        m_pCurr = m_pCurr->addLine(sLine);
        return *this;
    }
    JSCodeBuilder &end() {
        JSCodeLine *p = m_pCurr->getParent();
        if (p != nullptr) {
            m_pCurr = p;
        } else {
            std::cout << "Wrong called end function" << std::endl;
        }
        return *this;
    }

    void print(std::ofstream &__init__) {
        JSCodeLine *pRoot = m_pCurr->findRoot();
        pRoot->print(__init__);
    };
};


// ---------------------------------------------------------------------

class PkgJSONObject {
private:
    
public:

};

// ---------------------------------------------------------------------

ExportLibCliWebJS::ExportLibCliWebJS() {
    m_sPackageName = "";
    m_sAppName = "";
    m_sPackageName = "libwjscpp-web-js";
    m_sLibraryName = "wjscpp";
    m_sAppName = "wjscpp";
    m_sAppVersion = "0.0.0";
    m_sAuthor = "wjscpp";
    m_sPrefixRepositoryURL = "https://github.com/wjscpp/";
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::setPackageName(const std::string &sPackageName) {
    m_sPackageName = sPackageName;
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::setLibraryName(const std::string &sLibraryName) {
    m_sLibraryName = sLibraryName;
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::setAppName(const std::string &sAppName) {
    m_sAppName = sAppName;
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::setAppVersion(const std::string &sAppVersion) {
    m_sAppVersion = sAppVersion;
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::setAuthor(const std::string &sAuthor) {
    m_sAuthor = sAuthor;
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::setPrefixRepositoryURL(const std::string &sPrefixRepositoryURL) {
    m_sPrefixRepositoryURL = sPrefixRepositoryURL;
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::exportLib() {
    std::string sBasicDir = "./" + m_sPackageName;

    exportPrepareDirs(sBasicDir);
    exportLibCliWebJSFile(sBasicDir);
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
        if (!Fallen::dirExists(sDir)) {
            std::cout << " * mkdir " << sDir << std::endl
                << (Fallen::makeDir(sDir) ? "\t> OK" : "\t> FAILED") << std::endl;
        } else {
            std::cout << " * Directory already exists " << sDir << std::endl;
        }
    }
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::exportPackageJson(const std::string &sBasicDir) {
    std::ofstream packageJson;
    std::cout << " * write code to " << sBasicDir << "/package.json " << std::endl;
    packageJson.open (sBasicDir + "/package.json");

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

    // look here an example https://github.com/jquery/jquery/blob/master/package.json

    JSCodeBuilder builder;
    builder
    .sub("{")
        .add("\"name\": \"" + m_sPackageName + "\",")
        .add("\"version\": \"" + m_sAppVersion + "\",")
        .add("\"description\": \"FreeHackQuest JavaScript Web Client Library for " + m_sAppName + "\",")
        .add("\"main\": \"dist/" + m_sPackageName + ".js\",")
        .sub("\"repository\": {")
            .add("\"type\": \"git\",")
            .add("\"url\": \"" + m_sPrefixRepositoryURL + m_sPackageName + ".git\"")
            .end()
        .add("},")
        .sub("\"keywords\": [")
            .add("\"browser\",")
            .add("\"library\",")
            .add("\"client\",")
            .add("\"websocket\"")
            .end()
        .add("],")
        .sub("\"bugs\": {")
            .add("\"url\": \"" + m_sPrefixRepositoryURL + m_sPackageName + "/issues\"")
            .end()
        .add("},")
        .add("\"author\": \"" + m_sAuthor + "\",")
        .add("\"license\": \"MIT\",")
        .sub("\"licenses\": [{")
            .add("\"type\": \"MIT\",")
            .add("\"url\": \"" + m_sPrefixRepositoryURL + m_sPackageName + "/blob/master/LICENSE\"")
            .end()
        .add("}]")
        .end()
    .add("}");
    builder.print(packageJson);
    packageJson.close();
    std::cout << "\t> OK" << std::endl;

}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::exportAPImd(const std::string &sBasicDir) {
    
    std::ofstream apimd;
    std::cout << " * write file to " + sBasicDir + "/API.md" << std::endl;
    apimd.open(sBasicDir + "/API.md");

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

    // TODO redesign
    apimd << "# " + m_sPackageName + "\n\n";
    apimd << " Automatically generated by " << m_sAppName << ". \n";
    apimd << " * Version: " << m_sAppVersion << "\n";
    apimd << " * Date: " << buffer.str() << "\n\n";
    apimd << " Example connect/disconnect:\n"
        << "```\n"
        << "import libfhqcli \n\n"
        << "fhq = libfhqcli.FHQCli('ws://localhost:1234')\n"
        << " ... \n"
        << "fhq.close()\n"
        << "```\n";
    apimd << "\n";

    std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
    for (; it!=g_pCmdHandlers->end(); ++it) {
        std::string sCmd = it->first;
        CmdHandlerBase* pCmdHandlerBase = it->second;
        
        apimd << " ## " << sCmd << "\n\n";
        if (pCmdHandlerBase->description() != "") {
            apimd << pCmdHandlerBase->description() << "\n\n";
        }
        apimd 
            << "Access: unauthorized - **" << (pCmdHandlerBase->accessUnauthorized() ? "yes" : "no") << "**, "
            << " user - **" << (pCmdHandlerBase->accessUser() ? "yes" : "no") << "**, "
            << " admin - **" << (pCmdHandlerBase->accessAdmin() ? "yes" : "no") << "**\n"
            << "\n";

        apimd << " #### Input params \n\n";

        std::string pythonTemplate = "";

        std::vector<CmdInputDef> vVin = pCmdHandlerBase->inputs();
        for (int i = 0; i < vVin.size(); i++) {
            CmdInputDef inDef = vVin[i];
            std::string nameIn = std::string(inDef.getName());

            apimd << " * " << inDef.getName() << " - " << inDef.getType() << ", " << inDef.getRestrict() << "; " << inDef.getDescription() << "\n";

            if (pythonTemplate != "") {
                pythonTemplate += ", ";
            }
            if (inDef.isInteger()) {
                int nVal = 0;
                if (inDef.getName() == "onpage") {
                    nVal = 10;
                }
                pythonTemplate += "\"" + inDef.getName() + "\": " + std::to_string(nVal);
            } else {
                pythonTemplate += "\"" + inDef.getName() + "\": \"\"";
            }
        }
        apimd << "\n\n";

        apimd << " #### example call method \n\n ```response = " + m_sLibraryName + "." + sCmd + "({" + pythonTemplate + "});```";

        apimd << "\n\n";

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
    sample_html
        << "<html>\r\n"
        << "<head>\r\n"
        << "    <script>\r\n"
        << "        function log(n) {\r\n"
        << "            console.log(n);\r\n"
        << "            var log = document.getElementById('log');\r\n"
        << "            log.innerHTML += '[' + new Date() + '] ' + n + '\\r\\n';\r\n"
        << "        };\r\n"
        << "        window." << m_sLibraryName << " = window." << m_sLibraryName << " || {};\r\n"
        << "        window.fhq.ws = window.fhq.ws || {};\r\n"
        << "        window.fhq.showNotification = function(type, section, message) {\r\n"
        << "            // handle\r\n"
        << "        }\r\n"
        << "        window.fhq.onConnectionState = function(s) {\r\n"
        << "            log('onConnectionState: ' + s)\r\n"
        << "        }\r\n"
        << "        window.fhq.onGotUserData = function() {\r\n"
        << "            log('onGotUserData: ' + JSON.stringify(fhq.userinfo))\r\n"
        << "            log('onGotUserData: ' + JSON.stringify(fhq.userProfile))\r\n"
        << "        }\r\n"
        << "        window.fhq.onConnectionClose = function() {\r\n"
        << "            // handle\r\n"
        << "        }\r\n"
        << "        wjscpp.ws.base_url = 'ws://' + window.location.hostname + ':1234/api-ws/';\r\n"
        << "    </script>\r\n"
        << "    <script src='dist/" + m_sPackageName + ".js'></script>\r\n"
        << "</head>\r\n"
        << "<body>\r\n"
        << "<div>\r\n"
        << "    <h1>Login</h1>\r\n"
        << "    <input type='text' id='login'><br>\r\n"
        << "    <input type='password' id='password'><br>\r\n"
        << "    <button id='btn_login'>Login</button>\r\n"
        << "</div>\r\n"
        << "<script>\r\n"
        << "    btn_login.onclick = function() {\r\n"
        << "        fhq.login({email: login.value, password: password.value}).done(function(r) { \r\n"
        << "            log('Login success, token = ' + r.token);\r\n"
        << "        }).fail(function(e) {\r\n"
        << "            log('Login failed, error = ' + e.error);\r\n"
        << "        })\r\n"
        << "    }\r\n"
        << "</script>\r\n"
        << "<pre id='log'></pre>\r\n"
        << "</body>\r\n"
        << "</html>\r\n";
    sample_html.close();
    std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

void ExportLibCliWebJS::exportLibCliWebJSFile(const std::string &sBasicDir) {

    std::string sFilename = sBasicDir + "/dist/" + m_sPackageName + ".js";
    std::ofstream libwjscppcli_web_js_file;
    std::cout << " * write code to " << sFilename << std::endl;
    libwjscppcli_web_js_file.open(sFilename);

    // TODO move to fallen
    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");
    // now the result is in `buffer.str()`.

    libwjscppcli_web_js_file <<
        "// This file was automatically generated by " << m_sAppName << " \r\n"
        "// Version: " << m_sAppVersion << "\r\n"
        "// Date: " << buffer.str() << " \r\n"
        "\r\n"
        "window." << m_sLibraryName << " = window." << m_sLibraryName << " || (function(){ \r\n"
        "   var self = this;\r\n"
        "   var _lastm = 0;\r\n"
        "   var _listeners = {};\r\n"
        "   var _connectionState = '?';\r\n"
        "   function _lm(){ _lastm++; return 'm' + _lastm; };\r\n"
        "   console.warn('" << m_sLibraryName << " (v" << m_sAppVersion << ")');\r\n"
        "   self.promise = function() {\r\n"
        "       return {\r\n"
        "            completed: false, failed: false, successed: false, \r\n"
        "            done: function(callback) {\r\n"
        "                this.done_callback = callback;\r\n"
        "                if (this.completed && typeof this.done_callback === 'function' && this.successed) {\r\n"
        "                    this.done_callback.apply(this, this.result_arguments);\r\n"
        "                }\r\n"
        "                return this;\r\n"
        "            },\r\n"
        "            fail: function(callback) {\r\n"
        "                this.fail_callback = callback;\r\n"
        "                if (this.completed && typeof this.fail_callback === 'function' && this.failed) {\r\n"
        "                    this.fail_callback.apply(this, this.error_arguments);\r\n"
        "                }\r\n"
        "                return this;\r\n"
        "            },\r\n"
        "            resolve: function() {\r\n"
        "                if (!this.completed) {\r\n"
        "                    this.result_arguments = arguments; // [];\r\n"
        "                    if (typeof this.done_callback === 'function') {\r\n"
        "                        this.done_callback.apply(this, this.result_arguments);\r\n"
        "                    }\r\n"
        "                }\r\n"
        "                this.successed = true;\r\n"
        "                this.completed = true;\r\n"
        "            },\r\n"
        "            reject: function() {\r\n"
        "                if (!this.completed) {\r\n"
        "                    this.error_arguments = arguments;\r\n"
        "                    if (typeof this.fail_callback === 'function') {\r\n"
        "                        this.fail_callback.apply(this, this.error_arguments);\r\n"
        "                    }\r\n"
        "                }\r\n"
        "                this.failed = true;\r\n"
        "                this.completed = true;\r\n"
        "            }\r\n"
        "        }; // end of promise\r\n"
        "    };\r\n"
        "\r\n"
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
        "        self.setTokenToCookie = function(token) {\r\n"
        "            var date = new Date( new Date().getTime() + (7 * 24 * 60 * 60 * 1000) ); // cookie on week\r\n"
        "            document.cookie = 'fhqtoken=' + encodeURIComponent(token) + '; path=/; expires='+date.toUTCString();\r\n"
        "        }\r\n"
        "\r\n"
        "        self.removeTokenFromCookie = function() {\r\n"
        "            document.cookie = 'fhqtoken=; path=/;';\r\n"
        "        }\r\n"
        "\r\n"
        "        self.getTokenFromCookie = function() {\r\n"
        "            var matches = document.cookie.match(new RegExp(\r\n"
        "                '(?:^|; )' + 'fhqtoken'.replace(/([\\.$?*|{}\\(\\)\\[\\]\\\\\\/\\+^])/g, '\\\\$1') + '=([^;]*)'\r\n"
        "            ));\r\n"
        "            return matches ? decodeURIComponent(matches[1]) : '';\r\n"
        "        }\r\n"
        "        self.tokenValue = self.getTokenFromCookie();\r\n"
        "        self.handlerReceivedChatMessage = function(response) {\r\n"
        "            self.handlerReceivedChatMessage(response);\r\n"
        "        };\r\n"
        "        self.addListener = function(m, d) {\r\n"
        "            _listeners[m] = d;\r\n"
        "        }\r\n"
        "        return p;\r\n"
        "    };\r\n"
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
        "       } else if (response.cmd === 'server') {\r\n"
        "           console.warn('App: ' + response.app);\r\n"
        "           console.warn('Version: ' + response.version);\r\n"
        "           console.warn('All: ', response);\r\n"
        "       } else if (response.cmd === 'notify') {\r\n"
        "           if (self.showNotification) {\r\n"
        "               self.showNotification(response.type, response.section, response.message);\r\n"
        "           }\r\n"
        "       } else if (response.cmd === 'chat') {\r\n"
        "           self.handlerReceivedChatMessage(response);\r\n"
        "       } else {\r\n"
        "           console.error('Not found handler for [' + response.cmd + '/' + response.m + ']');\r\n"
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
        "   }\r\n"
        "   self.getConnectionState = function() {\r\n"
        "       return _connectionState;\r\n"
        "   }\r\n"
        "   \r\n"
        "   self._onConnectionState = function(s) {\r\n"
        "       _connectionState = s;\r\n"
        "       if (self.onConnectionState) {\r\n"
        "           self.onConnectionState(s);\r\n"
        "       }\r\n"
        "   }\r\n"
        "   self._onGotUserData = function() {\r\n"
        "       if (self.onGotUserData) {\r\n"
        "           self.onGotUserData();\r\n"
        "       }\r\n"
        "   }\r\n"
        "   self.onconnect = function() {\r\n"
        "       \r\n"
        "   };\r\n"
        "   self.init = function(baseUrl) {\r\n"
        "   \r\n"
        "       self.socket = new WebSocket(baseUrl);\r\n"
        "       self.socket.onopen = function() {\r\n"
        "           console.log('WS Opened');\r\n"
        "           setTimeout(self.onconnect,1);\r\n"
        "           self._onConnectionState('OK');\r\n"
        "           self.token();\r\n"
        "       };\r\n"
        "   \r\n"
        "       self.socket.onclose = function(event) {\r\n"
        "           console.log('Closed');\r\n"
        "           \r\n"
        "           if (self.onConnectionClose) {\r\n"
        "               self.onConnectionClose();\r\n"
        "           }\r\n"
        "           \r\n"
        "           if (event.wasClean) {\r\n"
        "               self._onConnectionState('CLOSED');\r\n"
        "           } else {\r\n"
        "               self._onConnectionState('BROKEN');\r\n"
        "               setTimeout(function() {\r\n"
        "                   self._onConnectionState('RECONN');\r\n"
        "                   self.init();\r\n"
        "               }, 10000);\r\n"
        "             // Try reconnect after 5 sec\r\n"
        "           }\r\n"
        "           console.log('Code: ' + event.code + ' Reason: ' + event.reason);\r\n"
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
        "   }\r\n"
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
        "               self._onGotUserData();\r\n"
        "           }).fail(function() {\r\n"
        "               self.cleanuptoken();\r\n"
        "               localStorage.removeItem('userinfo');\r\n"
        "               self._onGotUserData();\r\n"
        "           });\r\n"
        "       },10);\r\n"
        "   }\r\n";

    std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
    for (; it!=g_pCmdHandlers->end(); ++it) {
        std::string sCmd = it->first;
        CmdHandlerBase* pCmdHandlerBase = it->second;

        libwjscppcli_web_js_file
            << "// Access unauthorized: " << (pCmdHandlerBase->accessUnauthorized() ? "yes" : "no") << "\r\n"
            << "// Access user: " << (pCmdHandlerBase->accessUser() ? "yes" : "no") << "\r\n"
            << "// Access admin: " << (pCmdHandlerBase->accessAdmin() ? "yes" : "no") << "\r\n";
        
        if (pCmdHandlerBase->activatedFromVersion() != "") {
            libwjscppcli_web_js_file 
                << "// Activated From Version: " << pCmdHandlerBase->activatedFromVersion() << "\r\n";
        }
        
        if (pCmdHandlerBase->deprecatedFromVersion() != "") {
            libwjscppcli_web_js_file
                << "// Deprecated From Version: " + pCmdHandlerBase->deprecatedFromVersion() << "\r\n";
        }
        
        std::vector<CmdInputDef> vVin = pCmdHandlerBase->inputs();
        if (vVin.size() > 0) {
            libwjscppcli_web_js_file 
                <<  "// Input params:\r\n"; 
        }
        for (int i = 0; i < vVin.size(); i++) {
            CmdInputDef inDef = vVin[i];
            std::string nameIn = std::string(inDef.getName());
            libwjscppcli_web_js_file 
                <<  "// * " + nameIn + " - " + inDef.getType() + ", " + inDef.getRestrict() + " (" + inDef.getDescription() + ") \r\n";
        }
        libwjscppcli_web_js_file 
            << "self." << sCmd << " = function(params) {\r\n"
            << "    params = params || {};\r\n"
            << "    params.cmd = '" << sCmd << "';\r\n";
            // check required
        for (int i = 0; i < vVin.size(); i++) {
            CmdInputDef inDef = vVin[i];
            if (inDef.isRequired()) {
                std::string nameIn = std::string(vVin[i].getName());
                libwjscppcli_web_js_file 
                    << "    if (params['" + nameIn + "'] == undefined) {\r\n"
                    << "         console.error('Parameter \"" << nameIn << "\" expected (lib)');\r\n"
                    << "    }\r\n";
            }
        }
        if (sCmd == "login") {
            libwjscppcli_web_js_file
                << "    var ret = self.promise()\r\n"
                << "    self.send(params).done(function(r) {\r\n"
                << "        self.tokenValue = r.token;\r\n"
                << "        console.log(self.tokenValue);\r\n"
                << "        self.userinfo = r.user;\r\n"
                << "        localStorage.setItem('userinfo', JSON.stringify(self.userinfo));\r\n"
                << "        self.setTokenToCookie(self.tokenValue);\r\n"
                << "        self.updateUserProfileAsync();\r\n"
                << "        ret.resolve(r);\r\n"
                << "    }).fail(function(err) {\r\n"
                << "        self.cleanuptoken();\r\n"
                << "        localStorage.removeItem('userinfo');\r\n"
                << "        self.userinfo = {};\r\n"
                << "        ret.reject(err);\r\n"
                << "    })\r\n"
                << "    return ret;\r\n";
        } else if (sCmd == "token") {
            libwjscppcli_web_js_file
                << "    if (self.tokenValue && self.tokenValue != '') {\r\n"
                << "        var ret = self.promise()\r\n"
                << "        params.token = self.tokenValue;\r\n"
                << "        self.send(params).done(function(r) {\r\n"
                << "            self.updateUserProfileAsync();\r\n"
                << "            ret.resolve(r);\r\n"
                << "        }).fail(function(err) {\r\n"
                << "            self.api.cleanuptoken();\r\n"
                << "            self._onGotUserData();\r\n"
                << "            ret.reject(err);\r\n"
                << "        })\r\n"
                << "        return ret;\r\n"
                << "    } else {\r\n"
                << "        return self.send(params);\r\n"
                << "    }\r\n";
        } else {
            libwjscppcli_web_js_file 
                << "    return self.send(params);\r\n";
        }

        libwjscppcli_web_js_file 
            << "}\r\n\r\n";
    }

    libwjscppcli_web_js_file <<
        "   return self;\r\n"
        "})();\r\n";

    libwjscppcli_web_js_file.close();
    std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

} // namespace WsjCpp