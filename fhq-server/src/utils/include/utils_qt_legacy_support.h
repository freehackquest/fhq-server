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
inline void to_json(nlohmann::json & j, const QString& str) {
    auto test = nlohmann::json::parse( str.toStdString(), nullptr, false);
    if (test.is_discarded() || test.is_null()){
        j = str.toStdString();
        return;
    } else {
        j = std::move(test);
    }
}

//--------------------------------

inline void from_json(const nlohmann::json & j, QString& str) {
    if(j.type() == nlohmann::json::value_t::string){
        str = QString::fromStdString( j.get_ref<std::string const&>() );
    } else {
        str = QString::fromStdString( j.dump() );
    }
}

//--------------------------------

// rdv: remove this when get rid of QSql
inline void to_json(nlohmann::json & j, const QVariant& vr) {

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
        throw std::runtime_error ("to_json(nlohmann::json & j, const QVariant& vr): not implemented{vr.type = "
                                        + std::string(vr.typeName()) + "}" );
    }
}

//--------------------------------

inline void from_json(const nlohmann::json & j, QVariant& vr) {

    using value_t = nlohmann::json::value_t;
    switch (j.type()) {
    case value_t::object:
    case value_t::array:
    case value_t::string:
        vr = j.get<QString>();
        break;
    case value_t::boolean:
        vr = j.get<bool>();
        break;
    case value_t::number_integer:
        vr = j.get<int>();
        break;
    case value_t::number_unsigned:
        vr = j.get<uint>();
        break;
    case value_t::number_float:
        vr = j.get<double>();
        break;
    default:
        throw std::runtime_error ("from_json(nlohmann::json & j, const QVariant& vr): not implemented{j.type: "
                                        + std::string(j.type_name()) +  "}");
    }
}

//--------------------------------

#endif // NO_LEGACY_FOR_JSON

#endif // UTILS_QT_LEGACY_SUPPORT_H
