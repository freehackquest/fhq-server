#ifndef DATATYPE_BASE_H
#define DATATYPE_BASE_H

#include <string>

enum DataTypeEnum {
    STRING,
    INTEGER,
    STRING_ENUM,
    GAME_STATE
};

class DataTypeBase {
public:
    virtual DataTypeEnum type() = 0;
    virtual std::string toString() = 0;

protected:
    DataTypeEnum m_nType;

private:

};

#endif // DATATYPE_BASE_H