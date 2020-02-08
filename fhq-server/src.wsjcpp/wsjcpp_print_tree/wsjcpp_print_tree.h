#ifndef WSJCPP_PRINT_TREE_H
#define WSJCPP_PRINT_TREE_H

#include <string>
#include <vector>

class WSJCppPrintNode {
    public:
        WSJCppPrintNode(WSJCppPrintNode *pParent, const std::string &sTitle);
        std::string getTitle();
        WSJCppPrintNode *getParent();
        std::vector<WSJCppPrintNode *> getChildsList();
        WSJCppPrintNode *getLastChild();
        WSJCppPrintNode *addChild(const std::string &sTitle);
        bool hasChilds();
    private:
        std::string TAG;
        WSJCppPrintNode *m_pParent;
        std::string m_sTitle;
        std::vector<WSJCppPrintNode *> m_vChilds;
};

// ----------------------------------------------------------------------

class WSJCppPrintTree {
    public:
        WSJCppPrintTree(const std::string &sTitle);
        WSJCppPrintNode *getRootNode();
        WSJCppPrintNode *getCurrentNode();
        WSJCppPrintTree &switchToLatestChild();
        WSJCppPrintTree &addChild(const std::string &sTitle);
        WSJCppPrintTree &switchToParent();
        std::string printTree();

    private:
        std::string TAG;
        WSJCppPrintNode *m_pRootNode;
        WSJCppPrintNode *m_pCurrentNode;

        std::string printRecoursive(const std::string &sPrefix, WSJCppPrintNode *pParentNode);
};

#endif // WSJCPP_PRINT_TREE_H