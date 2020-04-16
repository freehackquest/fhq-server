#ifndef UNIT_TEST_STORAGE_STRUCT_H
#define UNIT_TEST_STORAGE_STRUCT_H

#include <wsjcpp_unit_tests.h>
#include <wsjcpp_storages.h>

class UnitTestStorageStruct : public WsjcppUnitTestBase {
    public:
        UnitTestStorageStruct();
        virtual void init();
        virtual bool run();

    private:

        void createTestTable0(bool &bTestSuccess, WsjcppStorage *pStorage);
        void dropTestTable0(bool &bTestSuccess, WsjcppStorage *pStorage);
        void createTestTable1(bool &bTestSuccess, WsjcppStorage *pStorage);
        void modifyTestTable1(bool &bTestSuccess, WsjcppStorage *pStorage);
        void checkModifiedTable1(bool &bTestSuccess, WsjcppStorage *pStorage);
        void insertTestTable1(bool &bTestSuccess, WsjcppStorage *pStorage);
        void createTestTable3(bool &bTestSuccess, WsjcppStorage *pStorage);
        void modifyTestTable3(bool &bTestSuccess, WsjcppStorage *pStorage);

};

#endif // UNIT_TEST_STORAGE_STRUCT_H
