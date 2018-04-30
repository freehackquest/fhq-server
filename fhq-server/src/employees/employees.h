#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#include <log.h>
#include <map>
#include <string>
#include <vector>

// ---------------------------------------------------------------------
// base employ class

class EmployBase {
public:
    EmployBase(
        const std::string &sName,
        const std::vector<std::string> &vLoadAfter);
    virtual bool init() = 0;
    const std::vector<std::string> &loadAfter();
private:
    std::string TAG;
    std::string m_sName;
    std::vector<std::string> m_vLoadAfter;
};

// ---------------------------------------------------------------------
// public employees

extern std::map<std::string, EmployBase*> *g_pEmployees;
extern std::vector<std::string> *g_pInitEmployees;

class Employees {
    public:
        static void initGlobalVariables();
        static void addEmploy(const std::string &sName, EmployBase* pEmploy);
        static bool init(const std::vector<std::string> &vLoadAfter);
};

// ---------------------------------------------------------------------
// RegistryEmploy
#define REGISTRY_EMPLOY( classname ) \
    static classname * pRegistry ## classname = new classname(); \

// ---------------------------------------------------------------------
// findEmploy

template <class T> T* findEmploy() {
    Employees::initGlobalVariables();
    std::string TAG = "findEmploy";
    std::string sEmployName = T::name();
    EmployBase *pEmploy = NULL;
    if(g_pEmployees->count(sEmployName)){
        pEmploy = g_pEmployees->at(sEmployName);
    }
    if(pEmploy == NULL){
        Log::err(TAG, "Not found employ " + sEmployName);
        return NULL;
    }
    T *pTEmploy = dynamic_cast<T*>(pEmploy);
    if(pTEmploy == NULL){
        Log::err(TAG, "Employ could not cast to T [" + sEmployName + "]");
        return NULL;
    }
    return pTEmploy;
}

#endif // EMPLOYEES_H
