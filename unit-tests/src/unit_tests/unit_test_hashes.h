#ifndef UNIT_TEST_MD5_H
#define UNIT_TEST_MD5_H

#include <unit_tests.h>

class UnitTestHashes : public UnitTestBase {
    public:
        UnitTestHashes();
        virtual void init();
        virtual bool run();
};

#endif // UNIT_TEST_MD5_H
