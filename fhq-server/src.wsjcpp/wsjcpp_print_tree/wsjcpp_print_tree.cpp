#include "wsjcpp_print_tree.h"
#include <wsjcpp_core.h>

WSJCppPrintNode::WSJCppPrintNode(WSJCppPrintNode *pParent, const std::string &sTitle) {
    TAG = "WSJCppPrintNode";
    m_pParent = pParent;
    m_sTitle = sTitle;
}

// ----------------------------------------------------------------------

std::string WSJCppPrintNode::getTitle() {
    return m_sTitle;
}

// ----------------------------------------------------------------------

WSJCppPrintNode *WSJCppPrintNode::getParent() {
    return m_pParent;
}

// ----------------------------------------------------------------------

std::vector<WSJCppPrintNode *> WSJCppPrintNode::getChildsList() {
    return m_vChilds;
}

// ----------------------------------------------------------------------

WSJCppPrintNode *WSJCppPrintNode::getLastChild() {
    if (m_vChilds.size() == 0) {
        return nullptr;
    }
    return m_vChilds[m_vChilds.size()-1];
}

// ----------------------------------------------------------------------

WSJCppPrintNode *WSJCppPrintNode::addChild(const std::string &sTitle) {
    WSJCppPrintNode *pNode = new WSJCppPrintNode(this, sTitle);
    m_vChilds.push_back(pNode);
    return pNode;
}

// ----------------------------------------------------------------------

bool WSJCppPrintNode::hasChilds() {
    return m_vChilds.size() > 0;
}

// ----------------------------------------------------------------------
// WSJCppPrintTree

WSJCppPrintTree::WSJCppPrintTree(const std::string &sTitle) {
    TAG = "WSJCppPrintTree";
    m_pRootNode = new WSJCppPrintNode(nullptr, sTitle);
    m_pCurrentNode = m_pRootNode;
}

// ----------------------------------------------------------------------

WSJCppPrintNode *WSJCppPrintTree::getRootNode() {
    return m_pRootNode;
}

// ----------------------------------------------------------------------

WSJCppPrintNode *WSJCppPrintTree::getCurrentNode() {
    return m_pCurrentNode;
}

// ----------------------------------------------------------------------

WSJCppPrintTree &WSJCppPrintTree::switchToLatestChild() {
    WSJCppPrintNode *pChild = m_pCurrentNode->getLastChild();
    if (pChild == nullptr) {
        WSJCppLog::throw_err(TAG, "Could not found last child");
    }
    m_pCurrentNode = pChild;
    return *this;
}

// ----------------------------------------------------------------------

WSJCppPrintTree &WSJCppPrintTree::addChild(const std::string &sTitle) {
    m_pCurrentNode->addChild(sTitle);
    return *this;
}

// ----------------------------------------------------------------------

WSJCppPrintTree &WSJCppPrintTree::switchToParent() {
    WSJCppPrintNode *pParent = m_pCurrentNode->getParent();
    if (pParent == nullptr) {
        WSJCppLog::throw_err(TAG, "Parent is null");
    }
    m_pCurrentNode = pParent;
    return *this;
}

// ----------------------------------------------------------------------

std::string WSJCppPrintTree::printTree() {
    return 
        m_pRootNode->getTitle() + "\n"
        + printRecoursive("", m_pRootNode);
}

// ----------------------------------------------------------------------

std::string WSJCppPrintTree::printRecoursive(const std::string &sPrefix, WSJCppPrintNode *pParentNode) {
    std::string sRet = "";
    std::vector<WSJCppPrintNode *>  v = pParentNode->getChildsList();
    int nLen = v.size();
    for (int i = 0; i < nLen; i++) {
        bool bLatestChild = (i == nLen-1);
        WSJCppPrintNode *pNode = v[i];
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