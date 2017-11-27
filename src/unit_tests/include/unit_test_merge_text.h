#ifndef UNIT_TEST_MERGE_TEXT_H
#define UNIT_TEST_MERGE_TEXT_H

#include <iunittest.h>

class UnitTestMergeText : public IUnitTest {
public:
    virtual QString name();
    virtual void init();
    virtual bool run();
};

#endif // UNIT_TEST_MERGE_TEXT_H

