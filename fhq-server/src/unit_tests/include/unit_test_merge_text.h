#ifndef UNIT_TEST_MERGE_TEXT_H
#define UNIT_TEST_MERGE_TEXT_H

#include <iunittest.h>

class UnitTestMergeText : public IUnitTest {
public:
    virtual QString name();
    virtual void init();
    virtual bool run();
private:
    QString loadFileFromRes(QString res_name);
};

#endif // UNIT_TEST_MERGE_TEXT_H

