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
	QMap<QChar, double> data;
	data['t'] = 50.0;
	data['e'] = 25.0;
	data['s'] = 25.0;
	tests.push_back(new LTest("test", data));

	unsigned int nSuccess = 0;
	for(unsigned int i = 0; i < tests.size(); i++){
			QString s1 = tests[i]->s1;
			QMap<QChar, double> n = tests[i]->n;
			QMap<QChar, double> n1 = UtilsStaticAnalizingText::calc(s1);
			// if (n1 != n) {
			// 		std::cout << "data static analizing for " << s1.toStdString() << "\n"
			// 							<< " (expected " << n << ", but got " << n1 << ")";
			// }else{
			// 		nSuccess++;
			// }
	}
	return nSuccess == tests.size();
}
