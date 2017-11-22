#include <utils_static_analizing_text.h>

QMap<QChar, double> UtilsStaticAnalizingText::calc(const QString &text){
  QMap<QChar, double> map;
  map[QChar::Script_Unknown] = 1.0;

  return map;
}
