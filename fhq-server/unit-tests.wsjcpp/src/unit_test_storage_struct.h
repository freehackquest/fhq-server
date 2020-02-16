#ifndef UNIT_TEST_STORAGE_STRUCT_H
#define UNIT_TEST_STORAGE_STRUCT_H

#include <wsjcpp_unit_tests.h>
#include <core/storages.h>

class UnitTestStorageStruct : public WSJCppUnitTestBase {
    public:
        UnitTestStorageStruct();
        virtual void init();
        virtual bool run();

    private:

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
