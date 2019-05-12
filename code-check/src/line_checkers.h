#ifndef LINE_CHECKERS_H
#define LINE_CHECKERS_H
#include <string>
#include <vector>
#include <json.hpp>
#include <regex>

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
        LineCheckerBase(const std::string &sName);
        std::string getName();
        void printWrongLine(const FileLine &line, const std::string &sError);
        bool isSuccess();
        void printResult();

        virtual void checkLine(const FileLine &line, const nlohmann::json &jsonConfig) = 0;

    private:
        std::string m_sName;
        int m_nCounter;
};

// ---------------------------------------------------------------------

class LineCheckerTabsInLine : LineCheckerBase {
    public:
        LineCheckerTabsInLine();
        virtual void checkLine(const FileLine &line, const nlohmann::json &jsonConfig) override;
    
    private:
        std::regex m_rxTabsInLine;
};

#endif // LINE_CHECKERS_H