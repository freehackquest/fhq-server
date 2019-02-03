#include <export_libfhqcli_web_js.h>

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <websocketserver.h>
#include <employees.h>
#include <QtCore> // TODO remove
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <iomanip> // put_time
#include <ctime>
#include <sstream>

// ---------------------------------------------------------------------

class JSCodeLine{
    
    JSCodeLine *m_pParent;
    std::string m_sLine;
    std::vector<JSCodeLine *> m_vLines;

public:
    
    JSCodeLine(){
        m_pParent = NULL;
        m_sLine = "";
    }

    JSCodeLine(JSCodeLine *parent, const std::string &sLine){
        m_pParent = parent;
        m_sLine = sLine;
    }

    ~JSCodeLine(){
        if(m_pParent == NULL){
            std::cout << "destruct root \n";
        }else{
            std::cout << "destruct something else [" << m_sLine << "]\n";
        }
    }

    JSCodeLine *addLine(const std::string &sLine){
        JSCodeLine *pJSCodeLine = new JSCodeLine(this,sLine);
        m_vLines.push_back(pJSCodeLine);
        return pJSCodeLine;
    }

    JSCodeLine *getParent(){
        return m_pParent;
    }

    std::string getLine(){
        return m_sLine;
    }

    JSCodeLine *findRoot(){
        if(m_pParent == NULL){
            return this;
        }
        return m_pParent->findRoot();
    }

    void print(std::ofstream &__init__, std::string intent = ""){
        if(m_pParent != NULL){
            __init__ << intent << m_sLine << std::endl;
            intent += "  ";
        }
        for(int i = 0; i < m_vLines.size(); i++){
            m_vLines[i]->print(__init__, intent);
        }
    }
};

// ---------------------------------------------------------------------

