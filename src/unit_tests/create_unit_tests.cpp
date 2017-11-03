#include <create_unit_tests.h>
#include <log.h>
#include <unit_test_levenshtein.h>

void create_unit_tests(QMap<QString, IUnitTest *> &pUnitTests){
	QString TAG = "create_unit_tests";
	QVector<IUnitTest *> v;

	v.push_back(new UnitTestLevenshtein());

	for(int i = 0; i < v.size(); i++){
		QString name = v[i]->name();
		if(pUnitTests.contains(name)){
			Log::warn(TAG, "unit_test for command " + name + " - already registered and will be skipped");
		}else{
			pUnitTests[name] = v[i];
		}
	}
}
