#ifndef WSJCPP_UNIT_TESTS_H
#define WSJCPP_UNIT_TESTS_H

#include <map>
#include <vector>
#include <wsjcpp_core.h>

class WSJCppUnitTestBase {
    public:
        WSJCppUnitTestBase(const std::string &sTestName);
        std::string name();
        virtual void init() = 0;
        virtual bool run() = 0;
    protected:
        std::string TAG;

        void compareS(bool &bTestSuccess, const std::string &sPoint, const std::string &sValue, const std::string &sExpected);
        bool compareN(bool &bTestSuccess, const std::string &sPoint, int nValue, int nExpected);
        bool compareD(bool &bTestSuccess, const std::string &sPoint, double nValue, double nExpected);
        void compareB(bool &bTestSuccess, const std::string &sPoint, bool bValue, bool bExpected);

    private:
        std::string m_sTestName;
};

extern std::vector<WSJCppUnitTestBase*> *g_pUnitTests;

class WSJCppUnitTests {
    public:
        static void initGlobalVariables();
        static void addUnitTest(const std::string &sTestName, WSJCppUnitTestBase* pCmdHandler);
        static bool runUnitTests();
};

// RegistryCmdHandler
#define REGISTRY_UNIT_TEST( classname ) \
    static classname * pRegistryWSJCppUnitTest ## classname = new classname(); \


#endif // WSJCPP_UNIT_TESTS_H