class JSCodeBuilder {

private:
    JSCodeLine *m_pCurr = NULL;

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
        if(p != NULL){
            m_pCurr = p;
        }else{
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

    ExportLibFHQCliWebJS::exportPrepareDirs();
    ExportLibFHQCliWebJS::export__init__py();
    ExportLibFHQCliWebJS::exportPackageJson();
    ExportLibFHQCliWebJS::exportAPImd();
}

// ---------------------------------------------------------------------

void ExportLibFHQCliWebJS::exportPrepareDirs(){
    int status;
    std::cout << " * mkdir libfhqcli-js" << std::endl;
    status = mkdir("libfhqcli-js", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    std::cout << "\t> OK" << std::endl;

    std::cout << " * mkdir libfhqcli-js/libfhqcli" << std::endl;
    status = mkdir("libfhqcli-js/libfhqcli", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

void ExportLibFHQCliWebJS::exportPackageJson(){
    std::ofstream packageJson;
    std::cout << " * write code to libfhqcli-js/package.json " << std::endl;
    packageJson.open ("libfhqcli-js/package.json");

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

    // look here an example https://github.com/jquery/jquery/blob/master/package.json

    JSCodeBuilder builder;
    builder
    .sub("{")
        .add("\"name\": \"libfhqcli-js\",")
        .add("\"version\": \"" + std::string(FHQSRV_VERSION) + "\",")
        .add("\"description\": \"FreeHackQuest JavaScript Client Library for fhq-server\",")
        .add("\"main\": \"dist/libfhqcli.js\",")
        .sub("\"repository\": {")
            .add("\"type\": \"git\",")
            .add("\"url\": \"https://github.com/freehackquest/libfhqcli-js.git\"")
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
            .add("\"url\": \"https://github.com/jquery/jquery/issues\"")
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

void ExportLibFHQCliWebJS::exportAPImd(){
    
    std::ofstream apimd;
    std::cout << " * write file to libfhqcli-js/API.md" << std::endl;
    apimd.open("libfhqcli-js/API.md");

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

void ExportLibFHQCliWebJS::export__init__py(){

    /*
if(!window.fhq) window.fhq = {};
if(!window.fhq.ws) window.fhq.ws = {};
fhq.ws.lastm = 0;

// WebSocket protocol

window.fhq.ws.handlerReceivedChatMessage = function(response){
	fhq.handlerReceivedChatMessage(response);
};
window.fhq.ws.listeners = {}
window.fhq.ws.addListener = function(m, d){
	fhq.ws.listeners[m] = d;
}

fhq.ws.handleCommand = function(response){
	if(fhq.ws.listeners[response.m]){
		if(response['error']){
			setTimeout(function(){
				fhq.ws.listeners[response.m].reject(response);
				delete fhq.ws.listeners[response.m];
			},1);
		} else {
			setTimeout(function(){
				fhq.ws.listeners[response.m].resolve(response);
				delete fhq.ws.listeners[response.m];
			},1);
		}
	}else if(response.cmd == "server"){
		console.warn("App: " + response.app);
		console.warn("Version: " + response.version);
		console.warn("All: ", response);
	}else if(response.cmd == "notify"){
        fhq.ui.showNotification(response.type, response.section, response.message);
	}else if(response.cmd == "chat"){
		fhq.ws.handlerReceivedChatMessage(response);
	}else{
		console.error("Not found handler for '" + response.cmd + "/" + response.m + "'");
	}
};

window.fhq.ws.WSState = "?";

window.fhq.ws.getWSState = function(){
	return fhq.ws.WSState;
}

window.fhq.ws.setWSState = function(s){
	fhq.ws.WSState = s;
	var el = document.getElementById('websocket_state');
	if(el){
		document.getElementById('websocket_state').innerHTML = s;
	}
}
window.fhq.ws.onconnect = function(){
	
};

window.fhq.ws.initWebsocket = function(){

	fhq.ws.socket = new WebSocket(fhq.ws.base_url);
	window.fhq.ws.socket.onopen = function() {
		console.log('WS Opened');
		setTimeout(window.fhq.ws.onconnect,1);
		fhq.ws.setWSState("OK");
		fhq.ws.token();
	};

	window.fhq.ws.socket.onclose = function(event) {
		console.log('Closed');
		
		if(fhq.ui && fhq.ui.onwsclose){
			fhq.ui.onwsclose();
		}
		
		if (event.wasClean) {
			fhq.ws.setWSState("CLOSED");
		} else {
			fhq.ws.setWSState("BROKEN");
			setTimeout(function(){
				fhq.ws.setWSState("RECONN");
				fhq.ws.initWebsocket();
			}, 10000);
		  // Try reconnect after 5 sec
		}
		console.log('Code: ' + event.code + ' Reason: ' + event.reason);
	};
	fhq.ws.socket.onmessage = function(event) {
		// console.log('Received: ' + event.data);
		try{
			var response = JSON.parse(event.data);
			fhq.ws.handleCommand(response);
		}catch(e){
			console.error(e);
		}
		
	};
	fhq.ws.socket.onerror = function(error) {
		console.log('Error: ' + error.message);
	};
}

fhq.ws.initWebsocket();

window.fhq.ws.send = function(obj, def){
	var d = def || $.Deferred();
	fhq.ws.lastm++;
	obj.m = "m" + fhq.ws.lastm;
	fhq.ws.listeners[obj.m] = d;
	try{
		if(fhq.ws.socket.readyState == 0){
			setTimeout(function(){
				fhq.ws.send(obj, d);
			},1000);
		}else{
			// console.log("ReadyState " + fhq.ws.socket.readyState);
			// console.log("Send " + JSON.stringify(obj));
			fhq.ws.socket.send(JSON.stringify(obj));
		}
	}catch(e){
		console.error(e);
	}
	return d;
}

*/

    std::ofstream __init__;
    std::cout << " * write code to libfhqcli-js/libfhqcli/__init__.py " << std::endl;
    __init__.open ("libfhqcli-js/libfhqcli/__init__.py");

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

    // now the result is in `buffer.str()`.

    JSCodeBuilder builder;
    builder
    .add("# -*- coding: utf-8 -*-")
    .add("### This file was automatically generated by fhq-server")
    .add("### Version: " + std::string(FHQSRV_VERSION))
    .add("### Date: " + buffer.str())
    .add("")
    .add("import json")
    .add("import functools")
    .add("import websocket")
    .add("import time")
    .sub("try:")
        .add("import Queue as queue")
        .end()
    .sub("except ImportError:")
        .add("import queue")
        .end()
    .add("import select")
    .add("from threading import Thread")
    .add("")
    .sub("class FHQCliRecvThread(Thread):")
        .add("__ws = None")
        .add("__cli_version = '" + std::string(FHQSRV_VERSION) + "'")
        .add("__sendCommandQueue = queue.Queue()")
        .add("__cmd_results = {}")
        .add("do_run = True")
        .add("__connecting = False")
        .add("__url = None")
        .add("__messageId = 0")
        .add("")
        .sub("def __init__(self, url):")
            .add("Thread.__init__(self)")
            .add("self.__url = url")
            .end()
        .add("")
        .sub("def run(self):")
            .add("self.__connecting = True")
            .add("print('[FHQCliThread] Connecting to ' + self.__url)")
            .sub("try:")
                .add("self.__ws = websocket.create_connection(self.__url)")
                .end()
            .sub("except:")
                .add("print('Failed connect to ' + self.__url)")
                .add("self.__ws = None")
                .add("self.__connecting = False")
                .add("return")
                .end()
            .add("# self.__ws.setblocking(0)")
            .add("self.__connecting = False")
            .add("print('[FHQCliThread] Connected')")
            .sub("while(self.do_run):")
                .sub("while not self.__sendCommandQueue.empty():")
                    .add("requestJson = self.__sendCommandQueue.get()")
                    .add("requestText = json.dumps(requestJson)")
                    .add("# print('requestText: ' + requestText)")
                    .add("self.__ws.send(requestText)")
                    .end()
                .add("ready = select.select([self.__ws], [], [], 0.1)")
                .sub("if ready[0]:")
                    .add("responseText =  self.__ws.recv()")
                    .add("responseJson = json.loads(responseText)")
                    .add("cmd_m = responseJson['m']")
                    .add("self.__cmd_results[cmd_m] = json.loads(responseText)")
                    .add("# print(responseText)")
                    .end()
                .add("time.sleep(0.5)")
                .end()
            .add("print('[FHQCliThread] Stopping...')")
            .add("self.__ws.close()")
            .add("self.__ws = None")
            .add("print('[FHQCliThread] Stopped')")
            .end()
        .add("")
        .sub("def addToSendQueue(self, requestJson):")
            .add("self.__sendCommandQueue.put(requestJson)")
            .add("return requestJson['m']")
            .end()
        .add("")
        .sub("def generateBaseCommand(self, cmd):")
            .add("self.__messageId = self.__messageId + 1")
            .add("requestJson = {}")
            .add("msgId = 'm' + str(self.__messageId)")
            .add("requestJson['m'] = msgId")
            .add("requestJson['cmd'] = cmd")
            .add("return requestJson")
            .end()
        .add("")
        .sub("def hasCmdResult(self, cmd_m):")
            .add("return cmd_m in self.__cmd_results")
            .end()
        .add("")
        .sub("def getCmdResult(self, cmd_m):")
            .add("return self.__cmd_results[cmd_m]")
            .end()
        .add("")
        .sub("def removeCmdResult(self, cmd_m):")
            .add("self.__cmd_results.pop(cmd_m, None)")
            .end()
        .add("")
        .sub("def hasConnection(self):")
            .add("return self.__connecting or self.__ws != None")
            .end()
        .end()
    .add("")
    .sub("class FHQCli:")
        .add("# __ws = None")
        .add("__url = 'ws://localhost:1234/'")
        .add("__recvThread = None")
        .sub("def __init__(self, url):")
            .add("self.__url = url")
            .add("self.connect()")
            .end()
        .add("")
        .sub("def __enter__(self):")
            .add("return self")
            .end()
        .add("")
        .sub("def __exit__(self, exc_type, exc_value, traceback):")
            .add("self.__recvThread.do_run = False")
            .end()
        .add("")
        .sub("def close(self):")
            .add("self.__recvThread.do_run = False")
            .end()
        .add("")
        .sub("def setUrl(self, url):")
            .add("self.__url = url")
            .end()
        .add("")
        .sub("def getUrl(self):")
            .add("return self.__url")
            .end()
        .add("")
        .sub("def connect(self):")
            .add("self.__recvThread = FHQCliRecvThread(self.__url)")
            .add("self.__recvThread.start()")
            .add("# self.__recvThread.join()")
            .add("# print('FHQCli: Thread started')")
            .add("return True")
            .end()
        .add("")
        .sub("def __looper(self, cmd_m):")
            .add("max_time = 5*10; # 5 seconds")
            .add("counter_time = 0")
            .add("cmd_result = {}")
            .sub("while True:")
                .add("counter_time = counter_time + 1")
                .sub("if not self.__recvThread.hasConnection():")
                    .add("return None")
                    .end()
                .sub("if self.__recvThread.hasCmdResult(cmd_m):")
                    .add("cmd_result = self.__recvThread.getCmdResult(cmd_m)")
                    .add("self.__recvThread.removeCmdResult(cmd_m)")
                    .add("return cmd_result")
                    .end()
                .sub("if counter_time > max_time:")
                    .add("cmd_result = None")
                    .add("break")
                    .end()
                .add("# print('__looper ....')")
                .add("time.sleep(0.2)")
                .end()
            .add("return cmd_result")
            .end()
        .add("");

    std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
    for (; it!=g_pCmdHandlers->end(); ++it){
        std::string sCmd = it->first;
        CmdHandlerBase* pCmdHandlerBase = it->second;
        builder
        .add("# Access unauthorized " + std::string(pCmdHandlerBase->access().accessUnauthorized() ? "yes" : "no"))
        .add("# Acess user " + std::string(pCmdHandlerBase->access().accessUser() ? "yes" : "no"))
        .add("# Access admin " + std::string(pCmdHandlerBase->access().accessAdmin() ? "yes" : "no"));
        
        if(pCmdHandlerBase->activatedFromVersion() != ""){
            builder.add("# Activated From Version: " + pCmdHandlerBase->activatedFromVersion());
        }
        
        if(pCmdHandlerBase->deprecatedFromVersion() != ""){
            builder.add("# Deprecated From Version: " + pCmdHandlerBase->deprecatedFromVersion());
        }
        
        std::vector<CmdInputDef> vVin = pCmdHandlerBase->inputs();
        for(int i = 0; i < vVin.size(); i++){
            CmdInputDef inDef = vVin[i];
            std::string nameIn = std::string(inDef.getName());
            builder.add("# " + nameIn + " - " + inDef.getType() + ", " + inDef.getRestrict() + " (" + inDef.getDescription() + ")" );
        }

        builder
        .sub("def " + sCmd + "(self, req):")
            .add("if not self.__recvThread.hasConnection(): return None")
            .add("requestJson = self.__recvThread.generateBaseCommand('" + sCmd + "')");

        // check required
        for(int i = 0; i < vVin.size(); i++){
            CmdInputDef inDef = vVin[i];
            if(inDef.isRequired()){
                std::string nameIn = std::string(vVin[i].getName());
                builder
                .sub("if '" + nameIn + "' not in req: ")
                    .add("raise Exception('Parameter \"" + nameIn + "\" expected (lib)')")
                    .end();
            }
        }

        for(int i = 0; i < vVin.size(); i++){
            std::string nameIn = std::string(vVin[i].getName());
            builder
            .sub("if '" + nameIn + "' in req: ")
                .add("requestJson['" + nameIn + "'] = req['" + nameIn + "']")
                .end();
        }

        builder
            .add("cmd_m = self.__recvThread.addToSendQueue(requestJson)")
            .add("return self.__looper(cmd_m)")
            .end()
        .add("");
    }
    builder.end();
    builder.print(__init__);
    __init__.close();
    std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

