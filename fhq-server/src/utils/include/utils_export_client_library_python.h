#ifndef UTILS_EXPORT_CLIENT_LIBRARY_PYTHON_H
#define UTILS_EXPORT_CLIENT_LIBRARY_PYTHON_H

class UtilsExportClientLibraryPython {
    public:
        static void exportLib();
    private:
        static void exportSetupPy();
        static void exportAPImd();
        static void export__init__py();
        static void exportPrepareDirs();
};

#endif // UTILS_EXPORT_CLIENT_LIBRARY_PYTHON_H
