#ifndef PARSE_CONFIG_H
#define PARSE_CONFIG_H

#include <string>
#include <map>

class ParseConfig {

    public:
        ParseConfig(const std::string &sFilepathConf);
        bool load();

        bool has(const std::string &sParamName);
        std::string stringValue(const std::string &sParamName, const std::string &defaultValue);
        int intValue(const std::string &sParamName, int defaultValue);
        bool boolValue(const std::string &sParamName, bool defaultValue);

    private:
        std::string TAG;
        std::string m_sFilepathConf;
        std::map<std::string,std::string> m_mapConfigValues;
};

#endif // PARSE_CONFIG_H
