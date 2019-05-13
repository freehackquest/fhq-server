#ifndef CONFIG_H
#define CONFIG_H
#include <string>
#include <vector>
#include "json.hpp"

class CodeCheckConfig {
    public:
        CodeCheckConfig();
        void applyArguments(int argc, char** argv);
        std::string getRootDir();
        nlohmann::json getJsonConfig();
        bool isShowOnlyErrors();
        
    private:
        std::string TAG;
        std::vector<std::string> m_vArgs;
        std::string m_sRootDir;
        nlohmann::json m_jsonConfig;
        bool m_bShowOnlyErrors;
};

#endif // CONFIG_H