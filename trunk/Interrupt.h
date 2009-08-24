#pragma once

#include "msxtypes.h"
#include <string>

class Interrupt
{
public:
    Interrupt(emuTimeType);
    virtual ~Interrupt(void);

    bool IsExpired(emuTimeType);
    std::string ToString();

protected:

    emuTimeType mTime;
};
