#ifndef EMPLOY_BASE_H
#define EMPLOY_BASE_H

#include <string>
#include <vector>

class EmployBase {// : IEmploy {
public:
    EmployBase(
        const std::string &sName,
        const std::vector<std::string> &vLoadAfter);
private:
    std::string m_sName;
    std::vector<std::string> m_vLoadAfter;
};


#endif // EMPLOY_BASE_H
