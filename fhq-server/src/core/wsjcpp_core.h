#ifndef WSJCPP_CORE_H
#define WSJCPP_CORE_H

#include <string>
#include <mutex>
#include <deque>
#include <thread>
#include <vector>


class WSJCppJobAsync {
    public:
        WSJCppJobAsync(const std::string &sName);
        const std::string &name();
        virtual bool run(const std::string &sWorkerId) = 0;

    private:
        std::string m_sName;
};

// ---------------------------------------------------------------------


#endif // WSJCPP_CORE_H