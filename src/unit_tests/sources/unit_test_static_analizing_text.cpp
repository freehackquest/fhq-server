#include <unit_test_static_analizing_text.h>
#include <utils_static_analizing_text.h>

QString UnitTestStaticAnalizingText::name(){
	return "UnitTestStaticAnalizingText";
}

void UnitTestStaticAnalizingText::init(){
}

bool UnitTestStaticAnalizingText::run(){

	struct LTest {
			LTest(QString s1, QMap<QChar, double> n) : s1(s1), n(n) {}
			QString s1;
			QMap<QChar, double> n;
	};

	std::vector<LTest *> tests;
	QMap<QChar, double> firstCase;
	firstCase['t'] = 50.0;
	firstCase['e'] = 25.0;
	firstCase['s'] = 25.0;
	tests.push_back(new LTest("test", firstCase));

	QMap<QChar, double> secondCase;
	secondCase['1'] = 50.0;
	secondCase['2'] = 50.0;
	tests.push_back(new LTest("12", secondCase));

	QMap<QChar, double> thirdCase;
	thirdCase['.'] = 50.0;
	thirdCase['-'] = 50.0;
	tests.push_back(new LTest(".--.", thirdCase));

	unsigned int nSuccess = 0;
	for(unsigned int i = 0; i < tests.size(); i++){
			QString s1 = tests[i]->s1;
			QMap<QChar, double> n = tests[i]->n;
			QMap<QChar, double> n1 = UtilsStaticAnalizingText::calc(s1);
			if (n1 != n) {
					std::cout << "Wrong\n";
			}else{
					nSuccess++;
			}
	}
	return nSuccess == tests.size();
}
