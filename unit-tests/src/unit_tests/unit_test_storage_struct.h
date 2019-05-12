#ifndef UNIT_TEST_STORAGE_STRUCT_H
#define UNIT_TEST_STORAGE_STRUCT_H

#include <unit_tests.h>
#include <storages.h>

class UnitTestStorageStruct : public UnitTestBase {
    public:
        UnitTestStorageStruct();
        virtual void init();
        virtual bool run();

    private:
        void compareS(bool &bTestSuccess, const std::string &sPoint, const std::string &sValue, const std::string &sExpected);
        bool compareN(bool &bTestSuccess, const std::string &sPoint, int nValue, int nExpected);
        void compareB(bool &bTestSuccess, const std::string &sPoint, bool bValue, bool bExpected);

        void createTestTable0(bool &bTestSuccess, Storage *pStorage);
        void dropTestTable0(bool &bTestSuccess, Storage *pStorage);
        void createTestTable1(bool &bTestSuccess, Storage *pStorage);
        void modifyTestTable1(bool &bTestSuccess, Storage *pStorage);
        void checkModifiedTable1(bool &bTestSuccess, Storage *pStorage);
        void insertTestTable1(bool &bTestSuccess, Storage *pStorage);
        void createTestTable3(bool &bTestSuccess, Storage *pStorage);
        void modifyTestTable3(bool &bTestSuccess, Storage *pStorage);

};

#endif // UNIT_TEST_STORAGE_STRUCT_H
