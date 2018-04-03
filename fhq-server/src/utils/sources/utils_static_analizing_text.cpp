#include <utils_static_analizing_text.h>

double UtilsStaticAnalizingText::calcWeightOfOneChar(int strLen){
  return 100.0/static_cast<double>(strLen);
}

QMap<QChar, double> UtilsStaticAnalizingText::calc(const QString &text){
  QMap<QChar, double> map;
  QString str = text;
  str = str.toLower();
  int strLen = str.length();
  double weightOfOneChar = calcWeightOfOneChar(strLen);
  for (int i = 0; i < strLen; i++) {
    map[str[i]] = 0.0;
  }
  for (int i = 0; i < strLen; i++) {
    map[str[i]] += weightOfOneChar;
  }
  return map;
}
