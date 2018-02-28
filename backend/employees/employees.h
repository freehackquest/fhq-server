#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#include <iemploy.h>
#include <log.h>
#include <map>

extern std::map<std::string, IEmploy*> *g_pEmployees;
extern void initEmployees();

template <class T> T* findEmploy() {
    // Log::err("findEmploy", "Find employ start");
    if(g_pEmployees == NULL){
        initEmployees();
    }
    std::string sEmployName = T::getEmployName();
    // Log::err("findEmploy", "Find employ " + QString(sEmployName.c_str()));
    // Log::err("findEmploy", "Employees count: " + QString::number(g_pEmployees->size()));

    IEmploy *pEmploy = NULL;
    if(g_pEmployees->count(sEmployName)){
        pEmploy = g_pEmployees->at(sEmployName);
    }
    if(pEmploy == NULL){
        Log::err("findEmploy", "Not found employ " + QString(sEmployName.c_str()));
        return NULL;
    }
    T *pTEmploy = dynamic_cast<T*>(pEmploy);
    if(pTEmploy == NULL){
        Log::err("findEmploy", "Employ could not cast to T [" + QString(sEmployName.c_str()) + "]");
        return NULL;
    }
    return pTEmploy;
}

#endif // EMPLOYEES_H
