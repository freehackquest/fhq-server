#include <utils_export_client_library_python.h>

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

// ---------------------------------------------------------------------

class PyCodeLine{
    
    PyCodeLine *m_pParent;
    std::string m_sLine;
    std::vector<PyCodeLine *> m_vLines;
public:
    
    PyCodeLine(){
        m_pParent = NULL;
        m_sLine = "";
    }

    PyCodeLine(PyCodeLine *parent, const std::string &sLine){
        m_pParent = parent;
        m_sLine = sLine;
    }

    ~PyCodeLine(){
        if(m_pParent == NULL){
            std::cout << "destruct root \n";
        }else{
            std::cout << "destruct something else [" << m_sLine << "]\n";
        }
    }

    PyCodeLine *addLine(const std::string &sLine){
        PyCodeLine *pPyCodeLine = new PyCodeLine(this,sLine);
        m_vLines.push_back(pPyCodeLine);
        return pPyCodeLine;
    }

    PyCodeLine *getParent(){
        return m_pParent;
    }

    std::string getLine(){
        return m_sLine;
    }

    PyCodeLine *findRoot(){
        if(m_pParent == NULL){
            return this;
        }
        return m_pParent->findRoot();
    }

    void print(std::ofstream &__init__, std::string intent = ""){
        if(m_pParent != NULL){
            __init__ << intent << m_sLine << std::endl;
            intent += "    ";
        }
        for(int i = 0; i < m_vLines.size(); i++){
            m_vLines[i]->print(__init__, intent);
        }
    }
};

// ---------------------------------------------------------------------

class PyCodeBuilder {

private:
    PyCodeLine *m_pCurr = NULL;

public:
    PyCodeBuilder(){
        m_pCurr = new PyCodeLine();
    }

    ~PyCodeBuilder(){
        // std::cout << "destruct something else [" << m_pCurr->getLine() << "]\n";
    }

    PyCodeBuilder &add(const std::string &sLine){
        m_pCurr->addLine(sLine);
        return *this;
    }
    PyCodeBuilder &sub(const std::string &sLine){
        m_pCurr = m_pCurr->addLine(sLine);
        return *this;
    }
    PyCodeBuilder &end(){
        PyCodeLine *p = m_pCurr->getParent();
        if(p != NULL){
            m_pCurr = p;
        }else{
            std::cout << "Wrong called end function" << std::endl;
        }
        return *this;
    }

    void print(std::ofstream &__init__){
        PyCodeLine *pRoot = m_pCurr->findRoot();
        pRoot->print(__init__);
    };
};

// ---------------------------------------------------------------------

void UtilsExportClientLibraryPython::exportLib(){

    UtilsExportClientLibraryPython::exportPrepareDirs();
    UtilsExportClientLibraryPython::export__init__py();
    UtilsExportClientLibraryPython::exportSetupPy();
    UtilsExportClientLibraryPython::exportAPImd();
}

// ---------------------------------------------------------------------

