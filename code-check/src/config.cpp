#include "config.h"
#include <logger.h>
#include <helpers.h>
#include <fstream>
#include <sstream>

CodeCheckConfig::CodeCheckConfig() {
    TAG = "CodeCheckConfig";
    m_bShowOnlyErrors = false;
}

// ---------------------------------------------------------------------

void CodeCheckConfig::applyArguments(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        m_vArgs.push_back(std::string(argv[i]));
    }
    
    if (m_vArgs.size() < 2) {
        Log::throw_err(TAG, "Usage: " + std::string(m_vArgs[0]) + " [options] <dir>");
    }

    m_sRootDir = m_vArgs[m_vArgs.size()-1];
    Log::ok(TAG, "Root Directory: " + m_sRootDir);
    std::string fileConfig = m_sRootDir + "/.code-check.json";
    if (!Helpers::fileExists(fileConfig)) {
        Log::throw_err(TAG, "Expected file: " + fileConfig);
    }

    for (int i = 1; i < m_vArgs.size()-1; i++) {
        if (m_vArgs[i] == "--show-only-errors") {
            m_bShowOnlyErrors = true;
        }
    }

    std::string sConfig;
    std::ifstream f( fileConfig );
    std::string sLine;
    while (std::getline(f, sLine)) {
        sConfig += sLine + "\n";
    }
    
    if (!nlohmann::json::accept(sConfig)) {
        Log::throw_err(TAG, " Wrong JSON format \n"
            "\nFile-config: " + fileConfig
            + "\nFile-content: " + sConfig + "\n"
        );
    }
    m_jsonConfig = nlohmann::json::parse(sConfig);
    std::cout << "JSON-Config: " << sConfig << std::endl;

}

// ---------------------------------------------------------------------

std::string CodeCheckConfig::getRootDir() {
    return m_sRootDir;
}

// ---------------------------------------------------------------------

nlohmann::json CodeCheckConfig::getJsonConfig() {
    return m_jsonConfig;
}

// ---------------------------------------------------------------------

bool CodeCheckConfig::isShowOnlyErrors() {
    return m_bShowOnlyErrors;
}

// ---------------------------------------------------------------------