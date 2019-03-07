#ifndef DATATYPE_GAME_STATE_H
#define DATATYPE_GAME_STATE_H

#include <string>
#include <vector>
#include "datatype_base.h"

// extern std::vector<std::string> *g_vGameStates;

class DataTypeGameState {
public:
    DataTypeGameState();
    DataTypeGameState(const std::string &sType);
    virtual DataTypeEnum type();
    virtual std::string toString();

private:
    std::string TAG;
    std::vector<std::string> m_vDataTypes;
};

#endif // DATATYPE_GAME_STATE_H