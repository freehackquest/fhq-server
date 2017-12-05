#include <utils_static_analizing_text.h>

QMap<QChar, double> UtilsStaticAnalizingText::calc(const QString &text){
  QMap<QChar, double> map;
  QString str = text;
  str = str.toLower();
  str = str.remove(QRegExp("[^a-zA-Z\\d\\s]"));
  for (int i = 0; i < str.length(); i++) {
    map[str[i]] = 1.0;
  }
  qDebug() << str;
  return map;
}
