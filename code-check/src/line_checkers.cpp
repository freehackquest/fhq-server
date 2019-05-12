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

LineCheckerBase::LineCheckerBase(const std::string &sName) {
    m_sName = sName;
    m_nCounter = 0;
}

// ---------------------------------------------------------------------

std::string LineCheckerBase::getName() {
    return m_sName;
}

// ---------------------------------------------------------------------

void LineCheckerBase::printWrongLine(const FileLine &line, const std::string &sError) {
    m_nCounter++;
    Log::err("checkTabsInLine", 
            "\n    File: " + line.getFilePath() + "(" + std::to_string(line.getLineNumber()) + ")"
        + "\n    Line: '" + line.getLine() + "'"
        + "\n    What: " + sError
    );
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
    } else {
        Log::err(getName(), sMessage);
    }
}

// ---------------------------------------------------------------------

LineCheckerTabsInLine::LineCheckerTabsInLine() : LineCheckerBase("tabs") {
    m_rxTabsInLine = std::regex(".*\\t+.*");
}

// ---------------------------------------------------------------------

void LineCheckerTabsInLine::checkLine(const FileLine &line, const nlohmann::json &jsonConfig) {
    if (std::regex_match(line.getLine(), m_rxTabsInLine)) {
        printWrongLine(line, "Found tabs in line");
    }
}