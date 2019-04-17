#include "export_list_of_handlers.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <websocketserver.h>
#include <employees.h>

void ExportListOfHandlers::print() {
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
}
