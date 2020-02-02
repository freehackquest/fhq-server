
#include "parser_ip_api_com.h"
#include <json.hpp>
#include <core/fallen.h>

// ----------------------------------------------------------------------

ParserIpApiCom::ParserIpApiCom() {
    TAG = "ParserIpApiCom";
}

// ----------------------------------------------------------------------

bool ParserIpApiCom::parse(const std::string &sIpAddress, const std::string &sJson) {

    if (!nlohmann::json::accept(sJson)) {
        Log::err(TAG, "ip-api.com/json/" + sIpAddress + ", Expected json: " + sJson);
        return false;
    }

    nlohmann::json obj = nlohmann::json::parse(sJson);
    
    try {
        std::string sStatus = obj.at("status").get<std::string>();
        if (sStatus == "fail") {
            Log::err(TAG, "ip-api.com/json/" + sIpAddress + " -> status: fail " + sJson);
            return false;
        }
    } catch (const std::exception &e) {
        Log::err(TAG, "ip-api.com/json/" + sIpAddress + " -> wrong field 'status' in struct \n" + sJson + "\n" + std::string(e.what()));
        return false;
    }

    try {
        m_sCountry = obj.at("country").get<std::string>();
    } catch (const std::exception &e) {
        Log::err(TAG, "ip-api.com/json/" + sIpAddress + " -> wrong field 'country' in struct \n" + sJson + "\n" + std::string(e.what()));
        m_sCountry = "";
    }

    try {
        m_sRegionName = obj.at("regionName").get<std::string>();
    } catch (const std::exception &e) {
        Log::err(TAG, "ip-api.com/json/" + sIpAddress + " -> wrong field 'regionName' in struct \n" + sJson + "\n" + std::string(e.what()));
        m_sRegionName = "";
    }

    try {
        m_sCity = obj.at("city").get<std::string>();
    } catch (const std::exception &e) {
        Log::err(TAG, "ip-api.com/json/" + sIpAddress + " -> wrong field 'city' in struct \n" + sJson + "\n" + std::string(e.what()));
        m_sCity = "";
    }

    try {
        m_nLat = obj.at("lat").get<double>();
    } catch (const std::exception &e) {
        Log::err(TAG, "ip-api.com/json/" + sIpAddress + " -> wrong field 'lat' in struct \n" + sJson + "\n" + std::string(e.what()));
        m_nLat = 0.0;
    }

    try {
        m_nLon = obj.at("lon").get<double>();
    } catch (const std::exception &e) {
        Log::err(TAG, "ip-api.com/json/" + sIpAddress + " -> wrong field 'lon' in struct \n" + sJson + "\n" + std::string(e.what()));
        m_nLon = 0.0;
    }

    return true;
}

// ----------------------------------------------------------------------

std::string ParserIpApiCom::getCountry() {
    return m_sCountry;
}

// ----------------------------------------------------------------------

std::string ParserIpApiCom::getRegionName() {
    return m_sRegionName;
}

// ----------------------------------------------------------------------

std::string ParserIpApiCom::getCity() {
    return m_sCity;    
}

// ----------------------------------------------------------------------

double ParserIpApiCom::getLat() {
    return m_nLat;    
}

// ----------------------------------------------------------------------

double ParserIpApiCom::getLon() {
    return m_nLon;
}

// ----------------------------------------------------------------------