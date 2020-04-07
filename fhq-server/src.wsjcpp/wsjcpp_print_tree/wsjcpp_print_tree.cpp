#include "wsjcpp_print_tree.h"
#include <wsjcpp_core.h>

WsjcppPrintNode::WsjcppPrintNode(WsjcppPrintNode *pParent, const std::string &sTitle) {
    TAG = "WsjcppPrintNode";
    m_pParent = pParent;
    m_sTitle = sTitle;
}

// ----------------------------------------------------------------------

std::string WsjcppPrintNode::getTitle() {
    return m_sTitle;
}

// ----------------------------------------------------------------------

WsjcppPrintNode *WsjcppPrintNode::getParent() {
    return m_pParent;
}

// ----------------------------------------------------------------------

std::vector<WsjcppPrintNode *> WsjcppPrintNode::getChildsList() {
    return m_vChilds;
}

// ----------------------------------------------------------------------

WsjcppPrintNode *WsjcppPrintNode::getLastChild() {
    if (m_vChilds.size() == 0) {
        return nullptr;
    }
    return m_vChilds[m_vChilds.size()-1];
}

// ----------------------------------------------------------------------

WsjcppPrintNode *WsjcppPrintNode::addChild(const std::string &sTitle) {
    WsjcppPrintNode *pNode = new WsjcppPrintNode(this, sTitle);
    m_vChilds.push_back(pNode);
    return pNode;
}

// ----------------------------------------------------------------------

bool WsjcppPrintNode::hasChilds() {
    return m_vChilds.size() > 0;
}

// ----------------------------------------------------------------------
// WsjcppPrintTree

WsjcppPrintTree::WsjcppPrintTree(const std::string &sTitle) {
    TAG = "WsjcppPrintTree";
    m_pRootNode = new WsjcppPrintNode(nullptr, sTitle);
    m_pCurrentNode = m_pRootNode;
}

// ----------------------------------------------------------------------

WsjcppPrintNode *WsjcppPrintTree::getRootNode() {
    return m_pRootNode;
}

// ----------------------------------------------------------------------

WsjcppPrintNode *WsjcppPrintTree::getCurrentNode() {
    return m_pCurrentNode;
}

// ----------------------------------------------------------------------

WsjcppPrintTree &WsjcppPrintTree::switchToLatestChild() {
    WsjcppPrintNode *pChild = m_pCurrentNode->getLastChild();
    if (pChild == nullptr) {
        WsjcppLog::throw_err(TAG, "Could not found last child");
    }
    m_pCurrentNode = pChild;
    return *this;
}

// ----------------------------------------------------------------------

WsjcppPrintTree &WsjcppPrintTree::addChild(const std::string &sTitle) {
    m_pCurrentNode->addChild(sTitle);
    return *this;
}

// ----------------------------------------------------------------------

WsjcppPrintTree &WsjcppPrintTree::switchToParent() {
    WsjcppPrintNode *pParent = m_pCurrentNode->getParent();
    if (pParent == nullptr) {
        WsjcppLog::throw_err(TAG, "Parent is null");
    }
    m_pCurrentNode = pParent;
    return *this;
}

// ----------------------------------------------------------------------

std::string WsjcppPrintTree::printTree() {
    return 
        m_pRootNode->getTitle() + "\n"
        + printRecoursive("", m_pRootNode);
}

// ----------------------------------------------------------------------

std::string WsjcppPrintTree::printRecoursive(const std::string &sPrefix, WsjcppPrintNode *pParentNode) {
    std::string sRet = "";
    std::vector<WsjcppPrintNode *>  v = pParentNode->getChildsList();
    int nLen = v.size();
    for (int i = 0; i < nLen; i++) {
        bool bLatestChild = (i == nLen-1);
        WsjcppPrintNode *pNode = v[i];
        sRet += sPrefix;
        sRet += bLatestChild ? "└─ " : "├─ ";
        sRet += pNode->getTitle() + "\n";
        if (pNode->hasChilds()) {
            std::string sPrefixAppend = bLatestChild ? "   " : "│  ";
            sRet += printRecoursive(sPrefix + sPrefixAppend, pNode);
        }
    }
    return sRet;
}

// ----------------------------------------------------------------------