/*
MIT License

Copyright (c) 2019-2025 wsjcpp

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Official Source Code: https://github.com/wsjcpp/wsjcpp-yaml
*/

#ifndef WSJCPP_YAML_H
#define WSJCPP_YAML_H

#include <iostream>
#include <vector>
#include <cstddef>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------------------

enum WsjcppYamlNodeType {
    WSJCPP_YAML_NODE_UNDEFINED = 0,
    WSJCPP_YAML_NODE_EMPTY = 1,
    WSJCPP_YAML_NODE_ARRAY = 2,
    WSJCPP_YAML_NODE_MAP = 3,
    WSJCPP_YAML_NODE_VALUE = 4
};

// ---------------------------------------------------------------------

class WsjcppYamlPlaceInFile {
    public:
        WsjcppYamlPlaceInFile();
        WsjcppYamlPlaceInFile(const std::string &sFilename, int nNumberOfLine, const std::string &sLine);

        std::string getFilename() const;
        void setFilename(const std::string &sFilename);

        int getNumberOfLine() const;
        void setNumberOfLine(int nNumberOfLine);

        std::string getLine() const;
        void setLine(const std::string &sLine);
        std::string getForLogFormat();

    private:
        std::string m_sFilename;
        int m_nNumberOfLine;
        std::string m_sLine;
};

// ---------------------------------------------------------------------
// WsjcppYamlQuotes

enum WsjcppYamlQuotes {
    WSJCPP_YAML_QUOTES_NONE,
    WSJCPP_YAML_QUOTES_DOUBLE,
    WSJCPP_YAML_QUOTES_SINGLE
};

class IWsjcppYamlLog {
    public:
        virtual void err(const std::string &TAG, const std::string &sMessage) = 0;
        virtual void throw_err(const std::string &TAG, const std::string &sMessage) = 0;
        virtual void warn(const std::string &TAG, const std::string &sMessage) = 0;
        virtual void info(const std::string &TAG, const std::string &sMessage) = 0;
};

// ---------------------------------------------------------------------
/*!
    \brief Class for keep data of yaml node

    Basic class for yaml tree
*/

class WsjcppYamlNode {
    public:
        WsjcppYamlNode(
            WsjcppYamlNode *pParent,
            IWsjcppYamlLog *pLog,
            const WsjcppYamlPlaceInFile &placeInFile,
            WsjcppYamlNodeType nItemType
        );
        // WsjcppYamlNode(
        //     WsjcppYamlNode *pParent,
        //     WsjcppYamlNodeType nItemType
        //     const WsjcppYamlPlaceInFile &placeInFile,
        // );
        ~WsjcppYamlNode();
        WsjcppYamlNode *getParent();

        WsjcppYamlPlaceInFile getPlaceInFile();
        void setPlaceInFile(const WsjcppYamlPlaceInFile &placeInFile);

        void setComment(const std::string &sComment);
        std::string getComment();

        void setName(const std::string &sName, WsjcppYamlQuotes nNameQuotes = WSJCPP_YAML_QUOTES_NONE);
        std::string getName();
        WsjcppYamlQuotes getNameQuotes();

        bool isEmpty();
        void doEmpty();

        bool isUndefined();
        void doArray();
        void doMap();
        void doValue();

        bool isMap();
        bool hasElement(const std::string &sName);
        WsjcppYamlNode *getElement(const std::string &sName);
        bool setElement(const std::string &sName, WsjcppYamlNode *pItem);
        bool removeElement(const std::string &sName);
        std::vector<std::string> getKeys();

        bool setElementValue(
            const std::string &sName, const std::string &sValue,
            WsjcppYamlQuotes nNameQuotes = WSJCPP_YAML_QUOTES_NONE,
            WsjcppYamlQuotes nValueQuotes = WSJCPP_YAML_QUOTES_NONE
        );

        bool createElementMap(const std::string &sName, WsjcppYamlQuotes nNameQuotes = WSJCPP_YAML_QUOTES_NONE);
        WsjcppYamlNode *createElementMap();
        bool createElementArray(const std::string &sName, WsjcppYamlQuotes nNameQuotes = WSJCPP_YAML_QUOTES_NONE);

