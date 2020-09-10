#include "export_list_of_handlers.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <websocketserver.h>
#include <employees.h>

void ExportListOfHandlers::print() {
    std::cout << "\n\n * CmdHandlers (" << g_pWsjcppJsonRpc20HandlerList->size() << "):\n";

    struct CmdRow{
        std::string sCommandName;
        bool bAccessUnauth;
        bool bAccessUser;
        bool bAccessTester;
        bool bAccessAdmin;
    };
    std::vector<CmdRow> vRows;
    int nMaxLenCommandName = 0;

    std::map<std::string, WsjcppJsonRpc20HandlerBase*>::iterator it = g_pWsjcppJsonRpc20HandlerList->begin();
    for (; it!=g_pWsjcppJsonRpc20HandlerList->end(); ++it) {
        std::string sCmd = it->first;
        WsjcppJsonRpc20HandlerBase* pWsjcppJsonRpc20HandlerBase = it->second;
        CmdRow row;
        row.bAccessUnauth = pWsjcppJsonRpc20HandlerBase->haveUnauthorizedAccess();
        row.bAccessUser = pWsjcppJsonRpc20HandlerBase->haveUserAccess();
        row.bAccessTester = pWsjcppJsonRpc20HandlerBase->haveTesterAccess();
        row.bAccessAdmin = pWsjcppJsonRpc20HandlerBase->haveAdminAccess();
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
        std::cout << "  " << (vRows[i].bAccessTester ? "tester" : "    ") << "  |";
        std::cout << " " << (vRows[i].bAccessAdmin ? "admin" : "     ") << "  |";
        std::cout << "\n";
    }
    std::cout << " +-" << std::setfill('-') << std::setw(nMaxLenCommandName) << "-"       << "-+"
        << "--------+--------+--------+\n";

    std::cout << "\n\n";
}
