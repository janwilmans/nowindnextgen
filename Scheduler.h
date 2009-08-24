#pragma once

#include "msxtypes.h"

class Scheduler
{
public:
    Scheduler();
    virtual ~Scheduler(void);
    static Uint32 Range(emuTimeType aTime);
    bool ExecuteInterrupt(emuTimeType aIntTime);
    static bool NextRange(emuTimeType aIntTime, emuTimeType aEmuTime);

protected:
    emuTimeType mTime;
    Uint32 mRange;

};
