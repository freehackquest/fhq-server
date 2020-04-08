#ifndef UNIT_TEST_VALIDATORS_H
#define UNIT_TEST_VALIDATORS_H

#include <wsjcpp_unit_tests.h>

class UnitTestValidators : public WsjcppUnitTestBase {
    public:
        UnitTestValidators();
        virtual void init();
        virtual bool run();
};

#endif // UNIT_TEST_VALIDATORS_H
