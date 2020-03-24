#ifndef WSJCPP_EMPLOYEES_H
#define WSJCPP_EMPLOYEES_H

#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// base employ class

class WSJCppEmployBase {
public:
    WSJCppEmployBase(
        const std::string &sName,
        const std::vector<std::string> &vLoadAfter
    );
    virtual ~WSJCppEmployBase();
    virtual bool init() = 0;
    virtual bool deinit() = 0;
    const std::vector<std::string> &loadAfter();

private:
    std::string TAG;
    std::string m_sName;
    std::vector<std::string> m_vLoadAfter;
};

// ---------------------------------------------------------------------
// public employees

extern std::map<std::string, WSJCppEmployBase*> *g_pWSJCppEmployees;
extern std::vector<std::string> *g_pWSJCppInitEmployees;

class WSJCppEmployees {
    public:
        static void initGlobalVariables();
        static void deinitGlobalVariables();
        static void addEmploy(const std::string &sName, WSJCppEmployBase* pEmploy);
        static bool init(const std::vector<std::string> &vLoadAfter);
        static bool deinit();
};

// ---------------------------------------------------------------------
// RegistryEmploy
#define REGISTRY_WJSCPP_EMPLOY( classname ) \
    static classname * pWJSCppRegistryEmploy ## classname = new classname(); \

// ---------------------------------------------------------------------
// findWSJCppEmploy

template <class T> T* findWSJCppEmploy() {
    WSJCppEmployees::initGlobalVariables();
    std::string TAG = "findWSJCppEmploy";
    std::string sEmployName = T::name();
    WSJCppEmployBase *pEmploy = NULL;
    if (g_pWSJCppEmployees->count(sEmployName)) {
        pEmploy = g_pWSJCppEmployees->at(sEmployName);
    }
    if (pEmploy == NULL) {
        WSJCppLog::throw_err(TAG, "Not found employ " + sEmployName);
    }
    T *pTEmploy = dynamic_cast<T*>(pEmploy);
    if (pTEmploy == NULL) {
        WSJCppLog::throw_err(TAG, "Employ could not cast to T [" + sEmployName + "]");
    }
    return pTEmploy;
}

// ---------------------------------------------------------------------
// WJSCppEmployRuntimeGlobalCache

class WJSCppEmployRuntimeGlobalCache : public WSJCppEmployBase {
    public:
        WJSCppEmployRuntimeGlobalCache();
        static std::string name() { return "WJSCppEmployRuntimeGlobalCache"; }
        virtual bool init() override;
        virtual bool deinit() override;
        void set(const std::string &sName, const std::string &sValue);
        bool has(const std::string &sName);
        std::string get(const std::string &sName);

    private:
        std::string TAG;
        std::map<std::string, std::string> m_sStringMap;
};

#endif // WSJCPP_EMPLOYEES_H
