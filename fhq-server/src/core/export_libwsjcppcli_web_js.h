#ifndef EXPORT_LIBWSJCPPCLI_WEB_JS_H
#define EXPORT_LIBWSJCPPCLI_WEB_JS_H

#include <string>

class ExportLibWsjCppCliWebJS {
    public:
        void setPackageName(const std::string &sPackageName);
        void setLibraryName(const std::string &sLibraryName);
        void setVersion(const std::string &sVersion);
        void setAuthor(const std::string &sAuthor);
        void setPrefixRepositoryURL(const std::string &sPrefixRepositoryURL);
        void exportLib();

    private:
        std::string m_sPackageName;
        std::string m_sLibraryName;
        std::string m_sVersion;
        std::string m_sAuthor;
        std::string m_sPrefixRepositoryURL;

        void exportPackageJson(const std::string &sBasicDir);
        void exportAPImd(const std::string &sBasicDir);
        void exportLibWsjCppCliWebJSFile(const std::string &sBasicDir);
        void exportSampleHtmlFile(const std::string &sBasicDir);
        void exportPrepareDirs(const std::string &sBasicDir);
};

#endif // EXPORT_LIBWSJCPPCLI_WEB_JS_H
