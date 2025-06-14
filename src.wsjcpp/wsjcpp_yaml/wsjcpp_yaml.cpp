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

#include "wsjcpp_yaml.h"
// #include <wsjcpp_core.h>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#if defined(__CODEGEARC__) && !defined(_WIN64)
#include <stdlib.h>
#endif

// ---------------------------------------------------------------------

std::string WSJCPP_INT_TO_STR(int number) {
    #if defined(__CODEGEARC__) && !defined(_WIN64)
    // TODO
    char buffer[] = {
        0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0
    };
    #if __CODEGEARC__ == 0x0770
    // 12.2
    _itoa(number, buffer, 10);
    #else
    itoa(number, buffer, 10);
    #endif
    return std::string(buffer);
    #else
    return std::to_string(number);
    #endif
}

std::string WSJCPP_FLOAT_TO_STR(float number) {
    std::string ret;
    #if defined(__CODEGEARC__) && !defined(_WIN64)
    // TODO
    char buffer[] = {
        0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0
    };
    #if __CODEGEARC__ == 0x0770
    // 12.2
    _itoa(number, buffer, 10);
    #else
    itoa(number, buffer, 10);
    #endif
    ret = std::string(buffer);
    #else
    ret = std::to_string(number);
    #endif
    ret.erase(std::find_if(ret.rbegin(), ret.rend(), [](unsigned char ch) {
        return ch != '0';
    }).base(), ret.end());
    return ret;
}

std::string WSJCPP_DOUBLE_TO_STR(double number) {
    std::string ret;
    #if defined(__CODEGEARC__) && !defined(_WIN64)
    // TODO
    char buffer[] = {
        0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0,
        0x0,0x0,0x0,0x0, 0x0,0x0,0x0,0x0
    };
    #if __CODEGEARC__ == 0x0770
    // 12.2
    _itoa(number, buffer, 10);
    #else
    itoa(number, buffer, 10);
    #endif
    ret = std::string(buffer);
    #else
    ret = std::to_string(number);
    #endif
    ret.erase(std::find_if(ret.rbegin(), ret.rend(), [](unsigned char ch) {
        return ch != '0';
    }).base(), ret.end());
    return ret;
}

// ---------------------------------------------------------------------
// WsjcppYamlPlaceInFile

WsjcppYamlPlaceInFile::WsjcppYamlPlaceInFile() {
    m_sFilename = "";
    m_nNumberOfLine = 0;
    m_sLine = "";
}

WsjcppYamlPlaceInFile::WsjcppYamlPlaceInFile(const std::string &sFilename, int nNumberOfLine, const std::string &sLine) {
    m_sFilename = sFilename;
    m_nNumberOfLine = nNumberOfLine;
    m_sLine = sLine;
}

std::string WsjcppYamlPlaceInFile::getFilename() const {
    return m_sFilename;
}

void WsjcppYamlPlaceInFile::setFilename(const std::string &sFilename) {
    m_sFilename = sFilename;
}

int WsjcppYamlPlaceInFile::getNumberOfLine() const {
    return m_nNumberOfLine;
}

void WsjcppYamlPlaceInFile::setNumberOfLine(int nNumberOfLine) {
    m_nNumberOfLine = nNumberOfLine;
}

std::string WsjcppYamlPlaceInFile::getLine() const {
    return m_sLine;
}

void WsjcppYamlPlaceInFile::setLine(const std::string &sLine) {
    m_sLine = sLine;
}

std::string WsjcppYamlPlaceInFile::getForLogFormat() {
    return "(" + m_sFilename + ":" + WSJCPP_INT_TO_STR(m_nNumberOfLine + 1) + "): " + m_sLine;
}

// ---------------------------------------------------------------------
// WsjcppYamlNode


#if defined(__CODEGEARC__) && !defined(_WIN64)
#define WSJCPP_NULL NULL
#else
#define WSJCPP_NULL nullptr
#endif

WsjcppYamlNode::WsjcppYamlNode(
    WsjcppYamlNode *pParent,
    IWsjcppYamlLog *pLog,
    const WsjcppYamlPlaceInFile &placeInFile,
    WsjcppYamlNodeType nItemType
) {
    m_pParent = pParent;
    m_pLog = pLog;
    m_placeInFile.setFilename(placeInFile.getFilename());
    m_placeInFile.setLine(placeInFile.getLine());
    m_placeInFile.setNumberOfLine(placeInFile.getNumberOfLine());
    m_nItemType = nItemType;
    m_nValueQuotes = WSJCPP_YAML_QUOTES_NONE;
    m_nNameQuotes = WSJCPP_YAML_QUOTES_NONE;
    // TODO get child indent
    if (m_pParent != WSJCPP_NULL && m_pParent->getParent() != WSJCPP_NULL) {
        m_nNodeDiffIndent = 2;
        m_sNodeDiffIndent = "  ";
    } else {
        m_nNodeDiffIndent = 0;
        m_sNodeDiffIndent = "";
    }

    TAG = "WsjcppYamlNode";
}

// ---------------------------------------------------------------------

WsjcppYamlNode::~WsjcppYamlNode() {
    for (unsigned int i = 0; i < m_vObjects.size(); i++) {
        delete m_vObjects[i];
    }
    m_vObjects.clear();
}

// ---------------------------------------------------------------------

WsjcppYamlNode *WsjcppYamlNode::getParent() {
    return m_pParent;
}

// ---------------------------------------------------------------------

WsjcppYamlPlaceInFile WsjcppYamlNode::getPlaceInFile() {
    return m_placeInFile;
}

// ---------------------------------------------------------------------

void WsjcppYamlNode::setPlaceInFile(const WsjcppYamlPlaceInFile &placeInFile) {
    m_placeInFile.setFilename(placeInFile.getFilename());
    m_placeInFile.setLine(placeInFile.getLine());
    m_placeInFile.setNumberOfLine(placeInFile.getNumberOfLine());
}

// ---------------------------------------------------------------------

