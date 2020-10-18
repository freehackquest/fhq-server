#include <string>
#include <iostream>
#include <json.hpp>
#include <fstream>
#include <sstream>
#include <regex>
#include "line_checkers.h"
#include "config.h"
#include <wsjcpp_core.h>

nlohmann::json g_jsonWarnings;
nlohmann::json g_jsonErrors;
std::vector<LineCheckerBase *> g_vLineCheckers;

// ---------------------------------------------------------------------

int startRecoursiveCheckSimpleFormat(const std::string &sRootDir, const std::string &sBasePath, const nlohmann::json &jsonConfig);

// ---------------------------------------------------------------------

int main(int argc, char** argv) {
    std::string TAG = "MAIN";
    std::string appName = std::string(WSJCPP_NAME);
    std::string appVersion = std::string(WSJCPP_VERSION);

    std::string appLogPath = ".logs";
    if (!WsjcppCore::dirExists(appLogPath)) {
        WsjcppCore::makeDir(appLogPath);
    }
    WsjcppLog::setPrefixLogFile(appName);
    WsjcppLog::setLogDirectory(".logs");

    CodeCheckConfig *pConfig = new CodeCheckConfig();

    pConfig->applyArguments(argc, argv);

    // registry line checkers
    g_vLineCheckers.push_back((LineCheckerBase *)new LineCheckerTabsInLine(pConfig));
    g_vLineCheckers.push_back((LineCheckerBase *)new LineCheckerTODOInLine(pConfig));
    g_vLineCheckers.push_back((LineCheckerBase *)new LineCheckerIfFormat(pConfig));
    g_vLineCheckers.push_back((LineCheckerBase *)new LineCheckerWhileFormat(pConfig));
    g_vLineCheckers.push_back((LineCheckerBase *)new LineCheckerForFormat(pConfig));
    g_vLineCheckers.push_back((LineCheckerBase *)new LineCheckerEndBrackets(pConfig));
    g_vLineCheckers.push_back((LineCheckerBase *)new LineCheckerStartBracketElse(pConfig));
    g_vLineCheckers.push_back((LineCheckerBase *)new LineCheckerEndBracketElse(pConfig));
    g_vLineCheckers.push_back((LineCheckerBase *)new LineCheckerAuto(pConfig));

    // TODO: check int nNumber 
    // TODO: lines in function/method (multiline)
    // TODO: size of line
    // TODO format of if (multiline)
    // TODO: check // ----------- in cpp files and in headers
    // TODO: foreach - forbiden use a while or for
    // TODO: d==0 - missing spaces
    // TODO: check the more then one space
    // TODO: { before must be one space
    // TODO: if () {
    // TODO: check start method like get/set/is/do etc... and check camalCase name of methods/functions
    nlohmann::json jsonConfig = pConfig->getJsonConfig();
    std::vector<std::string> vPaths = jsonConfig["paths"];
    for (int i = 0; i < vPaths.size(); i++) {
        std::string sBasePath = vPaths[i];
        sBasePath = pConfig->getRootDir() + "/" + sBasePath;
        if (!WsjcppCore::dirExists(sBasePath)) {
            WsjcppLog::throw_err(TAG, "Path '" + sBasePath + "' does not exists " + sBasePath);
        }
        startRecoursiveCheckSimpleFormat(pConfig->getRootDir(), sBasePath, pConfig->getJsonConfig());
    }
    int nRet = 0;
    for (int i = 0; i < g_vLineCheckers.size(); i++) {
        LineCheckerBase *pChecker = g_vLineCheckers[i];
        pChecker->printResult();
        if (!pChecker->isSuccess() && pChecker->getType() == "err") {
            nRet = -1;
        }
    }

    return nRet;
}

// ---------------------------------------------------------------------

int readFileAndCheckSimpleFormat(const std::string &sFilePath, const nlohmann::json &jsonConfig) {
    std::string sConfig;
    std::ifstream f( sFilePath );
    std::string sLine;
    int nLineNumber = 1;
    while (std::getline(f, sLine)) {
        // std::cout << nLineNumber << ": " << sLine << std::endl;
        FileLine line(sFilePath, nLineNumber, sLine);
        for (int i = 0; i < g_vLineCheckers.size(); i++) {
            g_vLineCheckers[i]->checkLine(line, jsonConfig);
        }
        nLineNumber++;
    }
    return 0;
}

// ---------------------------------------------------------------------

int startRecoursiveCheckSimpleFormat(const std::string &sRootDir, const std::string &sBasePath, const nlohmann::json &jsonConfig) {
    std::string TAG = "RECOURSIVE";
    std::vector<std::string> _vExludes = jsonConfig["exclude"];
    std::vector<std::string> vExludes;
    
    for (int e = 0; e < _vExludes.size(); e++) {
        std::string sExclude = sRootDir + "/" + _vExludes[e] + "/"; // todo normalize path
        vExludes.push_back(sExclude);
        vExludes.push_back(_vExludes[e]);
    }

    std::vector<std::string> vListOfDirs = WsjcppCore::listOfDirs(sBasePath);
    for (int i = 0; i < vListOfDirs.size(); i++) {
        
        std::string sNewPath = sBasePath + vListOfDirs[i] + "/";
        bool bExclude = false;
        for (int e = 0; e < vExludes.size(); e++) {
            if (vExludes[e] == sNewPath || vExludes[e] == vListOfDirs[i]) {
                bExclude = true;
            }
        }
        if (!bExclude) {
            // std::cout << "Dir: " << sNewPath << std::endl;
            startRecoursiveCheckSimpleFormat(sRootDir, sNewPath, jsonConfig);
        } else {
            WsjcppLog::warn(TAG, "Skipped Dir: " + sNewPath);
        }
    }
    std::vector<std::string> vListOfFiles = WsjcppCore::listOfFiles(sBasePath); 
    for (int i = 0; i < vListOfFiles.size(); i++) {
        std::string sNewPath = sBasePath + vListOfFiles[i];
        bool bExclude = false;
        for (int e = 0; e < vExludes.size(); e++) {
            if (vExludes[e] == sNewPath || vExludes[e] == vListOfFiles[i]) {
                bExclude = true;
            }
        }
        if (!bExclude) {
            // std::cout << "Check file: " << sNewPath << std::endl;
            readFileAndCheckSimpleFormat(sNewPath, jsonConfig);
        } else {
            WsjcppLog::warn(TAG, "Skipped File: " + sNewPath);
        }
    }

    return 0;
}

// ---------------------------------------------------------------------