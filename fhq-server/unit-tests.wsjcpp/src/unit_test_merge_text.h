#ifndef UNIT_TEST_MERGE_TEXT_H
#define UNIT_TEST_MERGE_TEXT_H

#include <wsjcpp_unit_tests.h>

class UnitTestMergeText : public WSJCppUnitTestBase {
    public:
        UnitTestMergeText();
        virtual void init();
        virtual bool run();
    private:
        std::string text001();
        std::string text002();
};

#endif // UNIT_TEST_MERGE_TEXT_H

