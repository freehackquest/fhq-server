#ifndef EXPORT_LIBFHQCLI_WEB_JS_H
#define EXPORT_LIBFHQCLI_WEB_JS_H

class ExportLibFHQCliWebJS {
    public:
        static void exportLib();
    private:
        static void exportPackageJson();
        static void exportAPImd();
        static void export__init__py();
        static void exportPrepareDirs();
};

#endif // EXPORT_LIBFHQCLI_WEB_JS_H
