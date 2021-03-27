#ifndef WSJCPP_PARSE_CONF_H
#define WSJCPP_PARSE_CONF_H

#include <string>
#include <map>

class WsjcppParseConf {
    public:
        WsjcppParseConf(const std::string &sFilepathConf);
        bool load();

        bool has(const std::string &sParamName);
        std::string getStringValue(const std::string &sParamName, const std::string &defaultValue);
        int getIntValue(const std::string &sParamName, int defaultValue);
        bool getBoolValue(const std::string &sParamName, bool defaultValue);
        std::map<std::string,std::string> getValues();

    private:
        std::string TAG;
        std::string m_sFilepathConf;
        std::map<std::string,std::string> m_mapConfigValues;
};

#endif // WSJCPP_PARSE_CONF_H
