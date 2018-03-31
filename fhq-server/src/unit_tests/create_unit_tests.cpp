#include <create_unit_tests.h>
#include <log.h>
#include <unit_test_levenshtein.h>
#include <unit_test_merge_text.h>
#include <unit_test_static_analizing_text.h>

void create_unit_tests(QMap<QString, IUnitTest *> &pUnitTests){
	QString TAG = "create_unit_tests";
	QVector<IUnitTest *> v;

	v.push_back(new UnitTestLevenshtein());
  v.push_back(new UnitTestMergeText);
  v.push_back(new UnitTestStaticAnalizingText);

	for(int i = 0; i < v.size(); i++){
		QString name = v[i]->name();
		if(pUnitTests.contains(name)){
			Log::warn(TAG, "unit_test for command " + name + " - already registered and will be skipped");
		}else{
			pUnitTests[name] = v[i];
		}
	}
}
