#ifndef EXPORT_LIBFHQCLI_WEB_JS_H
#define EXPORT_LIBFHQCLI_WEB_JS_H

#include <string>

class ExportLibFHQCliWebJS {
    public:
        static void exportLib();
    private:
        static void exportPackageJson(const std::string &sBasicDir);
        static void exportAPImd(const std::string &sBasicDir);
        static void exportLibfhqcliWebJSFile(const std::string &sBasicDir);
        static void exportPrepareDirs(const std::string &sBasicDir);
};

#endif // EXPORT_LIBFHQCLI_WEB_JS_H
