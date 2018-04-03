#ifndef IUNIT_TEST_H
#define IUNIT_TEST_H

#include <QString>

class IUnitTest {
	public:
		virtual QString name() = 0;
		virtual void init() = 0;
		virtual bool run() = 0;
};

#endif // IUNIT_TEST_H