void UtilsExportClientLibraryPython::exportPrepareDirs(){
    int status;
    std::cout << " * mkdir libfhqcli-py" << std::endl;
    status = mkdir("libfhqcli-py", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    std::cout << "\t> OK" << std::endl;

    std::cout << " * mkdir libfhqcli-py/libfhqcli" << std::endl;
    status = mkdir("libfhqcli-py/libfhqcli", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

void UtilsExportClientLibraryPython::exportSetupPy(){
    std::ofstream setupPy;
    std::cout << " * write code to libfhqcli-py/libfhqcli/setup.py " << std::endl;
    setupPy.open ("libfhqcli-py/setup.py");

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

    PyCodeBuilder builder;
    builder
    .add("import setuptools")
    .add("")
    .sub("with open('README.md', 'r') as fh:")
        .add("long_description = fh.read()")
        .end()
    .add("")
    .sub("setuptools.setup(")
        .add("name='libfhqcli',")
        .add("version='" + QCoreApplication::applicationVersion().toStdString() + "',")
        .add("author='FreeHackQuest Team',")
        .add("author_email='freehackquest@gmail.com',")
        .add("description='FreeHackQuest Python Client Library for fhq-server',")
        .add("long_description=long_description,")
        .add("long_description_content_type='text/markdown',")
        .add("url='https://github.com/freehackquest/libfhqcli-py',")
        .add("packages=setuptools.find_packages(),")
        .sub("classifiers=(")
            .add("'Programming Language :: Python :: 2',")
            .add("'License :: OSI Approved :: Apache Software License',")
            .add("'Operating System :: OS Independent',")
            .end()
        .add(")")
        .end()
    .add(")");
    builder.print(setupPy);
    setupPy.close();
    std::cout << "\t> OK" << std::endl;

}

// ---------------------------------------------------------------------

void UtilsExportClientLibraryPython::exportAPImd(){
    
    std::ofstream apimd;
    std::cout << " * write file to libfhqcli-py/API.md" << std::endl;
    apimd.open("libfhqcli-py/API.md");

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

    apimd << "# FHQCli\n\n";
    apimd << " Automatically generated by fhq-server. \n";
    apimd << " * Version: " << QCoreApplication::applicationVersion().toStdString() << "\n";
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

void UtilsExportClientLibraryPython::export__init__py(){
    std::ofstream __init__;
    std::cout << " * write code to libfhqcli-py/libfhqcli/__init__.py " << std::endl;
    __init__.open ("libfhqcli-py/libfhqcli/__init__.py");

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

    // now the result is in `buffer.str()`.

    PyCodeBuilder builder;
    builder
    .add("# -*- coding: utf-8 -*-")
    .add("### This file was automatically generated by fhq-server")
    .add("### Version: " + QCoreApplication::applicationVersion().toStdString())
    .add("### Date: " + buffer.str())
    .add("")
    .add("import json")
    .add("import functools")
    .add("import websocket")
    .add("import time")
    .add("import Queue")
    .add("import select")
    .add("from threading import Thread")
    .add("")
    .sub("class FHQCliRecvThread(Thread):")
        .add("__ws = None;")
        .add("__sendCommandQueue = Queue.Queue();")
        .add("__cmd_results = {};")
        .add("do_run = True;")
        .add("__connecting = False;")
        .add("__url = None;")
        .add("__messageId = 0;")
        .add("")
        .sub("def __init__(self, url):")
            .add("Thread.__init__(self)")
            .add("self.__url = url;")
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
            .add("print('[FHQCliThread] Connected');")
            .sub("while(self.do_run):")
                .sub("while not self.__sendCommandQueue.empty():")
                    .add("requestJson = self.__sendCommandQueue.get();")
                    .add("requestText = json.dumps(requestJson);")
                    .add("# print('requestText: ' + requestText);")
                    .add("self.__ws.send(requestText)")
                    .end()
                .add("ready = select.select([self.__ws], [], [], 0.1)")
                .sub("if ready[0]:")
                    .add("responseText =  self.__ws.recv()")
                    .add("responseJson = json.loads(responseText)")
                    .add("cmd_m = responseJson['m'];")
                    .add("self.__cmd_results[cmd_m] = json.loads(responseText);")
                    .add("# print(responseText)")
                    .end()
                .add("time.sleep(0.5)")
                .end()
            .add("print('[FHQCliThread] Stopping...');")
            .add("self.__ws.close()")
            .add("self.__ws = None")
            .add("print('[FHQCliThread] Stopped');")
            .end()
        .add("")
        .sub("def addToSendQueue(self, requestJson):")
            .add("self.__sendCommandQueue.put(requestJson);")
            .add("return requestJson['m'];")
            .end()
        .add("")
        .sub("def generateBaseCommand(self, cmd):")
            .add("self.__messageId = self.__messageId + 1;")
            .add("requestJson = {};")
            .add("msgId = 'm' + str(self.__messageId);")
            .add("requestJson['m'] = msgId;")
            .add("requestJson['cmd'] = cmd;")
            .add("return requestJson;")
            .end()
        .add("")
        .sub("def hasCmdResult(self, cmd_m):")
            .add("return cmd_m in self.__cmd_results;")
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
        .add("# __ws = None;")
        .add("__url = 'ws://localhost:1234/';")
        .add("__recvThread = None")
        .sub("def __init__(self, url):")
            .add("self.__url = url")
            .add("self.connect();")
            .end()
        .add("")
        .sub("def __enter__(self):")
            .add("return self")
            .end()
        .add("")
        .sub("def __exit__(self, exc_type, exc_value, traceback):")
            .add("self.__recvThread.do_run = False;")
            .end()
        .add("")
        .sub("def close(self):")
            .add("self.__recvThread.do_run = False;")
            .end()
        .add("")
        .sub("def setUrl(self, url):")
            .add("self.__url = url;")
            .end()
        .add("")
        .sub("def getUrl(self):")
            .add("return self.__url;")
            .end()
        .add("")
        .sub("def connect(self):")
            .add("self.__recvThread = FHQCliRecvThread(self.__url)")
            .add("self.__recvThread.start()")
            .add("# self.__recvThread.join()")
            .add("# print('FHQCli: Thread started');")
            .add("return True;")
            .end()
        .add("")
        .sub("def __looper(self, cmd_m):")
            .add("max_time = 5*10; # 5 seconds")
            .add("counter_time = 0;")
            .add("cmd_result = {};")
            .sub("while True:")
                .add("counter_time = counter_time + 1;")
                .sub("if not self.__recvThread.hasConnection():")
                    .add("return None")
                    .end()
                .sub("if self.__recvThread.hasCmdResult(cmd_m):")
                    .add("cmd_result = self.__recvThread.getCmdResult(cmd_m);")
                    .add("self.__recvThread.removeCmdResult(cmd_m);")
                    .add("return cmd_result")
                    .end()
                .sub("if counter_time > max_time:")
                    .add("cmd_result = None")
                    .add("break;")
                    .end()
                .add("# print('__looper ....')")
                .add("time.sleep(0.2)")
                .end()
            .add("return cmd_result;")
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

        for(int i = 0; i < vVin.size(); i++){
            std::string nameIn = std::string(vVin[i].getName());
            builder
            .sub("if '" + nameIn + "' in req: ")
                .add("requestJson['" + nameIn + "'] = req['" + nameIn + "']")
                .end();
        }

        builder
            .add("cmd_m = self.__recvThread.addToSendQueue(requestJson);")
            .add("return self.__looper(cmd_m);")
            .end()
        .add("");
    }
    builder.end();
    builder.print(__init__);
    __init__.close();
    std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

