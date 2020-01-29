#include <export_libwsjcppcli_py.h>

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <websocketserver.h>
#include <wsjcpp_employees.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <iomanip> // put_time
#include <ctime>
#include <sstream>

// ---------------------------------------------------------------------

class PyCodeLine{
    
    PyCodeLine *m_pParent;
    std::string m_sLine;
    std::vector<PyCodeLine *> m_vLines;
public:
    
    PyCodeLine() {
        m_pParent = NULL;
        m_sLine = "";
    }

    PyCodeLine(PyCodeLine *parent, const std::string &sLine) {
        m_pParent = parent;
        m_sLine = sLine;
    }

    ~PyCodeLine() {
        if (m_pParent == NULL) {
            std::cout << "destruct root \n";
        } else {
            std::cout << "destruct something else [" << m_sLine << "]\n";
        }
    }

    PyCodeLine *addLine(const std::string &sLine) {
        PyCodeLine *pPyCodeLine = new PyCodeLine(this,sLine);
        m_vLines.push_back(pPyCodeLine);
        return pPyCodeLine;
    }

    PyCodeLine *getParent() {
        return m_pParent;
    }

    std::string getLine() {
        return m_sLine;
    }

    PyCodeLine *findRoot() {
        if (m_pParent == NULL) {
            return this;
        }
        return m_pParent->findRoot();
    }

    void print(std::ofstream &__init__, std::string intent = "") {
        if (m_pParent != NULL) {
            __init__ << intent << m_sLine << std::endl;
            intent += "    ";
        }
        for (int i = 0; i < m_vLines.size(); i++) {
            m_vLines[i]->print(__init__, intent);
        }
    }
};

// ---------------------------------------------------------------------

class PyCodeBuilder {

private:
    PyCodeLine *m_pCurr = NULL;

public:
    PyCodeBuilder() {
        m_pCurr = new PyCodeLine();
    }

    ~PyCodeBuilder() {
        // std::cout << "destruct something else [" << m_pCurr->getLine() << "]\n";
    }

    PyCodeBuilder &add(const std::string &sLine) {
        m_pCurr->addLine(sLine);
        return *this;
    }
    PyCodeBuilder &add(const std::vector<std::string> &vLines) {
        for (int i = 0; i < vLines.size(); i++) {
            m_pCurr->addLine(vLines[i]);
        }
        return *this;
    }
    PyCodeBuilder &sub(const std::string &sLine) {
        m_pCurr = m_pCurr->addLine(sLine);
        return *this;
    }
    PyCodeBuilder &end() {
        PyCodeLine *p = m_pCurr->getParent();
        if (p != NULL) {
            m_pCurr = p;
        } else {
            std::cout << "Wrong called end function" << std::endl;
        }
        return *this;
    }

    void print(std::ofstream &__init__) {
        PyCodeLine *pRoot = m_pCurr->findRoot();
        pRoot->print(__init__);
    };
};

// ---------------------------------------------------------------------

void ExportLibWsjCppCliPy::exportLib() {

    exportPrepareDirs();
    export__init__py();
    exportSetupPy();
    exportAPImd();
}

// ---------------------------------------------------------------------

