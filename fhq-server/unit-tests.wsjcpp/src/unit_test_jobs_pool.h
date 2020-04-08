#ifndef UNIT_TEST_JOBS_POOL_H
#define UNIT_TEST_JOBS_POOL_H

#include <wsjcpp_unit_tests.h>

class UnitTestJobsPool : public WsjcppUnitTestBase {
    public:
        UnitTestJobsPool();
        virtual void init();
        virtual bool run();
};

#endif // UNIT_TEST_JOBS_POOL_H
