#include <export_libwsjcppcli_py.h>

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
    PyCodeBuilder &rmSub() {
        PyCodeLine *p = m_pCurr;
        m_pCurr = m_pCurr->getParent();
        delete p;
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

ExportLibWsjCppCliPy::ExportLibWsjCppCliPy() {
    TAG = "ExportLibWsjCppCliPy";
}

void ExportLibWsjCppCliPy::exportLib() {
    // prepare groups
    m_vGroups.clear();
    std::vector<std::string> vStringGroups;
    std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
    for (; it!=g_pCmdHandlers->end(); ++it) {
        std::string sCmd = it->first;
        // CmdHandlerBase* pCmdHandlerBase = it->second;
        if (sCmd.find(".") != std::string::npos) {
            std::string sGroup = WsjcppCore::split(sCmd, ".")[0];
            if (std::find(vStringGroups.begin(), vStringGroups.end(), sGroup) == vStringGroups.end()) {
                vStringGroups.push_back(sGroup);
                m_vGroups.push_back(ApiGroup(
                    sGroup,
                    convertCammelCaseToSnakCase(sGroup, false)
                ));
                std::cout << "   - Found api-group: " << sGroup << std::endl;
                // builder.add("from libfreehackquestclient.FreeHackQuestApi"  + sGroup + " import FreeHackQuestApi" + sGroup);
            }
        }
    }

    exportPrepareDirs();
    exportApiGroupsPy();
    export__init__py();
    exportClientPy();
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
            "version='" + std::string(WSJCPP_APP_VERSION) + "',",
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
    apimd << " * Version: " << WSJCPP_APP_VERSION << "\n";
    apimd << " * Date: " << buffer.str() << "\n\n";
    apimd << " Example connect/disconnect:\n"
        << "```\n"
        << "from libfreehackquestclient import FreeHackQuestClient \n\n"
        << "cli = FreeHackQuestClient('ws://localhost:1234')\n"
        << " ... \n"
        << "cli.close()\n"
        << "```\n";
    apimd << "\n";

    std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
    for (; it!=g_pCmdHandlers->end(); ++it) {
        std::string sCmd = it->first;
        CmdHandlerBase* pCmdHandlerBase = it->second;

        apimd
            << "<details>\n"
            << "<summary>" << sCmd << " - " << pCmdHandlerBase->description() << "</summary>\n\n"
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
            << " #### example call method \n\n ```\nresponse = cli." + sCmd + "({\n" + pythonTemplate + "\n})\n```"
            << "\n\n</details>"
            << "\n\n";
        
    }

    apimd.close();
    std::cout << "\t> OK" << std::endl;
}


// ---------------------------------------------------------------------

// TODO move to core like convertCammelCaseToSnakCase
std::string ExportLibWsjCppCliPy::convertCammelCaseToSnakCase(const std::string &sName, bool bSilent) {
    std::string sRet = ""; 
    for (int i = 0; i < sName.size(); i++) {
        char c = sName[i];
        if (i == 0 && c >= '0' && c <= '9') {
            if (!bSilent) {
                WsjcppLog::warn(TAG, std::string("First symbol could not be number"));
            }
        } else if (i == 0 && c >= 'a' && c <= 'z') {
            sRet += char(c - 32); // capitalize first char
        } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            sRet += c;
        } else {
            if (!bSilent) {
                WsjcppLog::warn(TAG, std::string("Ignored symbol in Name '") + c + "'");
            }
        }
    }
    return sRet;
}

// ---------------------------------------------------------------------

void ExportLibWsjCppCliPy::export__init__py() {
    std::ofstream __init__;
    std::cout << " * write code to libfreehackquest-client-py/libfreehackquestclient/__init__.py " << std::endl;
    __init__.open ("libfreehackquest-client-py/libfreehackquestclient/__init__.py");

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

    PyCodeBuilder builder;
    builder
    .add("#!/usr/bin/env python3")
    .add("# -*- coding: utf-8 -*-")
    .add("# Copyright (c) 2020 FreeHackQuest Team <freehackquest@gmail.com>")
    .add("\"\"\"This file was automatically generated by fhq-server")
    .add("Version: " + std::string(WSJCPP_APP_VERSION))
    .add("Date: " + buffer.str())
    .add("\"\"\"")
    .add("from libfreehackquestclient.freehackquest_client import FreeHackQuestClient")
    ;

    builder.print(__init__);
    __init__.close();
    std::cout << "\t> OK" << std::endl;

}

