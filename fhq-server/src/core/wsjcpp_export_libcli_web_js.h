#ifndef WSJCPP_EXPORT_LIBCLI_WEB_JS_H
#define WSJCPP_EXPORT_LIBCLI_WEB_JS_H

#include <vector>
#include <string>

namespace WsjCpp {
class ExportLibCliWebJS {
    public:
        ExportLibCliWebJS();
        void setPackageName(const std::string &sPackageName);
        void setLibraryName(const std::string &sLibraryName);
        void setAppName(const std::string &sAppName);
        void setAppVersion(const std::string &sAppVersion);
        void setAuthor(const std::string &sAuthor);
        void setPrefixRepositoryURL(const std::string &sPrefixRepositoryURL);
        void exportLib();

    private:
        std::string m_sPackageName;
        std::string m_sLibraryName;
        std::string m_sAppName;
        std::string m_sAppVersion;
        std::string m_sAuthor;
        std::string m_sPrefixRepositoryURL;
        std::vector<std::string> m_vEvents;

        void exportPackageJson(const std::string &sBasicDir);
        void exportAPImd(const std::string &sBasicDir);
        void exportLibCliWebJSFile(const std::string &sBasicDir);
        void exportLibCliWebServiceTSFile(const std::string &sBasicDir);
        void exportSampleHtmlFile(const std::string &sBasicDir);
        void exportPrepareDirs(const std::string &sBasicDir);
};

} // namespace WsjCpp

#endif // WSJCPP_EXPORT_LIBCLI_WEB_JS_H