void WsjcppYamlNode::setComment(const std::string &sComment) {
    m_sComment = sComment;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlNode::getComment() {
    return m_sComment;
}

// ---------------------------------------------------------------------

void WsjcppYamlNode::setName(const std::string &sName, WsjcppYamlQuotes nNameQuotes) {
    m_sName = sName;
    m_nNameQuotes = nNameQuotes;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlNode::getName() {
    return m_sName;
}

// ---------------------------------------------------------------------

WsjcppYamlQuotes WsjcppYamlNode::getNameQuotes() {
    return m_nNameQuotes;
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::isEmpty() {
    return m_nItemType == WSJCPP_YAML_NODE_EMPTY;
}

// ---------------------------------------------------------------------

void WsjcppYamlNode::doEmpty() {
    if (m_nItemType == WSJCPP_YAML_NODE_UNDEFINED) {
        m_nItemType = WSJCPP_YAML_NODE_EMPTY;
    } else {
        throw std::runtime_error(TAG + ": Element already defined as '" + this->getNodeTypeAsString() + "'");
    }
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::isUndefined() {
    return m_nItemType == WSJCPP_YAML_NODE_UNDEFINED;
}

// ---------------------------------------------------------------------

void WsjcppYamlNode::doArray() {
    if (m_nItemType == WSJCPP_YAML_NODE_UNDEFINED) {
        m_nItemType = WSJCPP_YAML_NODE_ARRAY;
    } else {
        throw std::runtime_error(TAG + ": Element already defined as '" + this->getNodeTypeAsString() + "'");
    }
}

// ---------------------------------------------------------------------

void WsjcppYamlNode::doMap() {
    if (m_nItemType == WSJCPP_YAML_NODE_UNDEFINED) {
        m_nItemType = WSJCPP_YAML_NODE_MAP;
    } else {
        throw std::runtime_error(TAG + ": Element already defined as '" + this->getNodeTypeAsString() + "'");
    }
}

// ---------------------------------------------------------------------

void WsjcppYamlNode::doValue() {
    if (m_nItemType == WSJCPP_YAML_NODE_UNDEFINED) {
        m_nItemType = WSJCPP_YAML_NODE_VALUE;
    } else {
        throw std::runtime_error(TAG + ": Element already defined as '" + this->getNodeTypeAsString() + "'");
    }
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::isMap() {
    return m_nItemType == WSJCPP_YAML_NODE_MAP;
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::hasElement(const std::string &sName) {
    if (m_nItemType != WSJCPP_YAML_NODE_MAP) {
        throw std::runtime_error(TAG + ": hasElement('" + sName + "'): Element must be map");
    }
    for (unsigned int i = 0; i < m_vObjects.size(); i++) {
        if (m_vObjects[i]->getName() == sName) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

WsjcppYamlNode *WsjcppYamlNode::getElement(const std::string &sName) {
    if (m_nItemType != WSJCPP_YAML_NODE_MAP) {
        throw std::runtime_error(TAG + ": getElement: Element must be map");
    }

    for (unsigned int i = 0; i < m_vObjects.size(); i++) {
        std::string sObjectName = m_vObjects[i]->getName();
        if (m_vObjects[i]->getName() == sName) {
            return m_vObjects[i];
        }
    }
    throw_error(TAG + "Element '" + sName + "' not found for " + this->getForLogFormat());
    return WSJCPP_NULL;
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::setElement(const std::string &sName, WsjcppYamlNode *pItem) {
    if (m_nItemType == WSJCPP_YAML_NODE_UNDEFINED) {
        m_nItemType = WSJCPP_YAML_NODE_MAP; // change item type to map on first element
    }

    if (m_nItemType != WSJCPP_YAML_NODE_MAP) {
        throw std::runtime_error(TAG + ": setElement, Element must be 'map' for " + pItem->getPlaceInFile().getForLogFormat());
    }

    if (this->hasElement(sName)) { // TODO remove previous element
        throw std::runtime_error(TAG + ": setElement: Current map '" + this->getName()  + "' "
            "(" + m_placeInFile.getFilename() + ":" + WSJCPP_INT_TO_STR(m_placeInFile.getNumberOfLine()) + ") "
            "already has element with this name: '" + sName + "'");
    }
    m_vObjects.push_back(pItem); // TODO create clone
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::removeElement(const std::string &sName) {
    if (m_nItemType != WSJCPP_YAML_NODE_MAP) {
        throw std::runtime_error(TAG + ": removeElement: Element must be map");
    }
    std::vector<WsjcppYamlNode *>::iterator it;
    for (it = m_vObjects.begin(); it != m_vObjects.end(); ++it) {
        WsjcppYamlNode *pItem = *it;
        if (pItem->getName() == sName) {
            m_vObjects.erase(it);
            delete pItem;
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppYamlNode::getKeys() {
    if (m_nItemType != WSJCPP_YAML_NODE_MAP) {
        throw std::runtime_error(TAG + ": getKeys: Element must be map");
    }
    std::vector<std::string> vKeys;
    for (unsigned int i = 0; i < m_vObjects.size(); i++) {
        WsjcppYamlNode *pItem = m_vObjects[i];
        if (pItem->isValue() || pItem->isMap() || pItem->isArray() || pItem->isUndefined()) {
            std::string sName = pItem->getName();
            vKeys.push_back(sName);
        }
    }
    return vKeys;
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::setElementValue(
    const std::string &sName,
    const std::string &sValue,
    WsjcppYamlQuotes nNameQuotes,
    WsjcppYamlQuotes nValueQuotes
) {
    if (m_nItemType == WSJCPP_YAML_NODE_UNDEFINED) {
        m_nItemType = WSJCPP_YAML_NODE_MAP; // change item type to map on first element  
    }

    if (m_nItemType != WSJCPP_YAML_NODE_MAP) {
        throw std::runtime_error(TAG + ": setElement, Element must be 'map' for " + this->getPlaceInFile().getForLogFormat());
    }
    
    if (this->hasElement(sName)) {
        WsjcppYamlNode *pItem = this->getElement(sName);
        pItem->setValue(sValue, nValueQuotes);
    } else {
        WsjcppYamlPlaceInFile pl;
        WsjcppYamlNode *pNewItem = new WsjcppYamlNode(this, m_pLog, pl, WSJCPP_YAML_NODE_VALUE);
        pNewItem->setName(sName, nNameQuotes);
        pNewItem->setValue(sValue, nValueQuotes);
        this->setElement(sName, pNewItem);
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::createElementMap(const std::string &sName, WsjcppYamlQuotes nNameQuotes) {
    if (m_nItemType != WSJCPP_YAML_NODE_MAP ) {
        throw std::runtime_error(TAG + ": createElementMap, Element must be 'map' for " + this->getPlaceInFile().getForLogFormat());
    }
    if (this->hasElement(sName)) {
        return false; // already exists
    }
    WsjcppYamlPlaceInFile pl;
    WsjcppYamlNode *pNewItem = new WsjcppYamlNode(this, m_pLog, pl, WSJCPP_YAML_NODE_MAP);
    pNewItem->setName(sName, nNameQuotes);
    // pNewItem->setNodeIndents({2});
    this->setElement(sName, pNewItem);
    return true;
}

// ---------------------------------------------------------------------

WsjcppYamlNode *WsjcppYamlNode::createElementMap() {
    if (m_nItemType != WSJCPP_YAML_NODE_ARRAY ) {
        throw std::runtime_error(TAG + ": createElementMap, Element must be 'array' for " + this->getPlaceInFile().getForLogFormat());
    }
    WsjcppYamlPlaceInFile pl;
    WsjcppYamlNode *pNewItem = new WsjcppYamlNode(this, m_pLog, pl, WSJCPP_YAML_NODE_MAP);
    this->appendElement(pNewItem);
    return pNewItem;
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::createElementArray(const std::string &sName, WsjcppYamlQuotes nNameQuotes) {
    if (m_nItemType != WSJCPP_YAML_NODE_MAP ) {
        throw std::runtime_error(TAG + ": createElementArray, Element must be 'map' for " + this->getPlaceInFile().getForLogFormat());
    }
    if (this->hasElement(sName)) {
        return false;
    }
    WsjcppYamlPlaceInFile pl;
    WsjcppYamlNode *pNewItem = new WsjcppYamlNode(this, m_pLog, pl, WSJCPP_YAML_NODE_ARRAY);
    pNewItem->setName(sName, nNameQuotes);
    this->setElement(sName, pNewItem);
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::isArray() {
    return m_nItemType == WSJCPP_YAML_NODE_ARRAY;
}

// ---------------------------------------------------------------------

int WsjcppYamlNode::getLength() {
    if (m_nItemType != WSJCPP_YAML_NODE_ARRAY) {
        throw std::runtime_error(TAG + ": getLength, Element must be array for " + this->getForLogFormat());
    }
    int nCount = 0;
    for (unsigned int i = 0; i < m_vObjects.size(); i++) {
        if (!m_vObjects[i]->isEmpty()) {
            nCount++;
        }
    }
    return nCount;
}

// ---------------------------------------------------------------------

WsjcppYamlNode *WsjcppYamlNode::getElement(int i) {
    if (m_nItemType != WSJCPP_YAML_NODE_ARRAY) {
        throw std::runtime_error(TAG + ": getElement, Element must be array");
    }
    int nCounter = -1;
    WsjcppYamlNode *pItem = WSJCPP_NULL;
    for (unsigned int n = 0; n < m_vObjects.size(); n++) {
        if (!m_vObjects[n]->isEmpty()) {
            nCounter++;
            if (nCounter == i) {
                pItem = m_vObjects[n];
                break; 
            }
        }
    }
    if (pItem == WSJCPP_NULL) {
        throw std::runtime_error(TAG + ": getElement(" + WSJCPP_INT_TO_STR(i) +  "), Out of range in array for '" + this->getPlaceInFile().getLine() + "'");
    }
    return pItem;
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::appendElement(WsjcppYamlNode *pNode) {
    if (pNode->isEmpty()) {
        m_vObjects.push_back(pNode); // TODO clone object
        return true;
    }
    if (m_nItemType != WSJCPP_YAML_NODE_ARRAY) {
        throw std::runtime_error(TAG + ": appendElement, "
            "tring add node \n"
            "    name='" + pNode->getName() + "'\n"
            "    type=" + pNode->getNodeTypeAsString() + "\n"
            "    line=" + WSJCPP_INT_TO_STR(pNode->getNumberOfLine()) + ")\n"
            " To element (must be array) \n" + this->getForLogFormat());
    }
    m_vObjects.push_back(pNode); // TODO clone object
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::appendElementValue(const std::string &sValue, WsjcppYamlQuotes nValueQuotes) {
    if (m_nItemType != WSJCPP_YAML_NODE_ARRAY) {
        throw std::runtime_error(TAG + ": appendElementValue, Element must be array for " + this->getForLogFormat());
    }
    WsjcppYamlPlaceInFile pl;
    WsjcppYamlNode *pNewItem = new WsjcppYamlNode(this, m_pLog, pl, WSJCPP_YAML_NODE_VALUE);
    pNewItem->setValue(sValue, nValueQuotes);
    return this->appendElement(pNewItem);
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::removeElement(int i) {
    if (m_nItemType != WSJCPP_YAML_NODE_ARRAY) {
        throw std::runtime_error(TAG + ": appendElement, Element must be array for " + this->getForLogFormat());
    }
    int nCounter = -1;
    WsjcppYamlNode *pItem = WSJCPP_NULL;
    for (unsigned int n = 0; n < m_vObjects.size(); n++) {
        if (!m_vObjects[n]->isEmpty()) {
            nCounter++;
            if (nCounter == i) {
                pItem = m_vObjects[n];
                break; 
            }
        }
    }
    if (pItem == WSJCPP_NULL) {
        throw std::runtime_error(TAG + ": getElement(" + WSJCPP_INT_TO_STR(i) +  "), Out of range in array for '" + this->getPlaceInFile().getLine() + "'");
    }
    std::vector<WsjcppYamlNode *>::iterator it;
    for (it = m_vObjects.begin(); it != m_vObjects.end(); ++it) {
        if (*it == pItem) {
            delete pItem;
            m_vObjects.erase(it);
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

bool WsjcppYamlNode::isValue() {
    return m_nItemType == WSJCPP_YAML_NODE_VALUE;
}

// ---------------------------------------------------------------------

std::string  WsjcppYamlNode::getValue() {
    if (m_nItemType != WSJCPP_YAML_NODE_VALUE) {
        throw std::runtime_error(TAG + ": getValue, Element must be value for " + this->getForLogFormat());
    }
    return m_sValue;
}

// ---------------------------------------------------------------------

void WsjcppYamlNode::setValue(const std::string &sValue, WsjcppYamlQuotes nQuotes) {
    if (m_nItemType != WSJCPP_YAML_NODE_VALUE) {
        throw std::runtime_error(TAG + ": setValue, Element must be value for " + this->getForLogFormat());
    }
    m_nValueQuotes = nQuotes;
    m_sValue = sValue;
}

// ---------------------------------------------------------------------

WsjcppYamlQuotes WsjcppYamlNode::getValueQuotes() {
    return m_nValueQuotes;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlNode::getSerializedName() {
    std::string sRet = "";
    // TODO escape quotes
    if (this->getNameQuotes() == WSJCPP_YAML_QUOTES_DOUBLE) {
        sRet += "\"" + this->getName() + "\"";
    } else if (this->getNameQuotes() == WSJCPP_YAML_QUOTES_SINGLE) {
        sRet += "\'" + this->getName() + "\'";
    } else {
        sRet += this->getName();
    }
    return sRet;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlNode::toString(std::string sIndent) {
    std::string sRet = "";
    if (this->isValue()) {
        if (m_nValueQuotes == WSJCPP_YAML_QUOTES_DOUBLE) {
            sRet = "\"" + m_sValue + "\"";
        } else if (m_nValueQuotes == WSJCPP_YAML_QUOTES_SINGLE) {
            sRet = "\'" + m_sValue + "\'";
        } else {
            sRet = m_sValue;
        }
        if (m_sComment.length() > 0) {
            if (sRet.length() > 0) {
                sRet += " ";
            }
            sRet += "# " + m_sComment;
        }
    } else if (this->isUndefined()) {
        for (unsigned int i = 0; i < m_vObjects.size(); i++) {
            if (m_vObjects[i]->isEmpty()) {
                sRet += "\n";
            } else {
                m_pLog->warn(TAG, "Undefined element contains something else");
            }
            // sRet += std::to_string(m_vObjects.size());
        }
        return sRet;
    } else if (this->isEmpty()) {
        if (m_sComment.length() > 0) {
            sRet += sIndent + m_sNodeDiffIndent + "# " + m_sComment;
        }
        return sRet;
    } else if (this->isArray()) {
        for (unsigned int i = 0; i < m_vObjects.size(); i++) {
            WsjcppYamlNode *pNode = m_vObjects[i];
            if (pNode->isEmpty()) {
                // std::string sVal = pNode->toString(sIndent + pNode->getStringNodeLastIndent());
                std::string sVal = pNode->toString(sIndent);
                sRet += sVal;
            } else if (pNode->isMap()) {
                sRet += sIndent + pNode->getStringNodeLastIndent();
                std::string s = pNode->toString(sIndent + pNode->getStringNodeLastIndent());
                WsjcppYaml::ltrim(s);
                sRet += "- " + s;
            } else {
                sRet += sIndent + pNode->getStringNodeLastIndent();
                sRet += "- " + pNode->toString();
            }
            sRet += "\n";
        }
        removeLastCharNewLine(sRet);
    } else if (this->isMap()) {
        if (m_vObjects.size() > 0) {
            // sRet += "-s-" + std::to_string(m_vObjects.size()) + "--";
            for (unsigned int i = 0; i < m_vObjects.size(); i++) {
                WsjcppYamlNode *pNode = m_vObjects[i];
                if (pNode->isEmpty() ) {
                    std::string s = pNode->toString(sIndent);
                    // WsjcppLog::warn(TAG, "Empty");
                    // WsjcppYaml::trim(s);
                    // sRet += s + " # rmpty";
                    sRet += s;
                } else if (pNode->isUndefined()) {
                    std::string s = pNode->toString();
                    sRet += sIndent + pNode->getStringNodeLastIndent()
                        + pNode->getSerializedName() + ":";
                    if (pNode->hasObjects()) {
                        sRet += "\n" + pNode->toString();
                        removeLastCharNewLine(sRet);
                    }
                } else if (pNode->isArray() || pNode->isMap()) {
                    sRet += sIndent + pNode->getStringNodeLastIndent()
                    + pNode->getSerializedName() + ":";
                    if (pNode->getComment().length() > 0) {
                        sRet += " # " + pNode->getComment(); 
                    }
                    std::string s = pNode->toString(sIndent + pNode->getStringNodeLastIndent());
                    if (pNode->isMap()) {
                        if (pNode->getKeys().size() > 0) {
                            sRet += "\n";
                        } else {
                            // removeLastCharNewLine(s);
                        }
                    }
                    if (pNode->isArray()) {
                        if (pNode->getLength() > 0) {
                            sRet += "\n";
                        } else {
                            // removeLastCharNewLine(s);
                        }
                    }
                    sRet += s;
                } else {
                    std::string sVal = pNode->toString();
                    std::string sVal_ = sVal;
                    sVal_ = WsjcppYaml::trim(sVal_);
                    if (sVal_.length() > 0) {
                        sVal = " " + sVal;
                    }
                    WsjcppYaml::rtrim(sVal);
                    sRet += sIndent + pNode->getStringNodeLastIndent()
                        + pNode->getSerializedName() + ":" + sVal;
                }
                sRet += "\n";
            }
            removeLastCharNewLine(sRet);
        }
    } else {
        m_pLog->warn(TAG, "????");
        sRet = ""; // undefined element must be empty
    }

    if (m_pParent == WSJCPP_NULL) {
        removeLastCharNewLine(sRet);
    }
    return sRet;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlNode::getNodeTypeAsString() {
    if (m_nItemType == WSJCPP_YAML_NODE_UNDEFINED) {
        return "undefined";
    } else if (m_nItemType == WSJCPP_YAML_NODE_ARRAY) {
        return "array";
    } else if (m_nItemType == WSJCPP_YAML_NODE_MAP) {
        return "map";
    } else if (m_nItemType == WSJCPP_YAML_NODE_VALUE) {
        return "value";
    }
    return "unknown";
}

// ---------------------------------------------------------------------

std::string WsjcppYamlNode::getForLogFormat() {
    return m_placeInFile.getForLogFormat();
}

// ---------------------------------------------------------------------

int WsjcppYamlNode::getNodeLastIndent() {
    return m_nNodeDiffIndent;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlNode::getStringNodeLastIndent() {
    return m_sNodeDiffIndent;
}

// ---------------------------------------------------------------------

void WsjcppYamlNode::setNodeIndents(const std::vector<int> & vNodeIndents) {
    m_nNodeDiffIndent = vNodeIndents.back();
    m_sNodeDiffIndent = "";
    for (int i = 0; i < m_nNodeDiffIndent; i++) {
        m_sNodeDiffIndent += " ";
    }
    m_nNodeIndent = 0;
    for (unsigned int i = 0; i < vNodeIndents.size(); i++) {
        m_nNodeIndent += vNodeIndents[i];
    }
}

// ---------------------------------------------------------------------

int WsjcppYamlNode::getNodeIndent() {
    return m_nNodeIndent;
}

// ---------------------------------------------------------------------

int WsjcppYamlNode::getNumberOfLine() const {
    return m_placeInFile.getNumberOfLine();
}

void WsjcppYamlNode::setNumberOfLine(int nNumberOfLine) {
    m_placeInFile.setNumberOfLine(nNumberOfLine);
}

void WsjcppYamlNode::throw_error(const std::string &sError) {
    throw std::runtime_error(sError.c_str());
}

void WsjcppYamlNode::removeLastCharNewLine(std::string &sLine) {
    int nLen = sLine.length();
    if (nLen > 0 && sLine[nLen - 1] == '\n') {
        sLine = sLine.substr(0, nLen - 1);
    }
}

bool WsjcppYamlNode::hasContent(const std::string &sVal) {
    std::string sVal_ = sVal;
    sVal_ = WsjcppYaml::trim(sVal_);
    return sVal_.length() > 0;
}

bool WsjcppYamlNode::hasObjects() {
    return m_vObjects.size() > 0;
}

// ---------------------------------------------------------------------
// WsjcppYamlParsebleLine

WsjcppYamlParsebleLine::WsjcppYamlParsebleLine(int nLine) {
    initInstance(nLine);
}

WsjcppYamlParsebleLine::WsjcppYamlParsebleLine() {
    initInstance(-1);
}

void WsjcppYamlParsebleLine::initInstance(int nLine) {
    TAG = "WsjcppYamlParsebleLine(line:" + WSJCPP_INT_TO_STR(nLine) + ")";
    m_nLineNumber = nLine;
    m_sPrefix = "";
    m_bArrayItem = false;
    m_sComment = "";
    m_sTagName = "";
    m_sValue = "";
    m_nNameQuotes = WSJCPP_YAML_QUOTES_NONE;
    m_nValueQuotes = WSJCPP_YAML_QUOTES_NONE;
    m_bHasComment = false;
    m_bEmptyLine = false;
}

int WsjcppYamlParsebleLine::getLineNumber() {
    return m_nLineNumber;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlParsebleLine::getPrefix() {
    return m_sPrefix;
}

// ---------------------------------------------------------------------

int WsjcppYamlParsebleLine::getIndent() {
    return m_sPrefix.length();
}

// ---------------------------------------------------------------------

bool WsjcppYamlParsebleLine::isArrayItem() {
    return m_bArrayItem;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlParsebleLine::getComment() {
    return m_sComment;
}

// ---------------------------------------------------------------------

bool WsjcppYamlParsebleLine::hasComment() {
    return m_bHasComment;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlParsebleLine::getName() {
    return m_sTagName;
}

// ---------------------------------------------------------------------

WsjcppYamlQuotes WsjcppYamlParsebleLine::getNameQuotes() {
    return m_nNameQuotes;
}

// ---------------------------------------------------------------------

bool WsjcppYamlParsebleLine::isEmptyName() {
    return m_sTagName.length() == 0;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlParsebleLine::getValue() {
    return m_sValue;
}

// ---------------------------------------------------------------------

WsjcppYamlQuotes WsjcppYamlParsebleLine::getValueQuotes() {
    return m_nValueQuotes;
}

// ---------------------------------------------------------------------

bool WsjcppYamlParsebleLine::isEmptyValue() {
    return m_sValue.length() == 0;
}

// ---------------------------------------------------------------------

bool WsjcppYamlParsebleLine::isEmptyLine() {
    return m_bEmptyLine;
}

// ---------------------------------------------------------------------

enum WsjcppYamlParserLineStates {
    WSJCPP_YAML_PARSER_LINE_STATE_NO,
    WSJCPP_YAML_PARSER_LINE_STATE_VALUE,
    WSJCPP_YAML_PARSER_LINE_STATE_COMMENT,
    WSJCPP_YAML_PARSER_LINE_STATE_STRING_DOUBLE_QUOTES,
    WSJCPP_YAML_PARSER_LINE_STATE_STRING_SINGLE_QUOTES,
    WSJCPP_YAML_PARSER_LINE_STATE_ESCAPING_DOUBLE_QUOTES,
    WSJCPP_YAML_PARSER_LINE_STATE_ESCAPING_SINGLE_QUOTES
};

bool WsjcppYamlParsebleLine::parseLine(const std::string &sLine, std::string &sError) {
    // reset variables
    m_bArrayItem = false;
    m_sPrefix = "";
    m_sComment = "";
    m_sTagName = "";
    m_sValue = "";
    m_bHasComment = false;
    m_nNameQuotes = WSJCPP_YAML_QUOTES_NONE;
    m_nValueQuotes = WSJCPP_YAML_QUOTES_NONE;
    m_bEmptyLine = false;
    std::string sLineTrim = sLine;
    sLineTrim = WsjcppYaml::trim(sLineTrim);
    if (sLineTrim.length() == 0) {
        m_bEmptyLine = true;
        return true;
    }

    WsjcppYamlParserLineStates state = WSJCPP_YAML_PARSER_LINE_STATE_NO;
    for (unsigned int i = 0; i < sLine.length(); i++) {
        char c = sLine[i];
        if ((c == ' ' || c == '\t') && state == WSJCPP_YAML_PARSER_LINE_STATE_NO) {
            m_sPrefix += c;
        } else if (c == '#' && (state == WSJCPP_YAML_PARSER_LINE_STATE_NO || state == WSJCPP_YAML_PARSER_LINE_STATE_VALUE)) {
            state = WSJCPP_YAML_PARSER_LINE_STATE_COMMENT;
            m_bHasComment = true;
        } else if (state == WSJCPP_YAML_PARSER_LINE_STATE_COMMENT) {
            if (c != '\r') {
                m_sComment += c;
            }
        } else if (c == '-' && state == WSJCPP_YAML_PARSER_LINE_STATE_NO) {
            m_bArrayItem = true;
            state = WSJCPP_YAML_PARSER_LINE_STATE_VALUE;
        } else if ((c != ' ' && c != '\t') && state == WSJCPP_YAML_PARSER_LINE_STATE_NO) {
            state = WSJCPP_YAML_PARSER_LINE_STATE_VALUE;
            m_sValue += c;
            if (c == '"') {
                state = WSJCPP_YAML_PARSER_LINE_STATE_STRING_DOUBLE_QUOTES;
            }
            if (c == '\'') {
                state = WSJCPP_YAML_PARSER_LINE_STATE_STRING_SINGLE_QUOTES;
            }
        } else if (c == '"' && state == WSJCPP_YAML_PARSER_LINE_STATE_VALUE) {
            state = WSJCPP_YAML_PARSER_LINE_STATE_STRING_DOUBLE_QUOTES;
            m_sValue += c;
        } else if (c == '\'' && state == WSJCPP_YAML_PARSER_LINE_STATE_VALUE) {
            state = WSJCPP_YAML_PARSER_LINE_STATE_STRING_SINGLE_QUOTES;
            m_sValue += c;
        } else if (c == '\\' && state == WSJCPP_YAML_PARSER_LINE_STATE_STRING_DOUBLE_QUOTES) {
            state = WSJCPP_YAML_PARSER_LINE_STATE_ESCAPING_DOUBLE_QUOTES;
            m_sValue += c;
        } else if (c == '\\' && state == WSJCPP_YAML_PARSER_LINE_STATE_STRING_SINGLE_QUOTES) {
            state = WSJCPP_YAML_PARSER_LINE_STATE_ESCAPING_SINGLE_QUOTES;
            m_sValue += c;
        } else if (state == WSJCPP_YAML_PARSER_LINE_STATE_ESCAPING_DOUBLE_QUOTES) {
            state = WSJCPP_YAML_PARSER_LINE_STATE_STRING_DOUBLE_QUOTES;
            m_sValue += c;
        } else if (state == WSJCPP_YAML_PARSER_LINE_STATE_ESCAPING_SINGLE_QUOTES) {
            state = WSJCPP_YAML_PARSER_LINE_STATE_STRING_SINGLE_QUOTES;
            m_sValue += c;
        } else if (c == '"' && state == WSJCPP_YAML_PARSER_LINE_STATE_STRING_DOUBLE_QUOTES) {
            state = WSJCPP_YAML_PARSER_LINE_STATE_VALUE;
            m_sValue += c;
        } else if (c == '\'' && state == WSJCPP_YAML_PARSER_LINE_STATE_STRING_SINGLE_QUOTES) {
            state = WSJCPP_YAML_PARSER_LINE_STATE_VALUE;
            m_sValue += c;
        } else if (c == ':' && state == WSJCPP_YAML_PARSER_LINE_STATE_VALUE) {
            if (m_sTagName.length() == 0 && this->canTagName(m_sValue)) {
                m_sTagName = m_sValue;
                m_sValue = ""; // reset value it was param name
            } else {
                m_sValue += c;
            }
        } else if (
            state == WSJCPP_YAML_PARSER_LINE_STATE_STRING_DOUBLE_QUOTES
            || state == WSJCPP_YAML_PARSER_LINE_STATE_STRING_SINGLE_QUOTES
        ) {
            m_sValue += c;
        } else if (state == WSJCPP_YAML_PARSER_LINE_STATE_VALUE) {
            m_sValue += c;
        } else {
            // skip
        }
    }

    if (
        state == WSJCPP_YAML_PARSER_LINE_STATE_STRING_DOUBLE_QUOTES
        || state == WSJCPP_YAML_PARSER_LINE_STATE_STRING_SINGLE_QUOTES
        || state == WSJCPP_YAML_PARSER_LINE_STATE_ESCAPING_DOUBLE_QUOTES
        || state == WSJCPP_YAML_PARSER_LINE_STATE_ESCAPING_SINGLE_QUOTES
    ) {
        sError = "Line has wrong format.";
        return false;
    }

    // split name and value
    /*for (int i = 0; i < m_sNameAndValue.size(); i++) {
        char c = m_sNameAndValue[i];
        if (c == ':' && m_sName.size() == 0) {
            m_sName = m_sValue;
            m_sValue = "";
        } else {
            m_sValue += c;
        }
    }*/

    m_sTagName = WsjcppYaml::trim(m_sTagName);
    if (m_sTagName.length() > 0 && m_sTagName[0] == '"') {
        m_nNameQuotes = WSJCPP_YAML_QUOTES_DOUBLE;
        m_sTagName = removeStringDoubleQuotes(m_sTagName);
    }
    if (m_sTagName.length() > 0 && m_sTagName[0] == '\'') {
        m_nNameQuotes = WSJCPP_YAML_QUOTES_SINGLE;
        m_sTagName = removeStringSingleQuotes(m_sTagName);
    }

    m_sValue = WsjcppYaml::trim(m_sValue);
    if (m_sValue.length() > 0 && m_sValue[0] == '"') {
        m_nValueQuotes = WSJCPP_YAML_QUOTES_DOUBLE;
        m_sValue = removeStringDoubleQuotes(m_sValue);
    }
    if (m_sValue.length() > 0 && m_sValue[0] == '\'') {
        m_nValueQuotes = WSJCPP_YAML_QUOTES_SINGLE;
        m_sValue = removeStringSingleQuotes(m_sValue);
    }

    m_sComment = WsjcppYaml::trim(m_sComment);

    if (m_bArrayItem == false && m_sTagName.length() == 0 && m_sValue.length() > 0 ) {
        sError = "Value of name can be empty only for array-item (line: " + sLine + ")";
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppYamlParsebleLine::canTagName(const std::string &sVal) {
    std::string sTrim = sVal;
    sTrim = WsjcppYaml::trim(sTrim);
    int nLen = sTrim.length();
    if (nLen == 0) {
        return false;
    }
    if (sTrim.length() > 0 && sTrim[0] == '"' && sTrim[nLen-1] == '"') {
        return true;
    }
    if (sTrim.length() > 0 && sTrim[0] == '\'' && sTrim[nLen-1] == '\'') {
        return true;
    }
    // check illegal char
    for (int i = 0; i < nLen; i++) {
        char c = sTrim[i];
        if (
            c != '-' && c != '_'
            && (c < '0' || c > '9')
            && (c < 'a' || c > 'z')
            && (c < 'A' || c > 'Z')
        ) {
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlParsebleLine::removeStringDoubleQuotes(const std::string &sValue) {
    if (sValue.size() > 0 && sValue[0] != '"') {
        return sValue;
    }
    int nStartPos = 1;
    int nEndPos = sValue.size()-1;
    std::string sRet = "";
    bool bEscape = false;
    for (int i = nStartPos; i < nEndPos; i++) {
        char c = sValue[i];
        if (bEscape) {
            if (c == 'n') {
                sRet += '\n';
            } else if (c == 'r') {
                sRet += '\r';
            } else {
                sRet += c;
            }
        } else if (c == '\\') {
            bEscape = true;
        } else {
            sRet += c;
        }
    }
    return sRet;
}


// ---------------------------------------------------------------------

std::string WsjcppYamlParsebleLine::removeStringSingleQuotes(const std::string &sValue) {
    if (sValue.size() > 0 && sValue[0] != '\'') {
        return sValue;
    }
    int nStartPos = 1;
    int nEndPos = sValue.size()-1;
    std::string sRet = "";
    bool bEscape = false;
    for (int i = nStartPos; i < nEndPos; i++) {
        char c = sValue[i];
        if (bEscape) {
            if (c == 'n') {
                sRet += '\n';
            } else if (c == 'r') {
                sRet += '\r';
            } else {
                sRet += c;
            }
        } else if (c == '\\') {
            bEscape = true;
        } else {
            sRet += c;
        }
    }
    return sRet;
}

// ---------------------------------------------------------------------
// WsjcppYamlCursor

WsjcppYamlCursor::WsjcppYamlCursor(WsjcppYamlNode *pCurrentNode) {
    initInstance(pCurrentNode);
}

WsjcppYamlCursor::WsjcppYamlCursor() {
    initInstance(WSJCPP_NULL);
}

WsjcppYamlCursor::~WsjcppYamlCursor() {
    // do nothing
}

void WsjcppYamlCursor::initInstance(WsjcppYamlNode *pCurrentNode) {
    m_pCurrentNode = pCurrentNode;
    TAG = "WsjcppYamlCursor";
}

bool WsjcppYamlCursor::isNull() const {
    return m_pCurrentNode == WSJCPP_NULL;
}

bool WsjcppYamlCursor::isUndefined() const {
    return m_pCurrentNode != WSJCPP_NULL && m_pCurrentNode->isUndefined();
}

bool WsjcppYamlCursor::isValue() const {
    return m_pCurrentNode != WSJCPP_NULL && m_pCurrentNode->isValue();
}

bool WsjcppYamlCursor::isArray() const {
    return m_pCurrentNode != WSJCPP_NULL && m_pCurrentNode->isArray();
}

size_t WsjcppYamlCursor::size() const {
    return m_pCurrentNode != WSJCPP_NULL && m_pCurrentNode->isArray() ? m_pCurrentNode->getLength() : -1;
}

// ---------------------------------------------------------------------

bool WsjcppYamlCursor::isMap() const {
    return m_pCurrentNode != WSJCPP_NULL && m_pCurrentNode->isMap();
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppYamlCursor::keys() const {
    return m_pCurrentNode != WSJCPP_NULL && m_pCurrentNode->isMap() ? m_pCurrentNode->getKeys() : std::vector<std::string>();
}

// ---------------------------------------------------------------------

bool WsjcppYamlCursor::hasKey(const std::string &sKey) const {
    return m_pCurrentNode != WSJCPP_NULL && m_pCurrentNode->isMap() && m_pCurrentNode->hasElement(sKey);
}

// ---------------------------------------------------------------------

// WsjcppYamlCursor &WsjcppYamlCursor::set(const std::string &sName, const std::string &sValue) {
//     return *this;
// }
//
// // ---------------------------------------------------------------------
//
// WsjcppYamlCursor &WsjcppYamlCursor::set(const std::string &sName, int nValue) {
//     return *this;
// }
//
// // ---------------------------------------------------------------------
//
// WsjcppYamlCursor &WsjcppYamlCursor::set(const std::string &sName, bool bValue) {
//     return *this;
// }
//
// // ---------------------------------------------------------------------
//
// WsjcppYamlCursor &WsjcppYamlCursor::remove(const std::string &sKey) {
//     return *this;
// }

// ---------------------------------------------------------------------

std::string WsjcppYamlCursor::comment() {
    return m_pCurrentNode != WSJCPP_NULL ? m_pCurrentNode->getComment() : std::string("");
}

// ---------------------------------------------------------------------

WsjcppYamlCursor &WsjcppYamlCursor::comment(const std::string& sComment) {
    if (m_pCurrentNode != WSJCPP_NULL) {
        m_pCurrentNode->setComment(sComment);
    }
    return *this;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlCursor::valStr() const {
    return m_pCurrentNode != WSJCPP_NULL ? m_pCurrentNode->getValue() : std::string("");
}

WsjcppYamlCursor &WsjcppYamlCursor::val(const std::string &sValue) {
    if (m_pCurrentNode != WSJCPP_NULL) {
        m_pCurrentNode->setValue(sValue); // TODO reserch need or not add quotes
    }
    return *this;
}

WsjcppYamlCursor &WsjcppYamlCursor::val(const char *sValue) {
    this->val(std::string(sValue));
    return *this;
}

int WsjcppYamlCursor::valInt() const {
    if (m_pCurrentNode != WSJCPP_NULL) {
        std::string sValue = m_pCurrentNode->getValue();
        sValue = WsjcppYaml::toLower(sValue);
        int nValue = std::strtol(sValue.c_str(), WSJCPP_NULL, 10);
        if (WSJCPP_INT_TO_STR(nValue) != sValue) {
            throw std::runtime_error(TAG + ": valInt, Element must be int but have a string" + m_pCurrentNode->getForLogFormat());
        }
        return nValue;
    }
    return 0;
}

float WsjcppYamlCursor::valFloat() const {
    if (m_pCurrentNode != WSJCPP_NULL) {
        std::string sValue = m_pCurrentNode->getValue();
        float nValue = std::stof(sValue);
        std::string sExpectedValue = WSJCPP_FLOAT_TO_STR(nValue);
        if (sExpectedValue != sValue) {
            std::string error_msg = TAG + ": valInt, Element must be float but have a string" + m_pCurrentNode->getForLogFormat() + "', but expected value is '" + sExpectedValue + "'";
            throw std::runtime_error(error_msg);
        }
        return nValue;
    }
    return 0.0f;
}

WsjcppYamlCursor &WsjcppYamlCursor::val(float nValue) {
    if (m_pCurrentNode != WSJCPP_NULL) {
        m_pCurrentNode->setValue(WSJCPP_FLOAT_TO_STR(nValue));
    }
    return *this;
}

double WsjcppYamlCursor::valDouble() const {
    if (m_pCurrentNode != WSJCPP_NULL) {
        std::string sValue = m_pCurrentNode->getValue();
        double nValue = std::stod(sValue);
        std::string sExpectedValue = WSJCPP_DOUBLE_TO_STR(nValue);
        if (sExpectedValue != sValue) {
            std::string error_msg = TAG + ": valInt, Element must be float but have a string '" + m_pCurrentNode->getForLogFormat() + "', but expected value is '" + sExpectedValue + "'";
            throw std::runtime_error(error_msg);
        }
        return nValue;
    }
    return 0.0f;
}

WsjcppYamlCursor &WsjcppYamlCursor::val(double nValue) {
    if (m_pCurrentNode != WSJCPP_NULL) {
        m_pCurrentNode->setValue(WSJCPP_DOUBLE_TO_STR(nValue));
    }
    return *this;
}

WsjcppYamlCursor &WsjcppYamlCursor::val(int nValue) {
    if (m_pCurrentNode != WSJCPP_NULL) {
        m_pCurrentNode->setValue(WSJCPP_INT_TO_STR(nValue));
    }
    return *this;
}

bool WsjcppYamlCursor::valBool() const {
    if (m_pCurrentNode != WSJCPP_NULL) {
        std::string sValue = m_pCurrentNode->getValue();
        sValue = WsjcppYaml::toLower(sValue);
        if (sValue == "yes" || sValue == "true") {
            return true;
        } else if (sValue == "no" || sValue == "false") {
            return false;
        } else {
            throw std::runtime_error(TAG + ": valBool, Element must be bool expected with ignore case like"
                " 'yes', 'no', 'true', 'false' for " + m_pCurrentNode->getForLogFormat());
        }
    }
    return false;
}

WsjcppYamlCursor &WsjcppYamlCursor::val(bool bValue) {
    if (m_pCurrentNode != WSJCPP_NULL) {
        m_pCurrentNode->setValue((bValue ? "yes" : "no"));
    }
    return *this;
}

WsjcppYamlNode *WsjcppYamlCursor::node() {
    return m_pCurrentNode;
}

WsjcppYamlCursor WsjcppYamlCursor::operator[](int idx) const {
    if (m_pCurrentNode != WSJCPP_NULL && m_pCurrentNode->isArray() && idx < m_pCurrentNode->getLength() && idx >= 0) {
        return WsjcppYamlCursor(m_pCurrentNode->getElement(idx));
    }
    return WsjcppYamlCursor();
}

WsjcppYamlCursor WsjcppYamlCursor::operator[](const std::string &sName) const {
    if (m_pCurrentNode != WSJCPP_NULL && m_pCurrentNode->isMap() && m_pCurrentNode->hasElement(sName)) {
        return WsjcppYamlCursor(m_pCurrentNode->getElement(sName));
    }
    return WsjcppYamlCursor();
}

WsjcppYamlCursor WsjcppYamlCursor::operator[](const char *pName) const {
    std::string sName(pName);
    if (m_pCurrentNode != WSJCPP_NULL && m_pCurrentNode->isMap() && m_pCurrentNode->hasElement(sName)) {
        return WsjcppYamlCursor(m_pCurrentNode->getElement(sName));
    }
    return WsjcppYamlCursor();
}

WsjcppYamlCursor& WsjcppYamlCursor::operator=(const char *sVal) {
    return this->val(std::string(sVal));
}

WsjcppYamlCursor& WsjcppYamlCursor::operator=(const std::string &sVal) {
    return this->val(sVal);
}

WsjcppYamlCursor& WsjcppYamlCursor::operator=(const int &nVal) {
    return this->val(nVal);
}

WsjcppYamlCursor& WsjcppYamlCursor::operator=(const bool &bVal) {
    return this->val(bVal);
}

// ---------------------------------------------------------------------
// WsjcppYaml

WsjcppYaml::WsjcppYaml() {
    m_pRoot = new WsjcppYamlNode(WSJCPP_NULL, this, WsjcppYamlPlaceInFile(), WSJCPP_YAML_NODE_MAP);
    TAG = "WsjcppYaml";
    m_pLog = WSJCPP_NULL;
}

WsjcppYaml::~WsjcppYaml() {
    delete m_pRoot;
}

void WsjcppYaml::setLogger(IWsjcppYamlLog *pLog) {
    m_pLog = pLog;
}

void WsjcppYaml::clear() {
    delete m_pRoot;
    m_pRoot = WSJCPP_NULL;
}

bool WsjcppYaml::loadFromFile(const std::string &sFileName, std::string &sError) {
    std::string sTextContent;
    if (!WsjcppYaml::readTextFile(sFileName, sTextContent, sError)) {
        return false;
    }
    return parse(sFileName, sTextContent, sError);
}

// ---------------------------------------------------------------------

bool WsjcppYaml::saveToFile(const std::string &sFileName, std::string &sError) {
    this->info(TAG, "Saving to " + sFileName);
    std::string sBuffer = m_pRoot->toString() + "\n"; // last empty line must be always
    if (!WsjcppYaml::writeFile(sFileName, sBuffer)) {
        sError = "Could not save to file";
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppYaml::loadFromString(const std::string &sBufferName, const std::string &sBuffer, std::string &sError) {
    return parse(sBufferName, sBuffer, sError);
}

// ---------------------------------------------------------------------

bool WsjcppYaml::saveToString(std::string &sBuffer, std::string &sError) {
    // WsjcppLog::warn(TAG, "WsjcppYaml::saveToString");
    sBuffer = m_pRoot->toString() + "\n"; // last empty line must be always
    return true;
}

// ---------------------------------------------------------------------

WsjcppYamlNode *WsjcppYaml::getRoot() {
    return m_pRoot;
}

// ---------------------------------------------------------------------

WsjcppYamlCursor WsjcppYaml::getCursor() const {
    return WsjcppYamlCursor(m_pRoot);
}

// ---------------------------------------------------------------------

WsjcppYamlCursor WsjcppYaml::operator[](int idx) const {
     return this->getCursor()[idx];
}

// ---------------------------------------------------------------------

WsjcppYamlCursor WsjcppYaml::operator[](const std::string &sName) const {
    return this->getCursor()[sName];
}

bool WsjcppYaml::readTextFile(const std::string &sFilename, std::string &sOutputContent, std::string &sError) {
    std::ifstream file_(sFilename.c_str());
    if (!file_) {
        sError = "FAILED could not open file to read " + sFilename;
        return false;
    }

    sOutputContent = std::string(
        (std::istreambuf_iterator<char>(file_)),
        std::istreambuf_iterator<char>()
    );

    return true;
}

bool WsjcppYaml::writeFile(const std::string &sFilename, const std::string &sContent) {

    // std::ofstream f(sFilename, std::ifstream::in);
    std::ofstream file_(sFilename.c_str(), std::ios::out);
    if (!file_) {
        std::cerr << "WsjcppYaml::writeFile" << "Could not create file to write '" << sFilename << "'" << std::endl;
        return false;
    }
    file_ << sContent << std::endl;
    file_.close();
    return true;
}

std::string& WsjcppYaml::ltrim(std::string& str, const std::string& chars) {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& WsjcppYaml::rtrim(std::string& str, const std::string& chars) {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& WsjcppYaml::trim(std::string& str, const std::string& chars) {
    return WsjcppYaml::ltrim(WsjcppYaml::rtrim(str, chars), chars);
}

std::string WsjcppYaml::toLower(const std::string &str) {
    // will worked only with latin
    std::string sRet = str;
    std::transform(sRet.begin(), sRet.end(), sRet.begin(), ::tolower);
    return sRet;
}

void WsjcppYaml::err(const std::string &TAG, const std::string &sMessage) {
    if (m_pLog) {
        m_pLog->err(TAG, sMessage);
    } else {
        std::cerr << TAG << " [error] : " << sMessage << std::endl;
    }
}

void WsjcppYaml::throw_err(const std::string &TAG, const std::string &sMessage) {
    if (m_pLog) {
        m_pLog->throw_err(TAG, sMessage);
    } else {
        std::cerr << TAG << " [critical_error] : " << sMessage << std::endl;
        throw std::runtime_error(TAG + " [critical_error] : " + sMessage);
    }
}

void WsjcppYaml::warn(const std::string &TAG, const std::string &sMessage) {
    if (m_pLog) {
        m_pLog->info(TAG, sMessage);
    } else {
        std::cerr << TAG << " [warn] : " << sMessage << std::endl;
    }
}

void WsjcppYaml::info(const std::string &TAG, const std::string &sMessage) {
    if (m_pLog) {
        m_pLog->info(TAG, sMessage);
    } else {
        std::cout << TAG << " [info] : " << sMessage << std::endl;
    }
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppYaml::splitToLines(const std::string &sBuffer) {
    std::vector<std::string> vLines;
    std::string sLine = "";
    int nSize = sBuffer.length();
    for (int i = 0; i < nSize; i++) {
        char c = sBuffer[i];
        if (c == '\n') {
            vLines.push_back(sLine);
            sLine = "";
        } else {
            sLine += c;
        }
    }
    if (sLine.length() > 0) {
        vLines.push_back(sLine);
    }
    return vLines;
}

// ---------------------------------------------------------------------

bool WsjcppYaml::parse(const std::string &sFileName, const std::string &sBuffer, std::string &sError) {
    this->clear();
    if (m_pRoot == WSJCPP_NULL) {
        m_pRoot = new WsjcppYamlNode(WSJCPP_NULL, this, WsjcppYamlPlaceInFile(), WSJCPP_YAML_NODE_MAP);
    }

    std::vector<std::string> vLines = this->splitToLines(sBuffer);
    m_pParseCurrentParentNode = m_pRoot;
    m_parsePlaceInFile.setFilename(sFileName);
    m_vStackDiffNodeIndents.clear();
    m_vStackDiffNodeIndents.push_back(0);
    m_nParseCurrentIndent = 0;
    m_pRoot->setPlaceInFile(m_parsePlaceInFile);

    for (unsigned int nLine = 0; nLine < vLines.size(); nLine++) {
        m_parsePlaceInFile.setLine(vLines[nLine]);
        m_parsePlaceInFile.setNumberOfLine(nLine);
        m_parseLine = WsjcppYamlParsebleLine(nLine);
        if (!m_parseLine.parseLine(m_parsePlaceInFile.getLine(), sError)) {
            return false;
        }

        // bool isEmptyName = m_parseLine.isEmptyName();
        bool isEmptyValue = m_parseLine.isEmptyValue();
        bool isArrayItem = m_parseLine.isArrayItem();
        int nLineIndent = m_parseLine.getIndent();
        int nDiffIndent = nLineIndent - m_nParseCurrentIndent;

        if (nDiffIndent > 0) {
            m_vStackDiffNodeIndents.push_back(nDiffIndent);
            m_nParseCurrentIndent = m_parseLine.getIndent();
        }

        if (m_parseLine.isEmptyLine()) {
            if (m_pParseCurrentParentNode != WSJCPP_NULL) {
                // WsjcppLog::warn(TAG, "here empty line and parent exists " + m_parsePlaceInFile.getForLogFormat());
                if (m_pParseCurrentParentNode->isArray() || m_pParseCurrentParentNode->isMap() || m_pParseCurrentParentNode->isUndefined()) {
                    // WsjcppLog::warn(TAG, "array, map or undefined");
                    WsjcppYamlNode *pNode = new WsjcppYamlNode(
                        m_pParseCurrentParentNode,
                        this,
                        m_parsePlaceInFile,
                        WSJCPP_YAML_NODE_EMPTY
                    );
                    pNode->setNodeIndents(m_vStackDiffNodeIndents);
                    m_pParseCurrentParentNode->appendElement(pNode);
                } else if (m_pParseCurrentParentNode->getParent() != WSJCPP_NULL && (m_pParseCurrentParentNode->getParent()->isArray() || m_pParseCurrentParentNode->getParent()->isMap())) {
                    // WsjcppLog::warn(TAG, "parent exists and parent map or array");
                    WsjcppYamlNode *pNode = new WsjcppYamlNode(
                        m_pParseCurrentParentNode->getParent(),
                        this,
                        m_parsePlaceInFile,
                        WSJCPP_YAML_NODE_EMPTY
                    );
                    pNode->setNodeIndents(m_vStackDiffNodeIndents);
                    m_pParseCurrentParentNode->getParent()->appendElement(pNode);
                } else {
                    throw std::runtime_error(TAG + ": Empty element can be added only to map or to array");
                }
            } else {
                this->warn(TAG, "here empty line" );
            }
            continue;
        }

        // fast switch to root
        if (nDiffIndent < 0 && m_parseLine.getIndent() == 0) {
            nDiffIndent = 0;
            m_nParseCurrentIndent = m_parseLine.getIndent();
            m_pParseCurrentParentNode = m_pRoot;
            m_vStackDiffNodeIndents.clear();
            m_vStackDiffNodeIndents.push_back(0);
        }

        // switch to parent
        while (nDiffIndent < 0 && m_nParseCurrentIndent != m_parseLine.getIndent()) {
            if (m_pParseCurrentParentNode == WSJCPP_NULL) {
                sError = "Current node is null, line: " + WSJCPP_INT_TO_STR(nLine);
                return false;
            }
            if (m_pParseCurrentParentNode->getParent() == WSJCPP_NULL) {
                sError = "Parent of current node is null, line: " + WSJCPP_INT_TO_STR(nLine);
                return false;
            }
            m_nParseCurrentIndent = m_nParseCurrentIndent - m_vStackDiffNodeIndents.back();
            m_vStackDiffNodeIndents.pop_back();
            m_pParseCurrentParentNode = m_pParseCurrentParentNode->getParent();
            if (m_nParseCurrentIndent < m_parseLine.getIndent()) {
                sError = "Wrong indent, expected "
                    "'" + WSJCPP_INT_TO_STR(m_parseLine.getIndent()) + "',"
                    " but got '" +
                    " in line: (" + sFileName + ":" + WSJCPP_INT_TO_STR(nLine) + ")";
                return false;
            }

            if (m_nParseCurrentIndent == m_parseLine.getIndent()) {
                break;
            }
        }

        if (m_parseLine.isEmptyName()) {
            if ( ! isEmptyValue && isArrayItem) {
                process_emptyName_hasValue_arrayItem();
            } else if (! isEmptyValue && ! isArrayItem) {
                process_emptyName_hasValue_noArrayItem();
            } else if (isEmptyValue && isArrayItem) {
                process_emptyName_emptyValue_arrayItem();
            } else if (isEmptyValue && ! isArrayItem) {
                process_emptyName_emptyValue_noArrayItem();
            } else {
                logUnknownParseLine();
            }
        } else if ( ! m_parseLine.isEmptyName()) {
            if ( ! isEmptyValue && isArrayItem) {
                process_hasName_hasValue_arrayItem();
            } else if ( ! isEmptyValue && ! isArrayItem) {
                process_hasName_hasValue_noArrayItem();
            } else if (isEmptyValue && isArrayItem) {
                process_hasName_emptyValue_arrayItem();
            } else if (isEmptyValue && ! isArrayItem) {
                process_hasName_emptyValue_noArrayItem();
            } else {
                logUnknownParseLine();
            }
        } else {
            logUnknownParseLine();
        }
    }
    return true;
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_hasName_emptyValue_arrayItem() {
    this->warn(TAG, "process_hasName_emptyValue_arrayItem");
    this->logUnknownParseLine();
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_hasName_emptyValue_noArrayItem() {
    // std::cout << "process_hasName_emptyValue_noArrayItem" << std::endl;
    if (m_parseLine.getIndent() == m_pParseCurrentParentNode->getNodeIndent()) {
        if (m_pParseCurrentParentNode->getParent() != WSJCPP_NULL) {
            m_pParseCurrentParentNode = m_pParseCurrentParentNode->getParent();
        }
    }
    WsjcppYamlNode *pNode = new WsjcppYamlNode(
        m_pParseCurrentParentNode,
        this,
        m_parsePlaceInFile,
        WSJCPP_YAML_NODE_UNDEFINED
    );
    if (m_parseLine.getValueQuotes() != WSJCPP_YAML_QUOTES_NONE) {
        // std::cout << "pNode->doValue() for '" << m_parseLine.getName() << "' value: '" << m_parseLine.getValue() << "' " << std::endl;
        pNode->doValue();
        pNode->setValue(m_parseLine.getValue(), m_parseLine.getValueQuotes());
    }
    // int nDiffIndent = m_parseLine.getIndent() - m_nParseCurrentIndent;
    pNode->setName(m_parseLine.getName(), m_parseLine.getNameQuotes());
    pNode->setComment(m_parseLine.getComment());
    pNode->setNodeIndents(m_vStackDiffNodeIndents);

    m_pParseCurrentParentNode->setElement(m_parseLine.getName(), pNode);
    if (pNode->isUndefined()) {
        m_pParseCurrentParentNode = pNode;
    }
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_hasName_hasValue_arrayItem() {
    // std::cout << "process_hasName_hasValue_arrayItem " << std::endl;
    if (m_pParseCurrentParentNode->isUndefined()) {
        m_pParseCurrentParentNode->doArray();
    }
    // if (!m_pParseCurrentParentNode->isArray()) {
    //     std::cout << "m_pParseCurrentParentNode->getName(): " << m_pParseCurrentParentNode->getName() << std::endl;
    // }

    WsjcppYamlNode *pMapItem = new WsjcppYamlNode(
        m_pParseCurrentParentNode,
        this,
        m_parsePlaceInFile,
        WSJCPP_YAML_NODE_MAP
    );
    // std::cout << "m_parseLine.getName(): " << m_parseLine.getName() << std::endl;

    m_pParseCurrentParentNode->appendElement(pMapItem);
    // std::cout << "appended " << std::endl;
    m_pParseCurrentParentNode = pMapItem;
    pMapItem->setNodeIndents(m_vStackDiffNodeIndents);

    WsjcppYamlNode *pNode = new WsjcppYamlNode(
        m_pParseCurrentParentNode,
        this,
        m_parsePlaceInFile,
        WSJCPP_YAML_NODE_VALUE
    );
    pNode->setComment(m_parseLine.getComment());
    pNode->setValue(m_parseLine.getValue(), m_parseLine.getValueQuotes());
    pNode->setName(m_parseLine.getName(), m_parseLine.getNameQuotes());
    pMapItem->setElement(m_parseLine.getName(), pNode);

    // next indents must be for map
    m_vStackDiffNodeIndents.push_back(2);
    m_nParseCurrentIndent += 2;
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_hasName_hasValue_noArrayItem() {
    // std::cout << "process_hasName_hasValue_noArrayItem" << std::endl;
    WsjcppYamlNode *pNode = new WsjcppYamlNode(
        m_pParseCurrentParentNode,
        this,
        m_parsePlaceInFile,
        WSJCPP_YAML_NODE_VALUE
    );
    // std::cout << "m_parseLine.getName(): " << m_parseLine.getName() << std::endl;
    // std::cout << "m_pParseCurrentParentNode: " << m_pParseCurrentParentNode->getPlaceInFile().getLine() << std::endl;
    pNode->setComment(m_parseLine.getComment());
    pNode->setValue(m_parseLine.getValue(), m_parseLine.getValueQuotes());
    pNode->setName(m_parseLine.getName(), m_parseLine.getNameQuotes());
    pNode->setNodeIndents(m_vStackDiffNodeIndents);
    m_pParseCurrentParentNode->setElement(m_parseLine.getName(), pNode);

    // m_pParseCurrentParentNode = pItem;
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_emptyName_hasValue_arrayItem() {
    // std::cout << "process_emptyName_hasValue_arrayItem " << std::endl;
    if (m_pParseCurrentParentNode->isUndefined()) {
        m_pParseCurrentParentNode->doArray();
    }
    WsjcppYamlNode *pNode = new WsjcppYamlNode(
        m_pParseCurrentParentNode,
        this,
        m_parsePlaceInFile,
        WSJCPP_YAML_NODE_VALUE
    );
    pNode->setComment(m_parseLine.getComment());
    pNode->setValue(m_parseLine.getValue(), m_parseLine.getValueQuotes());
    m_pParseCurrentParentNode->appendElement(pNode);
    // m_pParseCurrentParentNode = pNode;
    pNode->setNodeIndents(m_vStackDiffNodeIndents);
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_emptyName_hasValue_noArrayItem() {
    this->warn(TAG, "TODO process_emptyName_hasValue_noArrayItem");
    this->logUnknownParseLine();
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_emptyName_emptyValue_arrayItem() {
    // std::cout << "process_emptyName_emptyValue_arrayItem" << std::endl;
    if (m_pParseCurrentParentNode->isUndefined()) {
        m_pParseCurrentParentNode->doArray();
    }
    WsjcppYamlNode *pNode = new WsjcppYamlNode(
        m_pParseCurrentParentNode,
        this,
        m_parsePlaceInFile,
        WSJCPP_YAML_NODE_VALUE
    );
    pNode->setComment(m_parseLine.getComment());
    pNode->setValue(m_parseLine.getValue(), m_parseLine.getValueQuotes());
    pNode->setNodeIndents(m_vStackDiffNodeIndents);
    m_pParseCurrentParentNode->appendElement(pNode);
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_emptyName_emptyValue_noArrayItem() {
    // std::cout << "process_emptyName_emptyValue_noArrayItem " << std::endl;
    WsjcppYamlNode *pNode = new WsjcppYamlNode(
        m_pParseCurrentParentNode,
        this,
        m_parsePlaceInFile,
        WSJCPP_YAML_NODE_EMPTY
    );
    pNode->setComment(m_parseLine.getComment());
    pNode->setNodeIndents(m_vStackDiffNodeIndents);
    m_pParseCurrentParentNode->appendElement(pNode);
}

void WsjcppYaml::logUnknownParseLine() {
    this->warn(TAG, "\n"
        "  error:\n"
        "    desc: \"unknown_line\"\n"
        "    line_number: " + WSJCPP_INT_TO_STR(m_pParseCurrentParentNode->getPlaceInFile().getNumberOfLine()) + "\n"
        "    line: \"" + m_parsePlaceInFile.getLine() + "\"\n"
        "    indent: " + WSJCPP_INT_TO_STR(m_nParseCurrentIndent) +  "\n"
        "    filename: \"" + m_pParseCurrentParentNode->getPlaceInFile().getFilename() + "\""
    );
}
