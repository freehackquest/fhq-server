#ifndef UNIT_STATIC_ANALIZING_TEXT_H
#define UNIT_STATIC_ANALIZING_TEXT_H

#include <iunittest.h>

class UnitTestStaticAnalizingText : public IUnitTest {
public:
	virtual QString name();
	virtual void init();
	virtual bool run();
};

#endif // UNIT_TEST_LEVENSHTEIN_H
