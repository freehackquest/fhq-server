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

LineCheckerBase::LineCheckerBase(const std::string &sName, CodeCheckConfig *pConfig) {
    m_sName = sName;
    m_nCounter = 0;
    m_pConfig = pConfig;
    TAG = "LineCheckerBase-" + sName;
    nlohmann::json jsonConfig = pConfig->getJsonConfig();
    const nlohmann::json line_checkers = jsonConfig["line-checkers"];
    if (line_checkers.find(m_sName) == line_checkers.end()) {
        Log::throw_err(TAG, "'line-checkers' must contains '" + m_sName + "'");
    }
    m_sType = line_checkers[m_sName]; // TODO move check in CodeCheckConfig
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
        if (!m_pConfig->isShowOnlyErrors()) {
            Log::warn(TAG, sMessage);
        }
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

LineCheckerRegExp::LineCheckerRegExp(
    const std::string &sName,
    CodeCheckConfig *pConfig,
    const std::regex &rxRegExp
) : LineCheckerBase(sName, pConfig) {
    m_rxForLine = rxRegExp;
}

// ---------------------------------------------------------------------

void LineCheckerRegExp::checkLine(const FileLine &line, const nlohmann::json &jsonConfig) {
    if (std::regex_match(line.getLine(), m_rxForLine)) {
        printWrongLine(line, "Found " + getName() + " in line");
    }
}

// ---------------------------------------------------------------------

LineCheckerTabsInLine::LineCheckerTabsInLine(CodeCheckConfig *pConfig)
: LineCheckerBase("tabs", pConfig) {
    m_rxTabsInLine = std::regex(".*\\t+.*");
}

// ---------------------------------------------------------------------

void LineCheckerTabsInLine::checkLine(const FileLine &line, const nlohmann::json &jsonConfig) {
    if (std::regex_match(line.getLine(), m_rxTabsInLine)) {
        printWrongLine(line, "Found tabs in line");
    }
}

// ---------------------------------------------------------------------

LineCheckerTODOInLine::LineCheckerTODOInLine(CodeCheckConfig *pConfig)
: LineCheckerRegExp("TODO", pConfig, std::regex(".*TODO.*")) {
    // nothing
}

// ---------------------------------------------------------------------

LineCheckerIfFormat::LineCheckerIfFormat(CodeCheckConfig *pConfig)
: LineCheckerRegExp("if-format", pConfig, std::regex(".*[ ]+if\\(.*")) {
    // nothing
}

// ---------------------------------------------------------------------

LineCheckerWhileFormat::LineCheckerWhileFormat(CodeCheckConfig *pConfig)
: LineCheckerRegExp("while-format", pConfig, std::regex(".*while\\(.*")) {
    // nothing
}

// ---------------------------------------------------------------------

LineCheckerForFormat::LineCheckerForFormat(CodeCheckConfig *pConfig)
: LineCheckerRegExp("for-format", pConfig, std::regex(".*[ ]+for\\(.*")) {
    // nothing
}

// ---------------------------------------------------------------------

LineCheckerEndBrackets::LineCheckerEndBrackets(CodeCheckConfig *pConfig)
: LineCheckerRegExp("end-brackets", pConfig, std::regex(".*\\)\\{.*")) {
    // nothing
}

// ---------------------------------------------------------------------

LineCheckerStartBracketElse::LineCheckerStartBracketElse(CodeCheckConfig *pConfig)
: LineCheckerRegExp("start-bracket-else", pConfig, std::regex(".*\\}else.*")) {
    // nothing
}

// ---------------------------------------------------------------------

LineCheckerEndBracketElse::LineCheckerEndBracketElse(CodeCheckConfig *pConfig)
: LineCheckerRegExp("end-bracket-else", pConfig, std::regex(".*else\\{.*")) {
    // nothing
}

// ---------------------------------------------------------------------

LineCheckerAuto::LineCheckerAuto(CodeCheckConfig *pConfig)
: LineCheckerRegExp("auto", pConfig, std::regex(".*[^\\w]+auto[^\\w]+.*")) {
    // nothing
}

// ---------------------------------------------------------------------
