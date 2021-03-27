#ifndef WSJCPP_UNIT_TESTS_H
#define WSJCPP_UNIT_TESTS_H

#include <wsjcpp_core.h>
#include <sstream>

class WsjcppUnitTestBase {
    public:
        WsjcppUnitTestBase(const std::string &sTestName);
        std::string getName();
        void ok(const std::string &sSuccessMessage);
        void fail(const std::string &sFailedMessage);
        bool runTest();

        virtual bool doBeforeTest() = 0;
        virtual void executeTest() = 0;
        virtual bool doAfterTest() = 0;
    protected:
        std::string TAG;
        
        bool compareD(const std::string &sMark, double nValue, double nExpected);
        template<typename T1, typename T2> bool compare(const std::string &sMark, T1 tGotValue, T2 tExpectedValue) {
            if (tGotValue != tExpectedValue) {
                std::stringstream ss;
                ss << " {mark: " << sMark << "} Expected '" << tExpectedValue << "', but got '" << tGotValue << "'";
                fail(ss.str());
                return false;
            }
            return true;
        }
    private:
        bool m_bTestResult;
        std::string m_sTestName;
};

extern std::vector<WsjcppUnitTestBase*> *g_pWsjcppUnitTests;

class WsjcppUnitTests {
    public:
        static void initGlobalVariables();
        static void addUnitTest(const std::string &sTestName, WsjcppUnitTestBase* pUnitTest);
};

// RegistryCmdHandler
#define REGISTRY_WSJCPP_UNIT_TEST( classname ) \
    static classname * pRegistryWsjcppUnitTest ## classname = new classname(); \


#endif // WSJCPP_UNIT_TESTS_H