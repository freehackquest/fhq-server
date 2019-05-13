#include <string>
#include <iostream>
#include "helpers.h"
#include "json.hpp"
#include "logger.h"
#include <fstream>
#include <sstream>
#include <regex>
#include "line_checkers.h"

nlohmann::json g_jsonWarnings;
nlohmann::json g_jsonErrors;
std::vector<LineCheckerBase *> g_vLineCheckers;

/*
warnings = {
    'TODO': 0,
    'if-format': 0,
    'while-format': 0,
    'for-format': 0,
    'end-brackets': 0,
    'start-bracket-else': 0,
    'end-bracket-else': 0,
}

evil_pieces = {
    'auto': 0,
    'tabs': 0,
}
*/

// ---------------------------------------------------------------------

int startRecoursiveCheckSimpleFormat(const std::string &sRootDir, const std::string &sBasePath, const nlohmann::json &jsonConfig);

// ---------------------------------------------------------------------

int main(int argc, char** argv) {
    std::string TAG = "MAIN";
    if (argc != 2) {
        Log::throw_err(TAG, "Usage: " + std::string(argv[0]) + " <dir>");
        return -1;
    }
    
    std::string sRootDir(argv[1]);
    Log::ok(TAG, "Root Directory: " + sRootDir);
    std::string fileConfig = sRootDir + "/.code-check.json";
    if (!Helpers::fileExists(fileConfig)) {
        Log::throw_err(TAG, "Expected file: " + fileConfig);
        return -1;
    }
    
    std::string sConfig;
    std::ifstream f( fileConfig );
    std::string sLine;
    while (std::getline(f, sLine)) {
        sConfig += sLine + "\n";
    }
    
    if (!nlohmann::json::accept(sConfig)) {
        std::cout << "[ERROR] Wrong JSON format " << std::endl
            << "File-config: " << fileConfig << std::endl
            << "File-content: " << sConfig << std::endl;
        return -1;
    }
    nlohmann::json jsonConfig = nlohmann::json::parse(sConfig);
    std::cout << "JSON-Config: " << sConfig << std::endl;

    // registry line checkers - TODO apply config
    g_vLineCheckers.push_back((LineCheckerBase *)new LineCheckerTabsInLine(jsonConfig));
    g_vLineCheckers.push_back((LineCheckerBase *)new LineCheckerTODOInLine(jsonConfig));
    g_vLineCheckers.push_back((LineCheckerBase *)new LineCheckerIfFormat(jsonConfig));

    std::vector<std::string> vPaths = jsonConfig["paths"];
    for (int i = 0; i < vPaths.size(); i++) {
        std::string sBasePath = vPaths[i];
        sBasePath = sRootDir + "/" + sBasePath;
        if (!Helpers::dirExists(sBasePath)) {
            Log::throw_err(TAG, "Path '" + sBasePath + "' does not exists " + sBasePath);
        }
        startRecoursiveCheckSimpleFormat(sRootDir, sBasePath, jsonConfig);
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

    std::vector<std::string> vListOfDirs = Helpers::listOfDirs(sBasePath);
    for (int i = 0; i < vListOfDirs.size(); i++) {
        
        std::string sNewPath = sBasePath + vListOfDirs[i] + "/";
        bool bExclude = false;
        for (int e = 0; e < vExludes.size(); e++) {
            if (vExludes[e] == sNewPath || vExludes[e] == vListOfDirs[i]) {
                bExclude = true;
            }
        }
        if (!bExclude) {
            std::cout << "Dir: " << sNewPath << std::endl;
            startRecoursiveCheckSimpleFormat(sRootDir, sNewPath, jsonConfig);
        } else {
            Log::warn(TAG, "Skipped Dir: " + sNewPath);
        }
    }
    std::vector<std::string> vListOfFiles = Helpers::listOfFiles(sBasePath); 
    for (int i = 0; i < vListOfFiles.size(); i++) {
        std::string sNewPath = sBasePath + vListOfFiles[i];
        bool bExclude = false;
        for (int e = 0; e < vExludes.size(); e++) {
            if (vExludes[e] == sNewPath || vExludes[e] == vListOfFiles[i]) {
                bExclude = true;
            }
        }
        if (!bExclude) {
            std::cout << "Check file: " << sNewPath << std::endl;
            readFileAndCheckSimpleFormat(sNewPath, jsonConfig);
        } else {
            Log::warn(TAG, "Skipped File: " + sNewPath);
        }
    }

    return 0;
}

// ---------------------------------------------------------------------