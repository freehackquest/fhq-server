#ifndef UNIT_TEST_MERGE_TEXT_H
#define UNIT_TEST_MERGE_TEXT_H

#include <unit_tests.h>

class UnitTestMergeText : public UnitTestBase {
    public:
        UnitTestMergeText();
        virtual void init();
        virtual bool run();
    private:
        QString loadFileFromRes(QString res_name);
};

#endif // UNIT_TEST_MERGE_TEXT_H

