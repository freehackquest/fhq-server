#ifndef EXPORT_LIBWSJCPPCLI_PY_H
#define EXPORT_LIBWSJCPPCLI_PY_H

class ExportLibWsjCppCliPy {
    public:
        void exportLib();
    private:
        void exportSetupPy();
        void exportAPImd();
        void export__init__py();
        void exportPrepareDirs();
};

#endif // EXPORT_LIBWSJCPPCLI_PY_H