        bool isArray();
        int getLength();
        WsjcppYamlNode *getElement(int i);
        bool appendElement(WsjcppYamlNode *pItem);
        bool appendElementValue(const std::string &sValue, WsjcppYamlQuotes nValueQuotes = WSJCPP_YAML_QUOTES_NONE);
        bool removeElement(int i);

        bool isValue();

        std::string getValue(); // contains only strings

        void setValue(const std::string &sValue, WsjcppYamlQuotes nQuotes = WSJCPP_YAML_QUOTES_NONE);
        WsjcppYamlQuotes getValueQuotes();

        std::string getSerializedName();
        std::string toString(std::string sIndent = "");
        std::string getNodeTypeAsString();

        std::string getForLogFormat();
        int getNodeLastIndent();
        std::string getStringNodeLastIndent();
        void setNodeIndents(const std::vector<int> & vNodeIndents);
        int getNodeIndent();

        int getNumberOfLine() const;
        void setNumberOfLine(int nNumberOfLine);

    private:
        void throw_error(const std::string &sError);
        void removeLastCharNewLine(std::string &sLine);
        bool hasContent(const std::string &sVal);
        bool hasObjects();
        std::string TAG;
        IWsjcppYamlLog *m_pLog;
        WsjcppYamlNode *m_pParent;
        WsjcppYamlPlaceInFile m_placeInFile;
        WsjcppYamlNodeType m_nItemType;
        std::vector<WsjcppYamlNode *> m_vObjects;
        std::string m_sValue; // if it is not array or map
        WsjcppYamlQuotes m_nValueQuotes;
        std::string m_sName;
        WsjcppYamlQuotes m_nNameQuotes;
        std::string m_sComment;
        int m_nNodeDiffIndent;
        std::string m_sNodeDiffIndent;
        int m_nNodeIndent;
};

// ---------------------------------------------------------------------

class WsjcppYamlParsebleLine {
    public:
        WsjcppYamlParsebleLine(int nLine);
        WsjcppYamlParsebleLine();
        int getLineNumber();

        std::string getPrefix();
        int getIndent(); // prefix length
        bool isArrayItem();
        std::string getComment();
        bool hasComment();
        std::string getName();
        WsjcppYamlQuotes getNameQuotes();
        bool isEmptyName();
        std::string getValue();
        WsjcppYamlQuotes getValueQuotes();
        bool isEmptyValue();
        bool isEmptyLine();

        bool parseLine(const std::string &sLine, std::string &sError);

    private:
        void initInstance(int nLine);

        std::string TAG;
        int m_nLineNumber;

        std::string m_sPrefix;
        bool m_bArrayItem;
        std::string m_sComment;
        std::string m_sTagName;
        std::string m_sValue;
        WsjcppYamlQuotes m_nNameQuotes;
        WsjcppYamlQuotes m_nValueQuotes;
        bool m_bHasComment;
        bool m_bEmptyLine;

        bool canTagName(const std::string &sVal);
        std::string removeStringDoubleQuotes(const std::string &sValue);
        std::string removeStringSingleQuotes(const std::string &sValue);
};

// ---------------------------------------------------------------------

class WsjcppYamlCursor {
    public:
        WsjcppYamlCursor(WsjcppYamlNode *pCurrentNode);
        WsjcppYamlCursor();
        ~WsjcppYamlCursor();

        // null or undefined
        bool isNull() const;

        // isUndefined
        bool isUndefined() const;

        // value
        bool isValue() const;

        // array
        bool isArray() const;
        size_t size() const;
        // WsjcppYamlCursor &push(const std::string &sVal);
        // WsjcppYamlCursor &push(int nVal);
        // WsjcppYamlCursor &push(bool bVal);
        // WsjcppYamlCursor &remove(int nIdx);

        // map
        bool isMap() const;
        std::vector<std::string> keys() const;
        bool hasKey(const std::string &sKey) const;
        // WsjcppYamlCursor &set(const std::string &sName, const std::string &sValue);
        // WsjcppYamlCursor &set(const std::string &sName, int nValue);
        // WsjcppYamlCursor &set(const std::string &sName, bool bValue);
        // WsjcppYamlCursor &remove(const std::string &sKey);

        // comment
        std::string comment();
        WsjcppYamlCursor &comment(const std::string& sComment);

