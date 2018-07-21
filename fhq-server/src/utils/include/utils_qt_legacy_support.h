#ifndef UTILS_QT_LEGACY_SUPPORT_H
#define UTILS_QT_LEGACY_SUPPORT_H

#include <json.hpp>
#include <QString>
#include <QVariant>

#ifndef NO_LEGACY_FOR_JSON

#ifndef NO_LEGACY_WARNING
    #ifdef _MSC_VER
        #pragma message ( "WARNING: This functionality is deprecated and not recommended for use" )
    #else
        #warning WARNING: This functionality is deprecated and not recommended for use
    #endif
#endif

//--------------------------------

// rdv: remove this when replace qstring with std::string when working with nlohmann::json
inline void to_json(nlohmann::json & j, const QString& str)
{
    j = str.toStdString();
}

//--------------------------------

inline void from_json(const nlohmann::json & j, QString& str)
{
    str = QString::fromStdString( j.get_ref<std::string const&>() );
}

//--------------------------------

// rdv: remove this when get rid of QSql
inline void to_json(nlohmann::json & j, const QVariant& vr)
{

    switch (vr.type()) {

    case QVariant::String:
        j = vr.toString();
        return;
    case QVariant::UInt:
        j = vr.toUInt();
        return;
    case QVariant::Int:
        j = vr.toInt();
        return;
    case QVariant::Double:
        j = vr.toDouble();
        return;
    default:
        throw std::runtime_error ("to_json(nlohmann::json & j, const QVariant& vr): not implemented");
    }
}

//--------------------------------

inline void from_json(const nlohmann::json & j, QVariant& str)
{
    str = QString::fromStdString( j.get_ref<std::string const&>() );
}

//--------------------------------

#endif // NO_LEGACY_FOR_JSON

#endif // UTILS_QT_LEGACY_SUPPORT_H
