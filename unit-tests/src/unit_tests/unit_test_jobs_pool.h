#ifndef UNIT_TEST_JOBS_POOL_H
#define UNIT_TEST_JOBS_POOL_H

#include <unit_tests.h>

class UnitTestJobsPool : public UnitTestBase {
    public:
        UnitTestJobsPool();
        virtual void init();
        virtual bool run();
};

#endif // UNIT_TEST_JOBS_POOL_H
