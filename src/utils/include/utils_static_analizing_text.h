#ifndef UTILS_STATIC_ANALIZING_TEXT_H
#define UTILS_STATIC_ANALIZING_TEXT_H

#include <QString>
#include <QMap>
#include <QChar>

class UtilsStaticAnalizingText {
public:
    QMap<QChar, double> calc(const QString &text);
};

#endif
