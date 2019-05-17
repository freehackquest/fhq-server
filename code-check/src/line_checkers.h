#ifndef LINE_CHECKERS_H
#define LINE_CHECKERS_H
#include <string>
#include <vector>
#include <json.hpp>
#include <regex>
#include "config.h"

class FileLine {
    public:
        FileLine(const std::string &sFilePath, int nLineNumber, const std::string &sLine);
        const std::string &getFilePath() const;
        int getLineNumber() const;
        const std::string &getLine() const;

    private:
        std::string m_sFilePath;
        int m_nLineNumber;
        std::string m_sLine;
};

// ---------------------------------------------------------------------

class LineCheckerBase {
    public:
        LineCheckerBase(const std::string &sName, CodeCheckConfig *pConfig);
        std::string getName();
        std::string getType();
        void printWrongLine(const FileLine &line, const std::string &sError);
        bool isSuccess();
        void printResult();

        virtual void checkLine(const FileLine &line, const nlohmann::json &jsonConfig) = 0;

    private:
        std::string TAG;
        std::string m_sName;
        std::string m_sType;
        int m_nCounter;
        CodeCheckConfig *m_pConfig;
};

// ---------------------------------------------------------------------

class LineCheckerRegExp : LineCheckerBase {
    public:
        LineCheckerRegExp(
            const std::string &sName,
            CodeCheckConfig *pConfig,
            const std::regex &rxRegExp
        );
        virtual void checkLine(const FileLine &line, const nlohmann::json &jsonConfig) override;

    private:
        std::regex m_rxForLine;
};

// ---------------------------------------------------------------------

class LineCheckerTabsInLine : LineCheckerBase {
    public:
        LineCheckerTabsInLine(CodeCheckConfig *pConfig);
        virtual void checkLine(const FileLine &line, const nlohmann::json &jsonConfig) override;
    
    private:
        std::regex m_rxTabsInLine;
};

// ---------------------------------------------------------------------

class LineCheckerTODOInLine : LineCheckerRegExp {
    public:
        LineCheckerTODOInLine(CodeCheckConfig *pConfig);
};

// ---------------------------------------------------------------------

class LineCheckerIfFormat : LineCheckerRegExp {
    public:
        LineCheckerIfFormat(CodeCheckConfig *pConfig);
};

// ---------------------------------------------------------------------

class LineCheckerWhileFormat : LineCheckerRegExp {
    public:
        LineCheckerWhileFormat(CodeCheckConfig *pConfig);
};

// ---------------------------------------------------------------------

class LineCheckerForFormat : LineCheckerRegExp {
    public:
        LineCheckerForFormat(CodeCheckConfig *pConfig);
};

// ---------------------------------------------------------------------

class LineCheckerEndBrackets : LineCheckerRegExp {
    public:
        LineCheckerEndBrackets(CodeCheckConfig *pConfig);
};

// ---------------------------------------------------------------------

class LineCheckerStartBracketElse : LineCheckerRegExp {
    public:
        LineCheckerStartBracketElse(CodeCheckConfig *pConfig);
};

// ---------------------------------------------------------------------

class LineCheckerEndBracketElse : LineCheckerRegExp {
    public:
        LineCheckerEndBracketElse(CodeCheckConfig *pConfig);
};

// ---------------------------------------------------------------------

class LineCheckerAuto : LineCheckerRegExp {
    public:
        LineCheckerAuto(CodeCheckConfig *pConfig);
};

#endif // LINE_CHECKERS_H