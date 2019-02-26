#ifndef UNIT_TESTS_H
#define UNIT_TESTS_H

#include <map>
#include <vector>
#include <utils_logger.h>

class UnitTestBase {
    public:
        UnitTestBase(const std::string &sTestName);
        std::string name();
        virtual void init() = 0;
        virtual bool run() = 0;
    protected:
        std::string TAG;
    private:
        std::string m_sTestName;
};

extern std::vector<UnitTestBase*> *g_pUnitTests;

class UnitTests {
    public:
        static void initGlobalVariables();
        static void addUnitTest(const std::string &sTestName, UnitTestBase* pCmdHandler);
        static bool runUnitTests();
};

// RegistryCmdHandler
#define REGISTRY_UNIT_TEST( classname ) \
    static classname * pRegistryUnitTest ## classname = new classname(); \


#endif // UNIT_TESTS_H
