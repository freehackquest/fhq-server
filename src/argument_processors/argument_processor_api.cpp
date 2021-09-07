
#include "argument_processor_api.h"
#include <wsjcpp_core.h>
#include <iomanip>
#include <algorithm>
#include <websocketserver.h>
#include <employees.h>

// ---------------------------------------------------------------------
// ArgumentProcessorApi

ArgumentProcessorApi::ArgumentProcessorApi() 
: WsjcppArgumentProcessor({"api"}, "api", "All what depened to api processing") {
    TAG = "ArgumentProcessorApi";
    // registrySingleArgument("--single", "What exactly do this single param?");
    // registryParameterArgument("-param", "N", "What need this param?");
    // registryExample("here example of command");
    registryProcessor(new ArgumentProcessorApiList());
}

// ---------------------------------------------------------------------

bool ArgumentProcessorApi::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

bool ArgumentProcessorApi::applyParameterArgument(
    const std::string &sProgramName, 
    const std::string &sArgumentName, 
    const std::string &sValue
) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

int ArgumentProcessorApi::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::err(TAG, "Not implemented");
    return -10;
}

// ---------------------------------------------------------------------
// ArgumentProcessorApiList

ArgumentProcessorApiList::ArgumentProcessorApiList() 
: WsjcppArgumentProcessor({"list", "ls"}, "Print list of api handlers", "Print list of api handlers") {
    TAG = "ArgumentProcessorApiList";
    // registrySingleArgument("--single", "What exactly do this single param?");
    // registryParameterArgument("-param", "N", "What need this param?");
    // registryExample("here example of command");
    // registryProcessor(new ArgumentProcessorOtherProcessor());
}

// ---------------------------------------------------------------------

bool ArgumentProcessorApiList::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

bool ArgumentProcessorApiList::applyParameterArgument(
    const std::string &sProgramName, 
    const std::string &sArgumentName, 
    const std::string &sValue
) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

int ArgumentProcessorApiList::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    std::cout << "\n\n * CmdHandlers (" << g_pCmdHandlers->size() << "):\n";

    struct CmdRow{
        std::string sCommandName;
        bool bAccessUnauth;
        bool bAccessUser;
        bool bAccessAdmin;
    };
    std::vector<CmdRow> vRows;
    int nMaxLenCommandName = 0;

    std::map<std::string, CmdHandlerBase*>::iterator it = g_pCmdHandlers->begin();
    for (; it!=g_pCmdHandlers->end(); ++it) {
        std::string sCmd = it->first;
        CmdHandlerBase* pCmdHandlerBase = it->second;
        CmdRow row;
        row.bAccessUnauth = pCmdHandlerBase->accessUnauthorized();
        row.bAccessUser = pCmdHandlerBase->accessUser();
        row.bAccessAdmin = pCmdHandlerBase->accessAdmin();
        row.sCommandName = sCmd;
        nMaxLenCommandName = std::max(nMaxLenCommandName, (int)sCmd.length());
        vRows.push_back(row);
    }

    std::cout << " +-" << std::setfill('-') << std::setw(nMaxLenCommandName) << "-" << "-+"
        << "--------+--------+--------+\n";
    std::cout << " | " << std::setfill(' ') << std::setw(nMaxLenCommandName) << "Command Name" << " |"
        << " Access | Access | Access |\n";
    std::cout << " +-" << std::setfill('-') << std::setw(nMaxLenCommandName) << "-"       << "-+"
        << "--------+--------+--------+\n";

    for (unsigned int i = 0; i < vRows.size(); i++) {
        std::cout << " | " << std::setfill(' ') << std::setw(nMaxLenCommandName) << vRows[i].sCommandName << " |";
        std::cout << " " << (vRows[i].bAccessUnauth ? "unauth" : "      ") << " |";
        std::cout << "  " << (vRows[i].bAccessUser ? "user" : "    ") << "  |";
        std::cout << " " << (vRows[i].bAccessAdmin ? "admin" : "     ") << "  |";
        std::cout << "\n";
    }
    std::cout << " +-" << std::setfill('-') << std::setw(nMaxLenCommandName) << "-"       << "-+"
        << "--------+--------+--------+\n";

    std::cout << "\n\n";
    return 0;
}
