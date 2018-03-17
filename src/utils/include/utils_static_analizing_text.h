#ifndef UTILS_STATIC_ANALIZING_TEXT_H
#define UTILS_STATIC_ANALIZING_TEXT_H

#include <QString>
#include <QMap>
#include <QChar>
#include <QRegularExpression>
#include <QDebug>

class UtilsStaticAnalizingText {
    public:
        static QMap<QChar, double> calc(const QString &text);
    private:
        static double calcWeightOfOneChar(int strLen); 
};

#endif
