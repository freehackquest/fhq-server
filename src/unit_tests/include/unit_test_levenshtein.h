#ifndef UNIT_TEST_LEVENSHTEIN_H
#define UNIT_TEST_LEVENSHTEIN_H

#include <iunittest.h>

class UnitTestLevenshtein : public IUnitTest {
public:
	virtual QString name();
	virtual void init();
	virtual bool run();
};

#endif // UNIT_TEST_LEVENSHTEIN_H
