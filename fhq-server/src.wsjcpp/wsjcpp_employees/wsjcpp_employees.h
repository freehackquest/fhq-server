#ifndef WSJCPP_EMPLOYEES_H
#define WSJCPP_EMPLOYEES_H

#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// base employ class

class WsjcppEmployBase {
public:
    WsjcppEmployBase(
        const std::string &sName,
        const std::vector<std::string> &vLoadAfter
    );
    virtual ~WsjcppEmployBase();
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

extern std::map<std::string, WsjcppEmployBase*> *g_pWsjcppEmployees;
extern std::vector<std::string> *g_pWsjcppInitEmployees;

class WsjcppEmployees {
    public:
        static void initGlobalVariables();
        static void deinitGlobalVariables();
        static void addService(const std::string &sName, WsjcppEmployBase* pEmploy);
        static bool init(const std::vector<std::string> &vLoadAfter);
        static bool deinit();
        static void recoursiveTestDependencies(std::vector<std::string> v);
};

// ---------------------------------------------------------------------
// RegistryServiceLocator
#define REGISTRY_WJSCPP_SERVICE_LOCATOR( classname ) \
    static classname * pWJSCppRegistryServiceLocator ## classname = new classname();

// ---------------------------------------------------------------------
// findWsjcppEmploy

template <class T> T* findWsjcppEmploy() {
    WsjcppEmployees::initGlobalVariables();
    std::string TAG = "findWsjcppEmploy";
    std::string sEmployName = T::name();
    WsjcppEmployBase *pEmploy = NULL;
    if (g_pWsjcppEmployees->count(sEmployName)) {
        pEmploy = g_pWsjcppEmployees->at(sEmployName);
    }
    if (pEmploy == NULL) {
        WsjcppLog::throw_err(TAG, "Not found employ " + sEmployName);
    }
    T *pTEmploy = dynamic_cast<T*>(pEmploy);
    if (pTEmploy == NULL) {
        WsjcppLog::throw_err(TAG, "Employ could not cast to T [" + sEmployName + "]");
    }
    return pTEmploy;
}

// ---------------------------------------------------------------------
// WsjcppEmployRuntimeGlobalCache

class WsjcppEmployRuntimeGlobalCache : public WsjcppEmployBase {
    public:
        WsjcppEmployRuntimeGlobalCache();
        static std::string name() { return "WsjcppEmployRuntimeGlobalCache"; }
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