        // val
        std::string valStr() const;
        WsjcppYamlCursor &val(const std::string &sValue);
        WsjcppYamlCursor &val(const char *sValue);
        int valInt() const;
        WsjcppYamlCursor &val(int nValue);
        float valFloat() const;
        WsjcppYamlCursor &val(float nValue);
        double valDouble() const;
        WsjcppYamlCursor &val(double nValue);
        bool valBool() const;
        WsjcppYamlCursor &val(bool bValue);

        // node
        WsjcppYamlNode *node();

        std::string getCurrentNodePath();

        operator int() const { return int(valInt()); };
        // explicit operator bool() const { return valBool(); }
        operator std::string() const { return valStr(); };

        WsjcppYamlCursor operator[](int idx) const;
        WsjcppYamlCursor operator[](const std::string &sName) const;
        WsjcppYamlCursor operator[](const char *sName) const;

        WsjcppYamlCursor& operator=(const char *sVal);
        WsjcppYamlCursor& operator=(const std::string &sVal);
        WsjcppYamlCursor& operator=(const int &nVal);
        WsjcppYamlCursor& operator=(const bool &bVal);

    private:
        void initInstance(WsjcppYamlNode *pCurrentNode);

        std::string TAG;
        WsjcppYamlNode *m_pCurrentNode;
};

class WsjcppYaml : public IWsjcppYamlLog {
    public:
        WsjcppYaml();
        ~WsjcppYaml();
        void clear();
        void setLogger(IWsjcppYamlLog *pLog);
        bool loadFromFile(const std::string &sFileName, std::string &sError);
        bool saveToFile(const std::string &sFileName, std::string &sError);
        bool loadFromString(const std::string &sBufferName, const std::string &sBuffer, std::string &sError);
        bool saveToString(std::string &sBuffer, std::string &sError);
        WsjcppYamlNode *getRoot();

        WsjcppYamlCursor getCursor() const;
        WsjcppYamlCursor operator[](int idx) const;
        WsjcppYamlCursor operator[](const std::string &sName) const;

        // copy functions from WsjcppCore
        static bool readTextFile(const std::string &sFilename, std::string &sOutputContent, std::string &sError);
        static bool writeFile(const std::string &sFilename, const std::string &sContent);
        static std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
        static std::string toLower(const std::string &str);

        // IWsjcppYamlLog
        #if defined(__CODEGEARC__) && !defined(_WIN64)
        virtual void err(const std::string &TAG, const std::string &sMessage);
        virtual void throw_err(const std::string &TAG, const std::string &sMessage);
        virtual void warn(const std::string &TAG, const std::string &sMessage);
        virtual void info(const std::string &TAG, const std::string &sMessage);
        #else
        virtual void err(const std::string &TAG, const std::string &sMessage) override;
        virtual void throw_err(const std::string &TAG, const std::string &sMessage) override;
        virtual void warn(const std::string &TAG, const std::string &sMessage) override;
        virtual void info(const std::string &TAG, const std::string &sMessage) override;
        #endif

    private:
        std::string TAG;
        IWsjcppYamlLog *m_pLog;

        // TODO replace to WsjcppCore::split()
        std::vector<std::string> splitToLines(const std::string &sBuffer);
        bool parse(const std::string &sFileName, const std::string &sBuffer, std::string &sError);
        void process_hasName_emptyValue_arrayItem();
        void process_hasName_emptyValue_noArrayItem();
        void process_hasName_hasValue_arrayItem();
        void process_hasName_hasValue_noArrayItem();
        void process_emptyName_hasValue_arrayItem();
        void process_emptyName_hasValue_noArrayItem();
        void process_emptyName_emptyValue_arrayItem();
        void process_emptyName_emptyValue_noArrayItem();

        std::vector<std::string> m_sLines;
        WsjcppYamlNode *m_pRoot;

        // prsing line status
        void logUnknownParseLine();
        WsjcppYamlNode *m_pParseCurrentParentNode;
        int m_nParseCurrentIndent;
        WsjcppYamlPlaceInFile m_parsePlaceInFile;
        WsjcppYamlParsebleLine m_parseLine;
        std::vector<int> m_vStackDiffNodeIndents;
};

#endif // WSJCPP_YAML_H

