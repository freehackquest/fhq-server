#ifndef MODEL_LEAK_H
#define MODEL_LEAK_H

#include <string>

class ModelLeak {
    public:
        ModelLeak();
        const std::string &name();
    private:
        std::string TAG;
        std::string m_sName;
};

#endif // MODEL_LEAK_H
