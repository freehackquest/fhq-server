#pragma once
#ifndef PARSER_IP_API_COM_H
#define PARSER_IP_API_COM_H

#include <string>

class ParserIpApiCom {
    public:
        ParserIpApiCom();
        bool parse(const std::string &sIpAddress, const std::string &sJson);
        std::string getCountry();
        std::string getRegionName();
        std::string getCity();
        double getLat();
        double getLon();

    private:
        std::string TAG;
        std::string m_sCountry;
        std::string m_sRegionName;
        std::string m_sCity;
        double m_nLat;
        double m_nLon;
};

#endif // PARSER_IP_API_COM_H