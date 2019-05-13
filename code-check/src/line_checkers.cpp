#include "line_checkers.h"
#include "logger.h"

FileLine::FileLine(const std::string &sFilePath, int nLineNumber, const std::string &sLine) {
    m_sFilePath = sFilePath;
    m_nLineNumber = nLineNumber;
    m_sLine = sLine;
}

// ---------------------------------------------------------------------

const std::string &FileLine::getFilePath() const {
    return m_sFilePath;
}

// ---------------------------------------------------------------------

int FileLine::getLineNumber() const {
    return m_nLineNumber;
}

// ---------------------------------------------------------------------

const std::string &FileLine::getLine() const {
    return m_sLine;
}

// ---------------------------------------------------------------------

LineCheckerBase::LineCheckerBase(const std::string &sName, const nlohmann::json &jsonConfig) {
    m_sName = sName;
    m_nCounter = 0;
    TAG = "LineCheckerBase-" + sName;
    const nlohmann::json line_checkers = jsonConfig["line-checkers"];
    if (line_checkers.find(m_sName) == line_checkers.end()) {
        Log::throw_err(TAG, "'line-checkers' must contains '" + m_sName + "'");
    }
    m_sType = line_checkers[m_sName];
    if (m_sType != "err" && m_sType != "warn" && m_sType != "ignore") {
        Log::throw_err(TAG, "'line-checkers'['" + m_sName + "'] "
            "expected one of ['err', 'warn', 'ignoroe'] but got '" + m_sType + "'");
    }
}

// ---------------------------------------------------------------------

std::string LineCheckerBase::getType() {
    return m_sType;
}

// ---------------------------------------------------------------------

std::string LineCheckerBase::getName() {
    return m_sName;
}

// ---------------------------------------------------------------------

void LineCheckerBase::printWrongLine(const FileLine &line, const std::string &sError) {
    m_nCounter++;
    std::string sMessage =
          "\n    File: " + line.getFilePath() + "(" + std::to_string(line.getLineNumber()) + ")"
        + "\n    Line: '" + line.getLine() + "'"
        + "\n    What: " + sError;

    if (m_sType == "err") {
        Log::err(TAG, sMessage);
    } else if (m_sType == "warn") {
        Log::warn(TAG, sMessage);
    } else if (m_sType == "ignore") {
        // silent
    } else {
        Log::throw_err(TAG, sMessage);
    }
}

// ---------------------------------------------------------------------

bool LineCheckerBase::isSuccess() {
    return m_nCounter == 0;
}

// ---------------------------------------------------------------------

void LineCheckerBase::printResult() {
    std::string sMessage = std::to_string(m_nCounter) + " times in code";
    if (isSuccess()) {
        Log::ok(getName(), sMessage);
    } else if (m_sType == "warn") {
        Log::warn(getName(), sMessage);
    } else {
        Log::err(getName(), sMessage);
    }
}

// ---------------------------------------------------------------------

LineCheckerTabsInLine::LineCheckerTabsInLine(const nlohmann::json &jsonConfig)
: LineCheckerBase("tabs", jsonConfig) {
    m_rxTabsInLine = std::regex(".*\\t+.*");
}

// ---------------------------------------------------------------------

void LineCheckerTabsInLine::checkLine(const FileLine &line, const nlohmann::json &jsonConfig) {
    if (std::regex_match(line.getLine(), m_rxTabsInLine)) {
        printWrongLine(line, "Found tabs in line");
    }
}

// ---------------------------------------------------------------------

LineCheckerTODOInLine::LineCheckerTODOInLine(const nlohmann::json &jsonConfig)
: LineCheckerBase("TODO", jsonConfig) {
    m_rxTODOInLine = std::regex(".*TODO.*");
}

// ---------------------------------------------------------------------

void LineCheckerTODOInLine::checkLine(const FileLine &line, const nlohmann::json &jsonConfig) {
    if (std::regex_match(line.getLine(), m_rxTODOInLine)) {
        printWrongLine(line, "Found TODO in line ");
    }
}

// ---------------------------------------------------------------------

LineCheckerIfFormat::LineCheckerIfFormat(const nlohmann::json &jsonConfig)
: LineCheckerBase("if-format", jsonConfig) {
    m_rxIfFormatInLine = std::regex(".*[ ]+if\\(.*");
}

// ---------------------------------------------------------------------

void LineCheckerIfFormat::checkLine(const FileLine &line, const nlohmann::json &jsonConfig) {
    if (std::regex_match(line.getLine(), m_rxIfFormatInLine)) {
        printWrongLine(line, "Found TODO in line ");
    }
}
