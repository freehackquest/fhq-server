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
    ExportLibFHQCliWebJS::exportPackageJson(sBasicDir);
    ExportLibFHQCliWebJS::exportAPImd(sBasicDir);
}

// ---------------------------------------------------------------------

void ExportLibFHQCliWebJS::exportPrepareDirs(const std::string &sBasicDir) {
    std::vector<std::string> vDirs;
    vDirs.push_back(sBasicDir);
    vDirs.push_back(sBasicDir + "/libfhqcli");
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
            .add("\"websocket\",")
            .end()
        .add("],")
        .sub("\"bugs\": {")
            .add("\"url\": \"https://github.com/freehackquest/libfhqcli-web-js/issues\"")
            .end()
        .add("},")
        .add("\"author\": \"FreeHackQuest Teams\",")
        .add("\"license\": \"MIT\"")
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
            << "Access: unauthorized - **" << (pCmdHandlerBase->access().accessUnauthorized() ? "yes" : "no") << "**, "
            << " user - **" << (pCmdHandlerBase->access().accessUser() ? "yes" : "no") << "**, "
            << " admin - **" << (pCmdHandlerBase->access().accessAdmin() ? "yes" : "no") << "**\n"
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
        << "\r\n"
        << "window.fhq.ws.handlerReceivedChatMessage = function(response){\r\n"
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
        << "	} else if(response.cmd === 'server') {\r\n"
        << "		console.warn('App: ' + response.app);\r\n"
        << "		console.warn('Version: ' + response.version);\r\n"
        << "		console.warn('All: ', response);\r\n"
        << "	} else if(response.cmd === 'notify') {\r\n"
        << "        fhq.ui.showNotification(response.type, response.section, response.message);\r\n"
        << "	} else if(response.cmd === 'chat') {\r\n"
        << "		fhq.ws.handlerReceivedChatMessage(response);\r\n"
        << "	} else {\r\n"
        << "		console.error('Not found handler for [' + response.cmd + '/' + response.m + ']');\r\n"
        << "	}\r\n"
        << "};\r\n"
        << "\r\n"
        << "window.fhq.ws.WSState = '?';\r\n"
        << "\r\n"
        << "window.fhq.ws.getWSState = function(){\r\n"
        << "	return fhq.ws.WSState;\r\n"
        << "}\r\n"
        << "\r\n"
        << "window.fhq.ws.setWSState = function(s){\r\n"
        << "	fhq.ws.WSState = s;\r\n"
        << "	var el = document.getElementById('websocket_state');\r\n"
        << "	if(el){\r\n"
        << "		document.getElementById('websocket_state').innerHTML = s;\r\n"
        << "	}\r\n"
        << "}\r\n"
        << "window.fhq.ws.onconnect = function(){\r\n"
        << "	\r\n"
        << "};\r\n"
        << "\r\n"
        << "window.fhq.ws.initWebsocket = function(){\r\n"
        << "\r\n"
        << "	fhq.ws.socket = new WebSocket(fhq.ws.base_url);\r\n"
        << "	window.fhq.ws.socket.onopen = function() {\r\n"
        << "		console.log('WS Opened');\r\n"
        << "		setTimeout(window.fhq.ws.onconnect,1);\r\n"
        << "		fhq.ws.setWSState('OK');\r\n"
        << "		fhq.ws.token();\r\n"
        << "	};\r\n"
        << "\r\n"
        << "	window.fhq.ws.socket.onclose = function(event) {\r\n"
        << "		console.log('Closed');\r\n"
        << "		\r\n"
        << "		if(fhq.ui && fhq.ui.onwsclose){\r\n"
        << "			fhq.ui.onwsclose();\r\n"
        << "		}\r\n"
        << "		\r\n"
        << "		if (event.wasClean) {\r\n"
        << "			fhq.ws.setWSState('CLOSED');\r\n"
        << "		} else {\r\n"
        << "			fhq.ws.setWSState('BROKEN');\r\n"
        << "			setTimeout(function(){\r\n"
        << "				fhq.ws.setWSState('RECONN');\r\n"
        << "				fhq.ws.initWebsocket();\r\n"
        << "			}, 10000);\r\n"
        << "		  // Try reconnect after 5 sec\r\n"
        << "		}\r\n"
        << "		console.log('Code: ' + event.code + ' Reason: ' + event.reason);\r\n"
        << "	};\r\n"
        << "	fhq.ws.socket.onmessage = function(event) {\r\n"
        << "		// console.log('Received: ' + event.data);\r\n"
        << "		try{\r\n"
        << "			var response = JSON.parse(event.data);\r\n"
        << "			fhq.ws.handleCommand(response);\r\n"
        << "		}catch(e){\r\n"
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
        << "window.fhq.ws.send = function(obj, def){\r\n"
        << "	var d = def || $.Deferred();\r\n"
        << "	fhq.ws.lastm++;\r\n"
        << "	obj.m = 'm' + fhq.ws.lastm;\r\n"
        << "	fhq.ws.listeners[obj.m] = d;\r\n"
        << "	try{\r\n"
        << "		if(fhq.ws.socket.readyState == 0){\r\n"
        << "			setTimeout(function(){\r\n"
        << "				fhq.ws.send(obj, d);\r\n"
        << "			},1000);\r\n"
        << "		}else{\r\n"
        << "			// console.log('ReadyState ' + fhq.ws.socket.readyState);\r\n"
        << "			// console.log('Send ' + JSON.stringify(obj));\r\n"
        << "			fhq.ws.socket.send(JSON.stringify(obj));\r\n"
        << "		}\r\n"
        << "	}catch(e){\r\n"
        << "		console.error(e);\r\n"
        << "	}\r\n"
        << "	return d;\r\n"
        << "}\r\n\r\n";


    std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
    for (; it!=g_pCmdHandlers->end(); ++it){
        std::string sCmd = it->first;
        CmdHandlerBase* pCmdHandlerBase = it->second;

        libfhqcli_web_js_file
            << "// Access unauthorized: " << (pCmdHandlerBase->access().accessUnauthorized() ? "yes" : "no") << "\r\n"
            << "// Access user: " << (pCmdHandlerBase->access().accessUser() ? "yes" : "no") << "\r\n"
            << "// Access admin: " << (pCmdHandlerBase->access().accessAdmin() ? "yes" : "no") << "\r\n";
        
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

        libfhqcli_web_js_file 
            << "    return fhq.ws.send(params);\r\n"
            << "}\r\n\r\n";
    }

    libfhqcli_web_js_file.close();
    std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