void ExportLibWsjCppCliPy::exportPrepareDirs() {
    int status;
    std::cout << " * mkdir libfreehackquest-client-py" << std::endl;
    status = mkdir("libfreehackquest-client-py", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    std::cout << "\t> OK" << std::endl;

    std::cout << " * mkdir libfreehackquest-client-py/libfreehackquestclient" << std::endl;
    status = mkdir("libfreehackquest-client-py/libfreehackquestclient", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

void ExportLibWsjCppCliPy::exportSetupPy() {
    std::ofstream setupPy;
    std::cout << " * write code to libfreehackquest-client-py/libfreehackquestclient/setup.py " << std::endl;
    setupPy.open ("libfreehackquest-client-py/setup.py");

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
        .add({
            "name='libfreehackquestclient',",
            "version='" + std::string(FHQSRV_VERSION) + "',",
            "install_requires=['websocket-client>=0.56.0', 'requests>=2.21.0'],",
            "keywords=['ctf', 'fhq', 'fhq-server', 'libfreehackquest-client', 'jeopardy', 'freehackquest'],",
            "author='FreeHackQuest Team',",
            "author_email='freehackquest@gmail.com',",
            "description='FreeHackQuest Python Client Library for fhq-server',",
            "long_description=long_description,",
            "long_description_content_type='text/markdown',",
            "url='https://github.com/freehackquest/libfreehackquest-client-py',",
            "packages=['libfreehackquestclient'],",
        })
        .sub("classifiers=(")
            .add("'Programming Language :: Python :: 3',")
            .add("'License :: OSI Approved :: Apache Software License',")
            .add("'Operating System :: OS Independent',")
            .end()
        .add("),")
        .add("python_requires='>=3.6',")
        .end()
    .add(")");
    builder.print(setupPy);
    setupPy.close();
    std::cout << "\t> OK" << std::endl;

}

// ---------------------------------------------------------------------

void ExportLibWsjCppCliPy::exportAPImd() {
    
    std::ofstream apimd;
    std::cout << " * write file to libfreehackquest-client-py/API.md" << std::endl;
    apimd.open("libfreehackquest-client-py/API.md");

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

    apimd << "# FreeHackQuestClient Python\n\n";
    apimd << " Automatically generated by fhq-server. \n";
    apimd << " * Version: " << FHQSRV_VERSION << "\n";
    apimd << " * Date: " << buffer.str() << "\n\n";
    apimd << " Example connect/disconnect:\n"
        << "```\n"
        << "from libfreehackquestclient import FreeHackQuestClient \n\n"
        << "cli = libfreehackquestclient.FreeHackQuestClient('ws://localhost:1234')\n"
        << " ... \n"
        << "cli.close()\n"
        << "```\n";
    apimd << "\n";

/*
<details>
  <summary>addhint</summary>
  
  Methid add hint to quest

Access: unauthorized - **no**,  user - **no**,  admin - **yes**

 #### Input params 

 * questid - integer, required; quest id
 * hint - string, required; hint text


 #### example call method 

 ```response = fhq.addhint({"questid": 0, "hint": ""})```
 
</details>

*/

    std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
    for (; it!=g_pCmdHandlers->end(); ++it) {
        std::string sCmd = it->first;
        CmdHandlerBase* pCmdHandlerBase = it->second;

        apimd
            << "<details>\n"
            << "<summary>" << sCmd << "</summary>\n\n"
            << "## " << sCmd << "\n\n";

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
                pythonTemplate += ",\n";
            }
            if (inDef.isInteger()) {
                int nVal = 0;
                if (inDef.getName() == "onpage") {
                    nVal = 10;
                }
                pythonTemplate += "    \"" + inDef.getName() + "\": " + std::to_string(nVal);
            } else {
                pythonTemplate += "    \"" + inDef.getName() + "\": \"\"";
            }
        }
        apimd 
            << "\n\n"
            << " #### example call method \n\n ```\nresponse = fhq." + sCmd + "({\n" + pythonTemplate + "\n})\n```"
            << "\n\n</details>"
            << "\n\n";
        
    }

    apimd.close();
    std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

void ExportLibWsjCppCliPy::export__init__py() {
    std::ofstream __init__;
    std::cout << " * write code to libfreehackquest-client-py/libfreehackquestclient/__init__.py " << std::endl;
    __init__.open ("libfreehackquest-client-py/libfreehackquestclient/__init__.py");

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

    // now the result is in `buffer.str()`.

    PyCodeBuilder builder;
    builder
    .add("#!/usr/bin/env python3")
    .add("# -*- coding: utf-8 -*-")
    .add("### This file was automatically generated by fhq-server")
    .add("### Version: " + std::string(FHQSRV_VERSION))
    .add("### Date: " + buffer.str())
    .add("")
    .add("import asyncio")
    .add("import websocket")
    .add("import json")
    .add("import select")
    .add("import time")
    .add("")
    .sub("class FreeHackQuestClient:")
        .add("__ws = None")
        .add("__url = None")
        .add("__cli_version = '" + std::string(FHQSRV_VERSION) + "'")
        .add("__loop = None")
        .add("__connecting = False")
        .add("__messageIdCounter = 0")
        .add("__incomingMesssages = []")
        .add("")
        .sub("def __init__(self, url):")
            .add("self.__url = url")
            .add("self.__loop = asyncio.get_event_loop()")
            .sub("try:")
                .add("self.__ws = websocket.create_connection(self.__url)")
                .add("self.__connecting = True")
                .end()
            .sub("except Exception as e:")
                .add("print('Failed connect to ' + self.__url)")
                .add("print('Exception: ' + str(e))")
                .add("self.__ws = None")
                .add("self.__connecting = False")
                .add("return")
                .end()
            .end()
        .add("")
        .sub("def __enter__(self):")
            .add("return self")
            .end()
        .add("")
        .sub("def __exit__(self, exc_type, exc_value, traceback):")
            .add("self.close()")
            .end()
        .add("")
        .sub("def hasConnection(self):")
            .add("return self.__ws != None")
            .end()
        .add("")
        .sub("def close(self):")
            .sub("if self.__ws != None:")
                .add("self.__ws.close()")
                .add("self.__ws = None")
                .end()
            // .add("self.__loop.close()")
            .end()
        .add("")
        .sub("def receiveIncomingMesssages(self):")
            .sub("if self.__ws == None:")
                .add("return None # TODO has not connection")
                .end()
            .sub("while True:")
                .sub("if self.__ws == None:")
                    .add("return None # TODO has not connection")
                    .end()
                .add("ready = select.select([self.__ws], [], [], 1)")
                .sub("if ready[0]:")
                    .add("responseText =  self.__ws.recv()")
                    .add("responseJson = json.loads(responseText)")
                    .add("# print('responseText: ' + responseText)")
                    .add("# incomeMessageId = responseJson['m']")
                    .add("self.__incomingMesssages.append(responseJson)")
                    .end()
                .sub("else:")
                    .add("return")
                    .end()
                .add("time.sleep(0.2)")
                .end()
            .end()
        .add("")
        .sub("def getIncomingMesssages(self):")
            .add("return self.__incomingMesssages")
            .end()
        .add("")
        .sub("def cleanIncomingMesssages(self):")
            .add("self.__incomingMesssages = []")
            .end()
        .add("")
        .add("@asyncio.coroutine")
        .sub("async def __looper(self, messageId):")
            .add("max_time = 5*10 # 5 seconds")
            .add("counter_time = 0")
            .sub("while True:")
                .sub("if self.__ws == None:")
                    .add("return None # TODO has not connection")
                    .end()
                .sub("for inmsg in self.__incomingMesssages:")
                    .sub("if inmsg['m'] == messageId:")
                        .add("responseJson = inmsg")
                        .add("self.__incomingMesssages.remove(responseJson)")
                        .add("return responseJson")
                        .end()
                    .end()
                .add("counter_time = counter_time + 1")
                .add("ready = select.select([self.__ws], [], [], 0.1)")
                .sub("if ready[0]:")
                    .add("responseText =  self.__ws.recv()")
                    .add("responseJson = json.loads(responseText)")
                    .add("# print('responseText: ' + responseText)")
                    .sub("if 'm' in responseJson:")
                        .add("incomeMessageId = responseJson['m']")
                        .sub("if incomeMessageId == messageId:")
                            .add("return responseJson")
                            .end()
                        .sub("else:")
                            .add("self.__incomingMesssages.append(responseJson)")
                            .end()
                        .end()
                    .sub("else:")
                        .add("print('What is it? ' + responseText)")
                        .end()
                    .end()
                .sub("else:")
                    .add("print('wait...')")
                    .end()
                .sub("if counter_time > max_time:")
                    .add("return None")
                    .end()
                .add("await asyncio.sleep(0.2)")
                .end()
            .end()
        .add("")
        .sub("def generateBaseCommand(self, method):")
            .add("requestJson = {}")
            .add("self.__messageIdCounter = self.__messageIdCounter + 1")
            .add("msgId = 'm' + str(self.__messageIdCounter)")
            .add("requestJson['m'] = msgId")
            .add("requestJson['cmd'] = method")
            .add("requestJson['jsonrpc'] = '2.0'")
            .add("requestJson['method'] = method")
            .add("return requestJson")
            .end()
        .add("")

        .sub("def __sendCommand(self, req):")
            .add("requestText = json.dumps(req)")
            .add("messageId = req['m']")
            .add("# print('requestText: ' + requestText)")
            .add("self.__ws.send(requestText) ")
            .add("result = self.__loop.run_until_complete(asyncio.gather(self.__looper(messageId)))")
            .add("return result[0]")
            .end()
        .add("");

    std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
    for (; it!=g_pCmdHandlers->end(); ++it) {
        std::string sCmd = it->first;
        CmdHandlerBase* pCmdHandlerBase = it->second;

        builder
        .sub("def " + sCmd + "(self, req):")
            .sub("\"\"\"" + pCmdHandlerBase->description())
                .add("");
        
        if (pCmdHandlerBase->activatedFromVersion() != "") {
            builder.add("Activated From Version: " + pCmdHandlerBase->activatedFromVersion());
        }
        
        if (pCmdHandlerBase->deprecatedFromVersion() != "") {
            builder.add("Deprecated From Version: " + pCmdHandlerBase->deprecatedFromVersion());
        }


        builder
            .add(pCmdHandlerBase->accessUnauthorized() ? "Allowed access for unauthorized users" : "Denied access for unauthorized users")
            .add(pCmdHandlerBase->accessUser() ? "Allowed access for users" : "Denied access for users")
            .add(pCmdHandlerBase->accessUser() ? "Allowed access for admins" : "Denied access for admins")
            .add("");

        builder
            .sub("Args:");
        
        std::vector<CmdInputDef> vVin = pCmdHandlerBase->inputs();
        for (int i = 0; i < vVin.size(); i++) {
            CmdInputDef inDef = vVin[i];
            std::string nameIn = std::string(inDef.getName());
            builder.add(nameIn + " (" + inDef.getType() + "): " + inDef.getRestrict() + ", " + inDef.getDescription());
        }
        builder.end();
        /* 
        Returns:
            bool: The return value. True for success, False otherwise.
        */

        builder
                .end()
            .add("\"\"\"")
            .add("if not self.hasConnection(): return None")
            .add("requestJson = self.generateBaseCommand('" + sCmd + "')");
           
        // check required
        std::string sAllowedInputParamNames = "";
        for (int i = 0; i < vVin.size(); i++) {
            if (sAllowedInputParamNames.length() > 0) {
                sAllowedInputParamNames += ",";
            }
            sAllowedInputParamNames += "'" + vVin[i].getName() + "'";
        }
        builder
            .add("allowedParams = [" + sAllowedInputParamNames + "]")
            .sub("for paramName in req:")
                .sub("if paramName not in allowedParams:")
                    .add("raise Exception('Excess parameter \"' + paramName + '\" (lib)')")
                    .end()
                .end();
        for (int i = 0; i < vVin.size(); i++) {
            CmdInputDef inDef = vVin[i];
            if (inDef.isRequired()) {
                std::string nameIn = std::string(vVin[i].getName());
                builder
                .sub("if '" + nameIn + "' not in req: ")
                    .add("raise Exception('Parameter \"" + nameIn + "\" expected (lib)')")
                    .end();
            }
        }

        for (int i = 0; i < vVin.size(); i++) {
            std::string nameIn = std::string(vVin[i].getName());
            builder
            .sub("if '" + nameIn + "' in req: ")
                .add("requestJson['" + nameIn + "'] = req['" + nameIn + "']")
                .end();
        }

        builder
            .add("return self.__sendCommand(requestJson)")
            .end()
        .add("");
    }
    builder.end();
    builder.print(__init__);
    __init__.close();
    std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

