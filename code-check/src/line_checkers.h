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
        LineCheckerBase(const std::string &sName, const nlohmann::json &jsonConfig);
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
};

// ---------------------------------------------------------------------

class LineCheckerTabsInLine : LineCheckerBase {
    public:
        LineCheckerTabsInLine(const nlohmann::json &jsonConfig);
        virtual void checkLine(const FileLine &line, const nlohmann::json &jsonConfig) override;
    
    private:
        std::regex m_rxTabsInLine;
};

// ---------------------------------------------------------------------

class LineCheckerTODOInLine : LineCheckerBase {
    public:
        LineCheckerTODOInLine(const nlohmann::json &jsonConfig);
        virtual void checkLine(const FileLine &line, const nlohmann::json &jsonConfig) override;
    
    private:
        std::regex m_rxTODOInLine;
};

// ---------------------------------------------------------------------

class LineCheckerIfFormat : LineCheckerBase {
    public:
        LineCheckerIfFormat(const nlohmann::json &jsonConfig);
        virtual void checkLine(const FileLine &line, const nlohmann::json &jsonConfig) override;
    
    private:
        std::regex m_rxIfFormatInLine;
};

#endif // LINE_CHECKERS_H