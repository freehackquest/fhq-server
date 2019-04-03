#ifndef UNIT_TEST_SHA1_H
#define UNIT_TEST_SHA1_H

#include <unit_tests.h>

class UnitTestSha1 : public UnitTestBase {
    public:
        UnitTestSha1();
        virtual void init();
        virtual bool run();
};

#endif // UNIT_TEST_SHA1_H
