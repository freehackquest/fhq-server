#include <export_libfhqcli_web_js.h>

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <websocketserver.h>
#include <employees.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <iomanip> // put_time
#include <ctime>
#include <sstream>
#include <fs.h>

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

    ~JSCodeLine(){
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
    JSCodeBuilder(){
        m_pCurr = new JSCodeLine();
    }

    ~JSCodeBuilder(){
        // std::cout << "destruct something else [" << m_pCurr->getLine() << "]\n";
    }

    JSCodeBuilder &add(const std::string &sLine){
        m_pCurr->addLine(sLine);
        return *this;
    }
    JSCodeBuilder &sub(const std::string &sLine){
        m_pCurr = m_pCurr->addLine(sLine);
        return *this;
    }
    JSCodeBuilder &end(){
        JSCodeLine *p = m_pCurr->getParent();
        if (p != nullptr) {
            m_pCurr = p;
        } else {
            std::cout << "Wrong called end function" << std::endl;
        }
        return *this;
    }

    void print(std::ofstream &__init__){
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

void ExportLibFHQCliWebJS::exportLib(){
    std::string sBasicDir = "./libfhqcli-web-js";

    ExportLibFHQCliWebJS::exportPrepareDirs(sBasicDir);
    ExportLibFHQCliWebJS::exportLibfhqcliWebJSFile(sBasicDir);
    ExportLibFHQCliWebJS::exportSampleHtmlFile(sBasicDir);
    ExportLibFHQCliWebJS::exportPackageJson(sBasicDir);
    ExportLibFHQCliWebJS::exportAPImd(sBasicDir);
}

// ---------------------------------------------------------------------

void ExportLibFHQCliWebJS::exportPrepareDirs(const std::string &sBasicDir) {
    std::vector<std::string> vDirs;
    vDirs.push_back(sBasicDir);
    vDirs.push_back(sBasicDir + "/dist");
    
    for (int i = 0; i < vDirs.size(); i++) {
        std::string sDir = vDirs[i];
        if (!FS::dirExists(sDir)) {
            std::cout << " * mkdir " << sDir << std::endl
                << (FS::makeDir(sDir) ? "\t> OK" : "\t> FAILED") << std::endl;
        } else {
            std::cout << " * Directory already exists " << sDir << std::endl;
        }
    }
}

// ---------------------------------------------------------------------

void ExportLibFHQCliWebJS::exportPackageJson(const std::string &sBasicDir) {
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
        .add("\"name\": \"libfhqcli-web-js\",")
        .add("\"version\": \"" + std::string(FHQSRV_VERSION) + "\",")
        .add("\"description\": \"FreeHackQuest JavaScript Web Client Library for fhq-server\",")
        .add("\"main\": \"dist/libfhqcli-web.js\",")
        .sub("\"repository\": {")
            .add("\"type\": \"git\",")
            .add("\"url\": \"https://github.com/freehackquest/libfhqcli-web-js.git\"")
            .end()
        .add("},")
        .sub("\"keywords\": [")
            .add("\"browser\",")
            .add("\"library\",")
            .add("\"fhq\",")
            .add("\"freehackquest\",")
            .add("\"websocket\"")
            .end()
        .add("],")
        .sub("\"bugs\": {")
            .add("\"url\": \"https://github.com/freehackquest/libfhqcli-web-js/issues\"")
            .end()
        .add("},")
        .add("\"author\": \"FreeHackQuest Team\",")
        .add("\"license\": \"MIT\",")
        .sub("\"licenses\": [{")
            .add("\"type\": \"MIT\",")
            .add("\"url\": \"https://github.com/freehackquest/libfhqcli-web-js/blob/master/LICENSE\"")
            .end()
        .add("}]")
        .end()
    .add("}");
    builder.print(packageJson);
    packageJson.close();
    std::cout << "\t> OK" << std::endl;

}

// ---------------------------------------------------------------------

void ExportLibFHQCliWebJS::exportAPImd(const std::string &sBasicDir){
    
    std::ofstream apimd;
    std::cout << " * write file to " + sBasicDir + "/API.md" << std::endl;
    apimd.open(sBasicDir + "/API.md");

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

    apimd << "# FHQCli\n\n";
    apimd << " Automatically generated by fhq-server. \n";
    apimd << " * Version: " << FHQSRV_VERSION << "\n";
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
    for (; it!=g_pCmdHandlers->end(); ++it){
        std::string sCmd = it->first;
        CmdHandlerBase* pCmdHandlerBase = it->second;
        
        apimd << " ## " << sCmd << "\n\n";
        if(pCmdHandlerBase->description() != ""){
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
        for(int i = 0; i < vVin.size(); i++){
            CmdInputDef inDef = vVin[i];
            std::string nameIn = std::string(inDef.getName());

            apimd << " * " << inDef.getName() << " - " << inDef.getType() << ", " << inDef.getRestrict() << "; " << inDef.getDescription() << "\n";

            if(pythonTemplate != ""){
                pythonTemplate += ", ";
            }
            if(inDef.isInteger()){
                int nVal = 0;
                if(inDef.getName() == "onpage"){
                    nVal = 10;
                }
                pythonTemplate += "\"" + inDef.getName() + "\": " + std::to_string(nVal);
            }else{
                pythonTemplate += "\"" + inDef.getName() + "\": \"\"";
            }
        }
        apimd << "\n\n";

        apimd << " #### example call method \n\n ```response = fhq." + sCmd + "({" + pythonTemplate + "});```";

        apimd << "\n\n";

    }

    apimd.close();
    std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

void ExportLibFHQCliWebJS::exportSampleHtmlFile(const std::string &sBasicDir) {
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
        << "            log.innerHTML += '[' + new Date() + '] ' + n + '\\r\\n';"
        << "        };\r\n"
        << "        window.fhq = window.fhq || {};\r\n"
        << "        window.fhq.ws = window.fhq.ws || {};\r\n"
        << "        window.fhq.ws.showNotification = function(type, section, message) {\r\n"
        << "            // handle\r\n"
        << "        }\r\n"
        << "        window.fhq.ws.onConnectionState = function(s) {\r\n"
        << "            log('onConnectionState: ' + s)\r\n"
        << "        }\r\n"
        << "        window.fhq.ws.onGotUserData = function() {\r\n"
        << "            log('onGotUserData: ' + JSON.stringify(fhq.userinfo))\r\n"
        << "            log('onGotUserData: ' + JSON.stringify(fhq.ws.userProfile))\r\n"
        << "        }\r\n"
        << "        window.fhq.ws.onConnectionClose = function() {\r\n"
        << "            // handle\r\n"
        << "        }\r\n"
        << "        fhq.ws.base_url = 'ws://' + window.location.hostname + ':1234/api-ws/';\r\n"
        << "    </script>\r\n"
        << "    <script src='dist/libfhqcli-web.js'></script>\r\n"
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
        << "        fhq.ws.login({email: login.value, password: password.value}).done(function(r){ \r\n"
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

void ExportLibFHQCliWebJS::exportLibfhqcliWebJSFile(const std::string &sBasicDir){

    std::string sFilename = sBasicDir + "/dist/libfhqcli-web.js";
    std::ofstream libfhqcli_web_js_file;
    std::cout << " * write code to " << sFilename << std::endl;
    libfhqcli_web_js_file.open(sFilename);

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");
    // now the result is in `buffer.str()`.

    libfhqcli_web_js_file
        << "// This file was automatically generated by fhq-server \r\n"
        << "// Version: " << std::string(FHQSRV_VERSION) << "\r\n"
        << "// Date: " << buffer.str() << " \r\n\r\n"

        << "if (!window.fhq) window.fhq = {};\r\n"
        << "if (!window.fhq.ws) window.fhq.ws = {};\r\n"
        << "fhq.ws.lastm = 0;\r\n"
        << "\r\n"
        << "// WebSocket protocol\r\n"
        << "fhq.ws.promise = function() {\r\n"
        << "    var d = {};\r\n"
        << "    d.completed = false;\r\n"
        << "    d.failed = false;\r\n"
        << "    d.successed = false;\r\n"
        << "    d.done = function(callback){\r\n"
        << "        d.done_callback = callback;\r\n"
        << "        if (d.completed && typeof d.done_callback === 'function' && d.successed) {\r\n"
        << "            d.done_callback.apply(this, d.result_arguments);\r\n"
        << "        }\r\n"
        << "        return d;\r\n"
        << "    }\r\n"
        << "    \r\n"
        << "    d.fail = function(callback) {\r\n"
        << "        d.fail_callback = callback;\r\n"
        << "        if (d.completed && typeof d.fail_callback === 'function' && d.failed) {\r\n"
        << "            d.fail_callback.apply(this,d.error_arguments);\r\n"
        << "        }\r\n"
        << "        return d;\r\n"
        << "    }\r\n"
        << "    \r\n"
        << "    d.resolve = function() {\r\n"
        << "        if (!d.completed) {\r\n"
        << "            d.result_arguments = arguments; // [];\r\n"
        << "            if (typeof d.done_callback === 'function') {\r\n"
        << "                d.done_callback.apply(this, d.result_arguments);\r\n"
        << "            }\r\n"
        << "        }\r\n"
        << "        d.successed = true;\r\n"
        << "        d.completed = true;\r\n"
        << "    }\r\n"
        << "    d.reject = function() {\r\n"
        << "        if (!d.completed) {\r\n"
        << "            d.error_arguments = arguments;\r\n"
        << "            if (typeof d.fail_callback === 'function') {\r\n"
        << "                d.fail_callback.apply(this, d.error_arguments);\r\n"
        << "            }\r\n"
        << "        }\r\n"
        << "        d.failed = true;\r\n"
        << "        d.completed = true;\r\n"
        << "    }\r\n"
        << "    return d;\r\n"
        << "};\r\n"
        << "\r\n"
        << "fhq.ws.waitAllPromises = function(arr_promise) {\r\n"
        << "    var p = fhq.ws.promise();\r\n"
        << "    var max_len = arr_promise.length;\r\n"
        << "    var result = [];\r\n"
        << "    function cmpl(r){\r\n"
        << "        result.push(r);\r\n"
        << "        if (result.length == max_len) {\r\n"
        << "            p.resolve(result);\r\n"
        << "        }\r\n"
        << "    };\r\n"
        << "    for (var i in arr_promise) {\r\n"
        << "        arr_promise[i].done(cmpl).fail(cmpl);\r\n"
        << "    }\r\n"
        << "    return p;\r\n"
        << "}"
        << "\r\n"
        << "window.fhq.ws.setTokenToCookie = function(token) {\r\n"
        << "    var date = new Date( new Date().getTime() + (7 * 24 * 60 * 60 * 1000) ); // cookie on week\r\n"
        << "    document.cookie = 'fhqtoken=' + encodeURIComponent(token) + '; path=/; expires='+date.toUTCString();\r\n"
        << "}\r\n"
        << "\r\n"
        << "window.fhq.ws.removeTokenFromCookie = function() {\r\n"
        << "    document.cookie = 'fhqtoken=; path=/;';\r\n"
        << "}\r\n"
        << "\r\n"
        << "window.fhq.ws.getTokenFromCookie = function() {\r\n"
        << "    var matches = document.cookie.match(new RegExp(\r\n"
        << "        '(?:^|; )' + 'fhqtoken'.replace(/([\\.$?*|{}\\(\\)\\[\\]\\\\\\/\\+^])/g, '\\\\$1') + '=([^;]*)'\r\n"
        << "    ));\r\n"
        << "    return matches ? decodeURIComponent(matches[1]) : '';\r\n"
        << "}\r\n"
        << "fhq.ws.tokenValue = fhq.ws.getTokenFromCookie();\r\n"
        << "window.fhq.ws.handlerReceivedChatMessage = function(response) {\r\n"
        << "	fhq.handlerReceivedChatMessage(response);\r\n"
        << "};\r\n"
        << "window.fhq.ws.listeners = {}\r\n"
        << "window.fhq.ws.addListener = function(m, d) {\r\n"
        << "	fhq.ws.listeners[m] = d;\r\n"
        << "}\r\n"
        << "\r\n"
        << "fhq.ws.handleCommand = function(response) {\r\n"
        << "	if (fhq.ws.listeners[response.m]) {\r\n"
        << "		if (response['error']) {\r\n"
        << "			setTimeout(function(){\r\n"
        << "				fhq.ws.listeners[response.m].reject(response);\r\n"
        << "				delete fhq.ws.listeners[response.m];\r\n"
        << "			},1);\r\n"
        << "		} else {\r\n"
        << "			setTimeout(function(){\r\n"
        << "				fhq.ws.listeners[response.m].resolve(response);\r\n"
        << "				delete fhq.ws.listeners[response.m];\r\n"
        << "			},1);\r\n"
        << "		}\r\n"
        << "	} else if (response.cmd === 'server') {\r\n"
        << "		console.warn('App: ' + response.app);\r\n"
        << "		console.warn('Version: ' + response.version);\r\n"
        << "		console.warn('All: ', response);\r\n"
        << "	} else if (response.cmd === 'notify') {\r\n"
        << "        if (fhq.ws.showNotification) {\r\n"
        << "            fhq.ws.showNotification(response.type, response.section, response.message);\r\n"
        << "        }\r\n"
        << "	} else if (response.cmd === 'chat') {\r\n"
        << "		fhq.ws.handlerReceivedChatMessage(response);\r\n"
        << "	} else {\r\n"
        << "		console.error('Not found handler for [' + response.cmd + '/' + response.m + ']');\r\n"
        << "	}\r\n"
        << "};\r\n"
        << "\r\n"
        << "window.fhq.ws._connectionState = '?';\r\n"
        << "\r\n"
        << "window.fhq.ws.getConnectionState = function() {\r\n"
        << "	return fhq.ws._connectionState;\r\n"
        << "}\r\n"
        << "\r\n"
        << "window.fhq.ws._onConnectionState = function(s) {\r\n"
        << "	fhq.ws._connectionState = s;\r\n"
        << "	if (window.fhq.ws.onConnectionState) {\r\n"
        << "		window.fhq.ws.onConnectionState(s);\r\n"
        << "	}\r\n"
        << "}\r\n"
        << "window.fhq.ws._onGotUserData = function() {\r\n"
        << "	if (window.fhq.ws.onGotUserData) {\r\n"
        << "		window.fhq.ws.onGotUserData();\r\n"
        << "	}\r\n"
        << "}\r\n"
        << "window.fhq.ws.onconnect = function() {\r\n"
        << "	\r\n"
        << "};\r\n"
        << "\r\n"
        << "window.fhq.ws.initWebsocket = function() {\r\n"
        << "\r\n"
        << "	fhq.ws.socket = new WebSocket(fhq.ws.base_url);\r\n"
        << "	window.fhq.ws.socket.onopen = function() {\r\n"
        << "		console.log('WS Opened');\r\n"
        << "		setTimeout(window.fhq.ws.onconnect,1);\r\n"
        << "		fhq.ws._onConnectionState('OK');\r\n"
        << "		fhq.ws.token();\r\n"
        << "	};\r\n"
        << "\r\n"
        << "	window.fhq.ws.socket.onclose = function(event) {\r\n"
        << "		console.log('Closed');\r\n"
        << "		\r\n"
        << "		if (fhq.ws.onConnectionClose){\r\n"
        << "			fhq.ws.onConnectionClose();\r\n"
        << "		}\r\n"
        << "		\r\n"
        << "		if (event.wasClean) {\r\n"
        << "			fhq.ws._onConnectionState('CLOSED');\r\n"
        << "		} else {\r\n"
        << "			fhq.ws._onConnectionState('BROKEN');\r\n"
        << "			setTimeout(function(){\r\n"
        << "				fhq.ws._onConnectionState('RECONN');\r\n"
        << "				fhq.ws.initWebsocket();\r\n"
        << "			}, 10000);\r\n"
        << "		  // Try reconnect after 5 sec\r\n"
        << "		}\r\n"
        << "		console.log('Code: ' + event.code + ' Reason: ' + event.reason);\r\n"
        << "	};\r\n"
        << "	fhq.ws.socket.onmessage = function(event) {\r\n"
        << "		// console.log('Received: ' + event.data);\r\n"
        << "		try {\r\n"
        << "			var response = JSON.parse(event.data);\r\n"
        << "			fhq.ws.handleCommand(response);\r\n"
        << "		} catch(e) {\r\n"
        << "			console.error(e);\r\n"
        << "		}\r\n"
        << "		\r\n"
        << "	};\r\n"
        << "	fhq.ws.socket.onerror = function(error) {\r\n"
        << "		console.log('Error: ' + error.message);\r\n"
        << "	};\r\n"
        << "}\r\n"
        << "\r\n"
        << "fhq.ws.initWebsocket();\r\n"
        << "\r\n"
        << "window.fhq.ws.send = function(obj, def) {\r\n"
        << "	var d = def || fhq.ws.promise();\r\n"
        << "	fhq.ws.lastm++;\r\n"
        << "	obj.m = 'm' + fhq.ws.lastm;\r\n"
        << "	fhq.ws.listeners[obj.m] = d;\r\n"
        << "	try {\r\n"
        << "		if (fhq.ws.socket.readyState == 0) {\r\n"
        << "			setTimeout(function(){\r\n"
        << "				fhq.ws.send(obj, d);\r\n"
        << "			},1000);\r\n"
        << "		} else {\r\n"
        << "			// console.log('ReadyState ' + fhq.ws.socket.readyState);\r\n"
        << "			// console.log('Send ' + JSON.stringify(obj));\r\n"
        << "			fhq.ws.socket.send(JSON.stringify(obj));\r\n"
        << "		}\r\n"
        << "	} catch(e) {\r\n"
        << "		console.error(e);\r\n"
        << "	}\r\n"
        << "	return d;\r\n"
        << "}\r\n"
        << "fhq.ws.userProfile = {bInitUserProfile: false}\r\n"
        << "fhq.ws.updateUserProfileAsync = function(){\r\n"
        << "    setTimeout(function(){\r\n"
        << "        fhq.ws.user().done(function(r){\r\n"
        << "            fhq.ws.userProfile.bInitUserProfile == true;\r\n"
        << "            fhq.ws.userProfile.university = r.data.university;\r\n"
        << "            fhq.ws.userProfile.country = r.data.country;\r\n"
        << "            fhq.ws.userProfile.city = r.data.city;\r\n"
        << "            fhq.userinfo = {};\r\n"
        << "            fhq.userinfo.id = r.data.id;\r\n"
        << "            fhq.userinfo.nick = r.data.nick;\r\n"
        << "            fhq.userinfo.email = r.data.email;\r\n"
        << "            fhq.userinfo.role = r.data.role;\r\n"
        << "            fhq.userinfo.logo = r.data.logo;\r\n"
        << "            fhq.ws._onGotUserData();\r\n"
        << "        }).fail(function(){\r\n"
        << "            fhq.ws.cleanuptoken();\r\n"
        << "            localStorage.removeItem('userinfo');\r\n"
        << "            fhq.ws._onGotUserData();\r\n"
        << "        });\r\n"
        << "    },10);\r\n"
        << "}\r\n"
        << "\r\n";

    std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
    for (; it!=g_pCmdHandlers->end(); ++it){
        std::string sCmd = it->first;
        CmdHandlerBase* pCmdHandlerBase = it->second;

        libfhqcli_web_js_file
            << "// Access unauthorized: " << (pCmdHandlerBase->accessUnauthorized() ? "yes" : "no") << "\r\n"
            << "// Access user: " << (pCmdHandlerBase->accessUser() ? "yes" : "no") << "\r\n"
            << "// Access admin: " << (pCmdHandlerBase->accessAdmin() ? "yes" : "no") << "\r\n";
        
        if(pCmdHandlerBase->activatedFromVersion() != ""){
            libfhqcli_web_js_file 
                << "// Activated From Version: " << pCmdHandlerBase->activatedFromVersion() << "\r\n";
        }
        
        if(pCmdHandlerBase->deprecatedFromVersion() != ""){
            libfhqcli_web_js_file
                << "// Deprecated From Version: " + pCmdHandlerBase->deprecatedFromVersion() << "\r\n";
        }
        
        std::vector<CmdInputDef> vVin = pCmdHandlerBase->inputs();
        if (vVin.size() > 0) {
            libfhqcli_web_js_file 
                <<  "// Input params:\r\n"; 
        }
        for(int i = 0; i < vVin.size(); i++){
            CmdInputDef inDef = vVin[i];
            std::string nameIn = std::string(inDef.getName());
            libfhqcli_web_js_file 
                <<  "// * " + nameIn + " - " + inDef.getType() + ", " + inDef.getRestrict() + " (" + inDef.getDescription() + ") \r\n";
        }
        libfhqcli_web_js_file 
            << "fhq.ws." << sCmd << " = function(params) {\r\n"
            << "    params = params || {};\r\n"
            << "    params.cmd = '" << sCmd << "';\r\n";
            // check required
        for (int i = 0; i < vVin.size(); i++) {
            CmdInputDef inDef = vVin[i];
            if(inDef.isRequired()){
                std::string nameIn = std::string(vVin[i].getName());
                libfhqcli_web_js_file 
                    << "    if (!params['" + nameIn + "']) {\r\n"
                    << "         console.error('Parameter \"" << nameIn << "\" expected (lib)');\r\n"
                    << "    }\r\n";
            }
        }
        if (sCmd == "login") {
            libfhqcli_web_js_file
                << "    var ret = fhq.ws.promise()\r\n"
                << "    fhq.ws.send(params).done(function(r){\r\n"
                << "        fhq.ws.tokenValue = r.token;\r\n"
                << "        console.log(fhq.ws.tokenValue);\r\n"
                << "        fhq.userinfo = r.user;\r\n"
                << "        localStorage.setItem('userinfo', JSON.stringify(fhq.userinfo));\r\n"
                << "        fhq.ws.setTokenToCookie(fhq.ws.tokenValue);\r\n"
                << "        fhq.ws.updateUserProfileAsync();\r\n"
                << "        ret.resolve(r);\r\n"
                << "    }).fail(function(err){\r\n"
                << "        fhq.ws.cleanuptoken();\r\n"
                << "        localStorage.removeItem('userinfo');\r\n"
                << "        fhq.userinfo = {};\r\n"
                << "        ret.reject(err);\r\n"
                << "    })\r\n"
                << "    return ret;\r\n";
        } else if (sCmd == "token") {
            libfhqcli_web_js_file
                << "    if (fhq.ws.tokenValue && fhq.ws.tokenValue != '') {\r\n"
                << "        var ret = fhq.ws.promise()\r\n"
                << "        params.token = fhq.ws.tokenValue;\r\n"
                << "        fhq.ws.send(params).done(function(r){\r\n"
                << "            fhq.ws.updateUserProfileAsync();\r\n"
                << "            ret.resolve(r);\r\n"
                << "        }).fail(function(err){\r\n"
                << "            fhq.api.cleanuptoken();\r\n"
                << "            fhq.ws._onGotUserData();\r\n"
                << "            ret.reject(err);\r\n"
                << "        })\r\n"
                << "        return ret;\r\n"
                << "    } else {\r\n"
                << "        return fhq.ws.send(params);"
                << "    }\r\n";
        } else {
            libfhqcli_web_js_file 
                << "    return fhq.ws.send(params);\r\n";
        }

        libfhqcli_web_js_file 
            << "}\r\n\r\n";
    }

    libfhqcli_web_js_file.close();
    std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

