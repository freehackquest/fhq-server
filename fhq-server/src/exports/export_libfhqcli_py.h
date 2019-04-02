#ifndef EXPORT_LIBFHQCLI_PY_H
#define EXPORT_LIBFHQCLI_PY_H

class ExportLibFHQCliPy {
    public:
        static void exportLib();
    private:
        static void exportSetupPy();
        static void exportAPImd();
        static void export__init__py();
        static void exportPrepareDirs();
};

#endif // EXPORT_LIBFHQCLI_PY_H