// ---------------------------------------------------------------------

void ExportLibWsjCppCliPy::exportApiGroupsPy() {
    for (int i = 0; i < m_vGroups.size(); i++) {
        std::ofstream apiGroup;
        std::string sFilename = m_vGroups[i].sFilename;
        std::string sClassname = m_vGroups[i].sClassname;
        std::cout << " * write code to libfreehackquest-client-py/libfreehackquestclient/" << sFilename << ".py " << std::endl;
        apiGroup.open("libfreehackquest-client-py/libfreehackquestclient/" + sFilename + ".py");

        std::time_t t = std::time(nullptr);
        std::stringstream buffer;
        buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

        PyCodeBuilder builder;
        builder
        .add("#!/usr/bin/env python3")
        .add("# -*- coding: utf-8 -*-")
        .add("# Copyright (c) 2020 FreeHackQuest Team <freehackquest@gmail.com>")
        .add("\"\"\"This file was automatically generated by fhq-server")
        .add("Version: " + std::string(WSJCPP_APP_VERSION))
        .add("Date: " + buffer.str())
        .add("\"\"\"")
        .add("")
        .sub("class " + sClassname + ":")
            .add("\"\"\" API Group " + m_vGroups[i].sGroupName + "\"\"\"")
            .add("__client = None")
            .sub("def __init__(self, client):")
                .add("self.__client = client")
                .end()
            .end()
        .end()
        .sub("")
        ;

        std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
        for (; it!=g_pCmdHandlers->end(); ++it) {
            std::string sCmd = it->first;
            CmdHandlerBase* pCmdHandlerBase = it->second;
            if (sCmd.find(".") == std::string::npos) {
                continue; // skip: no api-group
            }

            std::string sGroup = WsjcppCore::split(sCmd, ".")[0];
            if (sGroup != m_vGroups[i].sGroupName) {
                continue; // skip: no match api-group 
            }
            std::string sMethodName = WsjcppCore::split(sCmd, ".")[1];
            builder
            .sub("def " + sMethodName + "(self, req):")
                .sub("\"\"\"" + pCmdHandlerBase->description());
            
            if (pCmdHandlerBase->activatedFromVersion() != "") {
                builder.add("Activated From Version: " + pCmdHandlerBase->activatedFromVersion());
            }
            
            if (pCmdHandlerBase->deprecatedFromVersion() != "") {
                builder.add("Deprecated From Version: " + pCmdHandlerBase->deprecatedFromVersion());
            }


            builder
                .sub("Permissins:")
                    .add(pCmdHandlerBase->accessUnauthorized() ? "Allowed access for unauthorized users" : "Denied access for unauthorized users")
                    .add(pCmdHandlerBase->accessUser() ? "Allowed access for users" : "Denied access for users")
                    .add(pCmdHandlerBase->accessUser() ? "Allowed access for admins" : "Denied access for admins")
                    .end();

            builder
                .sub("Args:");
            
            std::vector<CmdInputDef> vVin = pCmdHandlerBase->inputs();
            for (int i = 0; i < vVin.size(); i++) {
                CmdInputDef inDef = vVin[i];
                std::string nameIn = std::string(inDef.getName());
                builder
                    .sub(nameIn + " (" + inDef.getType() + "," + inDef.getRestrict() +"):")
                        .add(inDef.getDescription())
                        .end();
            }
            builder.end();
            /* 
            Returns:
                bool: The return value. True for success, False otherwise.
            */

            builder
                    .end()
                .add("\"\"\"")
                .sub("if not self.__client.has_connection():")
                    .add("return None")
                    .end()
                .add("request_json = self.__client.generate_base_command('" + sCmd + "')");
            
            // check required
            int nRequired = 0;
            if (vVin.size() > 0) {
                builder.sub("allowed_params = [");
                for (int i = 0; i < vVin.size(); i++) {
                    builder.add("'" + vVin[i].getName() + "',");
                    if (vVin[i].isRequired()) {
                        nRequired++;
                    }
                }
                builder.end().add("]");
            } else {
                builder.add("allowed_params = []");
            }
            builder.add("self.__client.check_on_excess_params(req, '" + sCmd + "', allowed_params)");

            // required params
            if (nRequired > 0) {
                builder.sub("required_params = [");
                for (int i = 0; i < vVin.size(); i++) {
                    CmdInputDef inDef = vVin[i];
                    if (inDef.isRequired()) {
                        builder.add("'" + vVin[i].getName() + "',");
                    }
                }
                builder.end().add("]");
                builder.add("self.__client.check_on_required_params(req, '" + sCmd + "', required_params)");
            }

            // vRequired
            builder
                .sub("for param_name in required_params:")
                    .sub("if param_name not in req:")
                        .add("raise Exception('Parameter \"' + param_name + '\" expected (lib)')")
                        .end()
                    .end();

            for (int i = 0; i < vVin.size(); i++) {
                std::string nameIn = std::string(vVin[i].getName());
                builder
                .sub("if '" + nameIn + "' in req:")
                    .add("request_json['" + nameIn + "'] = req['" + nameIn + "']")
                    .end();
            }

            builder
                .add("return self.__client.send_command(request_json)")
                .end()
            .end()
            .sub("");
        }

        builder
            .rmSub()
            .end();

        builder.print(apiGroup);
        apiGroup.close();
        std::cout << "\t> OK" << std::endl;
    }
}

