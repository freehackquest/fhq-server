#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#include <iemploy.h>
#include <map>

extern std::map<std::string, IEmploy*> *g_pEmployees;
extern void initEmployees();

template <class T> T* findEmploy() {
    if(g_pEmployees == NULL){
        initEmployees();
    }
    std::string sEmployName = T::getEmployName();
    IEmploy *pEmploy = NULL;
    if(g_pEmployees->count(sEmployName)){
        pEmploy = g_pEmployees->at(sEmployName);
    }
    return pEmploy != NULL ? dynamic_cast<T*>(pEmploy) : NULL;
}

#endif // EMPLOYEES_H
