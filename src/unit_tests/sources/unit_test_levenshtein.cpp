#include <unit_test_levenshtein.h>
#include <utils_levenshtein.h>

QString UnitTestLevenshtein::name(){
	return "UnitTestLevenshtein";
}

void UnitTestLevenshtein::init(){
}

bool UnitTestLevenshtein::run(){
    if ((UtilsLevenshtein::distance("test", "test") == 0) && (UtilsLevenshtein::distance("tttt", "aaaa") == 4)) {
        return true;
    }
    else {
        return false;
    }
}

