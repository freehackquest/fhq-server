#include "help_parse_args.h"
#include <unistd.h>
#include <iostream>
#include <thread>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <sstream>


#include <iostream>
#include <algorithm>
#include <iomanip> // setfill, setw 

// ---------------------------------------------------------------------

HelpParseArg::HelpParseArg(const std::string &sName, const std::string &sAlias, HelpParseArgType nType, const std::string &sDescription) {
     m_sName = sName;
     m_sAlias = sAlias;
     m_sDescription = sDescription;
     m_bIsOption = (nType == HelpParseArgType::PARAMETER);
}

// ---------------------------------------------------------------------

HelpParseArg::HelpParseArg() {
     m_sName = "";
     m_sAlias = "";
     m_sDescription = "";
     m_bIsOption = false;
 }

// ---------------------------------------------------------------------

std::string HelpParseArg::name(){
     return m_sName;
}

// ---------------------------------------------------------------------

std::string HelpParseArg::alias(){
     return m_sAlias;
}

// ---------------------------------------------------------------------

std::string HelpParseArg::description(){
    return m_sDescription;
}

// ---------------------------------------------------------------------

bool HelpParseArg::isOption(){
    return m_bIsOption;
}

// ---------------------------------------------------------------------

HelpParseArgs::HelpParseArgs(int argc, char** argv){
    for(int i = 0; i < argc; i++){
        m_vArgs.push_back(std::string(argv[i]));
    }
}

// ---------------------------------------------------------------------

void HelpParseArgs::setAppName(const std::string &sAppName) {
    m_sAppName = sAppName;
}

// ---------------------------------------------------------------------

void HelpParseArgs::setAppVersion(const std::string &sAppVersion) {
    m_sAppVersion = sAppVersion;
}

// ---------------------------------------------------------------------

std::string HelpParseArgs::option(const std::string &sName) {
    HelpParseArg *pHpa = NULL;
    for (unsigned int i = 0; i < m_vHelpArgs.size(); i++) {
        if (m_vHelpArgs[i]->name() == sName) {
            pHpa = m_vHelpArgs[i];
            break;
        }
    }
    if (pHpa == NULL) {
        std::cout << "Error: Not defined attribute \n";
        // Log::err(TAG, "Not defined attribute");
        return "";
    }

    if (!pHpa->isOption()) {
        std::cout << "Error: It's not option \n";
        return "";
    }

    std::vector<std::string>::iterator it1 = std::find(m_vArgs.begin(), m_vArgs.end(), pHpa->name());
    if (it1 != m_vArgs.end() && std::next(it1, 1) != m_vArgs.end()) {
        return * std::next(it1, 1);
    }

    std::vector<std::string>::iterator it2 = std::find(m_vArgs.begin(), m_vArgs.end(), pHpa->alias());
    if (it2 != m_vArgs.end() && std::next(it2, 1) != m_vArgs.end()) {
        return * std::next(it2, 1);
    }
    return "";
}

// ---------------------------------------------------------------------

bool HelpParseArgs::has(const std::string &sName){
    std::vector<std::string> m_vAliases;
    m_vAliases.push_back(sName);
    HelpParseArg *pHpa = NULL;
    for (unsigned int i = 0; i < m_vHelpArgs.size(); i++) {
        if (m_vHelpArgs[i]->name() == sName) {
            pHpa = m_vHelpArgs[i];
            m_vAliases.push_back(pHpa->alias());
        }
    }
    if (pHpa == NULL) {
        std::cout << "Error: Not defined attribute \n";
        // Log::err(TAG, "Not defined attribute");
        return false;
    }

    for (unsigned int i = 0; i < m_vAliases.size(); i++) {
        std::string sAlias = m_vAliases[i];
        std::vector<std::string>::iterator it = std::find(m_vArgs.begin(), m_vArgs.end(), sAlias);
        if (it != m_vArgs.end()) {
            // std::cout << "Found " + sAlias << ", " << pHpa->name() << ", " << (pHpa->isOption() ? "option" : "not option") <<  " \n";
            
        }
        if (it != m_vArgs.end() && !pHpa->isOption()) {
            return true;
        }

        if (pHpa->isOption() && it != m_vArgs.end() && std::next(it, 1) != m_vArgs.end()) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

void HelpParseArgs::addHelp(const std::string &sName, const std::string &sAlias, HelpParseArgType nType, const std::string &sDescription) {
    m_vHelpArgs.push_back(new HelpParseArg(sName, sAlias, nType, sDescription));
}

// ---------------------------------------------------------------------

void HelpParseArgs::printHelp(){
    // TODO calculate max size
    int nMaxSize = 0;
    for(unsigned int i = 0; i < m_vHelpArgs.size(); i++){
        HelpParseArg *pHpa = m_vHelpArgs[i];
        std::string sTmp = pHpa->name() + ", " + pHpa->alias();
        nMaxSize = std::max(nMaxSize, (int)sTmp.size());
    }
    nMaxSize += 4;

    // TODO:
    std::cout << "" << m_sAppName << " " << m_sAppVersion << "\n";
    std::cout << "Usage: " << m_vArgs.at(0) << "   [PARAM]\n";
    for(unsigned int i = 0; i < m_vHelpArgs.size(); i++){
        HelpParseArg *pHpa = m_vHelpArgs[i];
        std::string sTmp = pHpa->name() + ", " + pHpa->alias();
        int nIntent = nMaxSize - sTmp.size();

        std::cout << "\t " << sTmp;
        for(int x = 0; x < nIntent; x++){
            std::cout << " ";
        }
        std::cout << pHpa->description() << "\n";
    }

    std::cout << "\n";
}

// ---------------------------------------------------------------------

bool HelpParseArgs::checkArgs(std::string &sArgsErrors) {
    // TODO 
    return true;
}

// ---------------------------------------------------------------------