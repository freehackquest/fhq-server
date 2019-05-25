#ifndef EXPORT_LIBWSJCPPCLI_JAVA_ANDROID_H
#define EXPORT_LIBWSJCPPCLI_JAVA_ANDROID_H

#include <string>

class ExportLibWsjCppCliJavaAndroid {
    public:
        ExportLibWsjCppCliJavaAndroid();
        void setVersion(const std::string &sVersion);
        void exportLib();
        
    private:
        std::string TAG;
        std::string m_sVersion;
        void exportPrepareDirs();
        void exportManifest();
        void exportBuildGradle();
        void exportAPImd();
};

#endif // EXPORT_LIBWSJCPPCLI_JAVA_ANDROID_H
