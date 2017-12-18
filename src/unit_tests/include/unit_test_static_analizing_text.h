#ifndef UNIT_TEST_STATIC_ANALIZING_TEXT_H
#define UNIT_TEST_STATIC_ANALIZING_TEXT_H

#include <iunittest.h>
#include <vector>
#include <iostream>
#include <QString>
#include <QMap>
#include <QChar>

class UnitTestStaticAnalizingText : public IUnitTest {
	public:
		virtual QString name();
		virtual void init();
		virtual bool run();
};

#endif // UNIT_TEST_STATIC_ANALIZING_TEXT_H