// ---------------------------------------------------------------------

void ExportLibWsjCppCliPy::exportClientPy() {
    std::ofstream freeHackQuestClientPy;
    std::cout << " * write code to libfreehackquest-client-py/libfreehackquestclient/freehackquest_client.py " << std::endl;
    freeHackQuestClientPy.open ("libfreehackquest-client-py/libfreehackquestclient/freehackquest_client.py");

    std::time_t t = std::time(nullptr);
    std::stringstream buffer;
    buffer << std::put_time(std::gmtime(&t), "%d %b %Y");

    // now the result is in `buffer.str()`.

    PyCodeBuilder builder;
    builder
    .add("#!/usr/bin/env python3")
    .add("# -*- coding: utf-8 -*-")
    .add("\"\"\"This file was automatically generated by fhq-server")
    .add("Version: " + std::string(WSJCPP_APP_VERSION))
    .add("Date: " + buffer.str())
    .add("\"\"\"")
    .add("import asyncio")
    .add("import time")
    .add("import select")
    .add("import json")
    .add("import traceback")
    .add("import websocket");

    for (int i = 0; i < m_vGroups.size(); i++) {
        builder.add("from libfreehackquestclient."  + m_vGroups[i].sFilename + " import " + m_vGroups[i].sClassname);
    }

    builder
    .add("")
    .sub("class FreeHackQuestClient:")
        .add("\"\"\"Client Api for fhq-server\"\"\"")
        .add("__ws = None")
        .add("__url = None")
        .add("__cli_version = '" + std::string(WSJCPP_APP_VERSION) + "'")
        .add("__loop = None")
        .add("__connecting = False")
        .add("__message_id_counter = 0")
        .add("__incoming_messsages = []")
        .end().sub("")
        .sub("def __init__(self, url):")
            .add("self.__url = url");

        for (int i = 0; i < m_vGroups.size(); i++) {
            builder.add("self." + m_vGroups[i].sGroupName + " = " + m_vGroups[i].sClassname + "(self)");
        }

        builder
            .add("self.__loop = asyncio.get_event_loop()")
            .sub("try:")
                .add("self.__ws = websocket.create_connection(self.__url)")
                .add("self.__connecting = True")
                .end()
            .sub("except Exception as err:")
                .add("print('Failed connect to ' + self.__url)")
                .add("print('Error: ' + str(err))")
                .add("traceback.print_exc()")
                .add("self.__ws = None")
                .add("self.__connecting = False")
                .add("return")
                .end()
            .end()
        .end().sub("")
        .sub("def __enter__(self):")
            .add("return self")
            .end()
        .end().sub("")
        .sub("def __exit__(self, exc_type, exc_value, exc_tracebalc):")
            .add("self.close()")
            .end()
        .end().sub("")
        .sub("def has_connection(self):")
            .add("\"\"\"Check the exists connection\"\"\"")
            .add("return self.__ws is not None")
            .end()
        .end().sub("")
        .sub("def close(self):")
            .add("\"\"\"Close connection\"\"\"")
            .sub("if self.__ws is not None:")
                .add("self.__ws.close()")
                .add("self.__ws = None")
                .end()
            // .add("self.__loop.close()")
            .end()
        .end().sub("")
        .sub("def receive_incoming_messsages(self):")
            .add("\"\"\"Recieve incoming messages and handle it\"\"\"")
            .sub("if self.__ws is None:")
                .add("return")
                .end()
            .sub("while True:")
                .sub("if self.__ws is None:")
                    .add("return")
                    .end()
                .add("ready = select.select([self.__ws], [], [], 1)")
                .sub("if ready[0]:")
                    .add("response_text = self.__ws.recv()")
                    .add("response_json = json.loads(response_text)")
                    .add("# print('response_text: ' + response_text)")
                    .add("# incomeMessageId = response_json['m']")
                    .add("self.__incoming_messsages.append(response_json)")
                    .end()
                .sub("else:")
                    .add("return")
                    .end()
                .add("time.sleep(0.2)")
                .end()
            .end()
        .end().sub("")
        .sub("def get_incoming_messsages(self):")
            .add("\"\"\"Return incoming messages list (for example: notifications)\"\"\"")
            .add("return self.__incoming_messsages")
            .end()
        .end().sub("")
        .sub("def clean_incoming_messsages(self):")
            .add("\"\"\"Clear list of incoming messages (like a notifications)\"\"\"")
            .add("self.__incoming_messsages = []")
            .end()
        .end().sub("")
        .add("@asyncio.coroutine")
        .sub("async def __looper(self, message_id):")
            .add("\"\"\"Infinty looper for handling income messages\"\"\"")
            .add("max_time = 5*10 # 5 seconds")
            .add("counter_time = 0")
            .sub("while True:")
                .sub("if self.__ws is None:")
                    .add("return None")
                    .end()
                .sub("for inmsg in self.__incoming_messsages:")
                    .sub("if inmsg['m'] == message_id:")
                        .add("response_json = inmsg")
                        .add("self.__incoming_messsages.remove(response_json)")
                        .add("return response_json")
                        .end()
                    .end()
                .add("counter_time = counter_time + 1")
                .add("ready = select.select([self.__ws], [], [], 0.1)")
                .sub("if ready[0]:")
                    .add("response_text = self.__ws.recv()")
                    .add("response_json = json.loads(response_text)")
                    .add("# print('response_text: ' + response_text)")
                    .sub("if 'm' in response_json:")
                        .add("income_message_id = response_json['m']")
                        .sub("if income_message_id == message_id:")
                            .add("return response_json")
                            .end()
                        .add("self.__incoming_messsages.append(response_json)")
                        .end()
                    .sub("else:")
                        .add("print('What is it? {' + response_text + '}')")
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
        .end().sub("")
        .sub("def generate_base_command(self, method):")
            .add("\"\"\"Prepare basic command\"\"\"")
            .add("request_json = {}")
            .add("self.__message_id_counter = self.__message_id_counter + 1")
            .add("msg_id = 'm' + str(self.__message_id_counter)")
            .add("request_json['m'] = msg_id")
            .add("request_json['cmd'] = method")
            .add("request_json['jsonrpc'] = '2.0'")
            .add("request_json['method'] = method")
            .add("return request_json")
            .end()
        .end().sub("")
        .sub("def send_command(self, req):")
            .add("\"\"\"Send request\"\"\"")
            .add("request_text = json.dumps(req)")
            .add("message_id = req['m']")
            .add("# print('request_text: ' + request_text)")
            .add("self.__ws.send(request_text)")
            .add("result = self.__loop.run_until_complete(asyncio.gather(self.__looper(message_id)))")
            .add("return result[0]")
            .end()
        .end().sub("")
        .sub("def check_on_excess_params(self, req, method_n, allowed_params):")
            .add("\"\"\"Check excess params\"\"\"")
            .sub("for p_name in req:")
                .sub("if p_name not in allowed_params:")
                    .add("raise Exception('Excess parameter \"' + p_name + '\" for \"' + method_n + '\" (lib)')")
                    .end()
                .end()
            .end()
        .end().sub("")
        .sub("def check_on_required_params(self, req, method_n, required_params):")
            .add("\"\"\"Check excess params\"\"\"")
            .sub("for p_name in required_params:")
                .sub("if p_name not in req:")
                    .add("raise Exception('Required parameter \"' + p_name + '\" for \"' + method_n + '\" (lib)')")
                    .end()
                .end()
            .end()
        .end().sub("");

    std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
    for (; it!=g_pCmdHandlers->end(); ++it) {
        std::string sCmd = it->first;
        CmdHandlerBase* pCmdHandlerBase = it->second;
        if (sCmd.find(".") != std::string::npos) {
            continue; // skip: has api-group
        }
        builder
        .sub("def " + sCmd + "(self, req):")
            .sub("\"\"\"" + pCmdHandlerBase->description());
        
        if (pCmdHandlerBase->activatedFromVersion() != "") {
            builder.add("Activated From Version: " + pCmdHandlerBase->activatedFromVersion());
        }
        
        if (pCmdHandlerBase->deprecatedFromVersion() != "") {
            builder.add("Deprecated From Version: " + pCmdHandlerBase->deprecatedFromVersion());
        }


        builder
            .sub("Permissins:")
                .add(pCmdHandlerBase->accessUnauthorized() ? "Allowed access for unauthorized users" : "Denied access for unauthorized users")
                .add(pCmdHandlerBase->accessUser() ? "Allowed access for users" : "Denied access for users")
                .add(pCmdHandlerBase->accessUser() ? "Allowed access for admins" : "Denied access for admins")
                .end();

        builder
            .sub("Args:");
        
        std::vector<CmdInputDef> vVin = pCmdHandlerBase->inputs();
        for (int i = 0; i < vVin.size(); i++) {
            CmdInputDef inDef = vVin[i];
            std::string nameIn = std::string(inDef.getName());
            builder
                .sub(nameIn + " (" + inDef.getType() + "," + inDef.getRestrict() +"):")
                    .add(inDef.getDescription())
                    .end();
        }
        builder.end();
        /* 
        Returns:
            bool: The return value. True for success, False otherwise.
        */

        builder
                .end()
            .add("\"\"\"")
            .sub("if not self.has_connection():")
                .add("return None")
                .end()
            .add("request_json = self.generate_base_command('" + sCmd + "')");
           
        // check required
        int nRequired = 0;
        if (vVin.size() > 0) {
            builder.sub("allowed_params = [");
            for (int i = 0; i < vVin.size(); i++) {
                builder.add("'" + vVin[i].getName() + "',");
                if (vVin[i].isRequired()) {
                    nRequired++;
                }
            }
            builder.end().add("]");
        } else {
            builder.add("allowed_params = []");
        }
        builder.add("self.check_on_excess_params(req, '" + sCmd + "', allowed_params)");

        // required params
        if (nRequired > 0) {
            builder.sub("required_params = [");
            for (int i = 0; i < vVin.size(); i++) {
                CmdInputDef inDef = vVin[i];
                if (inDef.isRequired()) {
                    builder.add("'" + vVin[i].getName() + "',");
                }
            }
            builder.end().add("]");
            builder.add("self.check_on_required_params(req, '" + sCmd + "', required_params)");
        }

        for (int i = 0; i < vVin.size(); i++) {
            std::string nameIn = std::string(vVin[i].getName());
            builder
            .sub("if '" + nameIn + "' in req:")
                .add("request_json['" + nameIn + "'] = req['" + nameIn + "']")
                .end();
        }

        builder
            .add("return self.send_command(request_json)")
            .end()
        .end()
        .sub("");
    }
    builder.rmSub();
    builder.print(freeHackQuestClientPy);
    freeHackQuestClientPy.close();
    std::cout << "\t> OK" << std::endl;
}

// ---------------------------------------------------------------------

