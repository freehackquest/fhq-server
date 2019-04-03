#ifndef UNIT_TEST_STORAGE_STRUCT_H
#define UNIT_TEST_STORAGE_STRUCT_H

#include <unit_tests.h>

class UnitTestStorageStruct : public UnitTestBase {
    public:
        UnitTestStorageStruct();
        virtual void init();
        virtual bool run();

    private:
        bool compareS(bool bTestSuccess, const std::string &sValue, const std::string &sExpected);
        bool compareN(const std::string &sPoint, int nValue, int nExpected);
};

#endif // UNIT_TEST_STORAGE_STRUCT_H
