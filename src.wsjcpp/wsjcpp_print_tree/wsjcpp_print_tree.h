#ifndef WSJCPP_PRINT_TREE_H
#define WSJCPP_PRINT_TREE_H

#include <string>
#include <vector>

class WsjcppPrintNode {
    public:
        WsjcppPrintNode(WsjcppPrintNode *pParent, const std::string &sTitle);
        std::string getTitle();
        WsjcppPrintNode *getParent();
        std::vector<WsjcppPrintNode *> getChildsList();
        WsjcppPrintNode *getLastChild();
        WsjcppPrintNode *addChild(const std::string &sTitle);
        bool hasChilds();
    private:
        std::string TAG;
        WsjcppPrintNode *m_pParent;
        std::string m_sTitle;
        std::vector<WsjcppPrintNode *> m_vChilds;
};

// ----------------------------------------------------------------------

class WsjcppPrintTree {
    public:
        WsjcppPrintTree(const std::string &sTitle);
        WsjcppPrintNode *getRootNode();
        WsjcppPrintNode *getCurrentNode();
        WsjcppPrintTree &switchToLatestChild();
        WsjcppPrintTree &addChild(const std::string &sTitle);
        WsjcppPrintTree &switchToParent();
        std::string printTree();

    private:
        std::string TAG;
        WsjcppPrintNode *m_pRootNode;
        WsjcppPrintNode *m_pCurrentNode;

        std::string printRecoursive(const std::string &sPrefix, WsjcppPrintNode *pParentNode);
};

#endif // WSJCPP_PRINT_TREE_H