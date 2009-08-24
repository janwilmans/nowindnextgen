#include "Scheduler.h"
#include <stdio.h>

static Uint32 Ranges = 4;

Scheduler::Scheduler()
{
}

Scheduler::~Scheduler(void)
{
}

Uint32 Scheduler::Range(emuTimeType aTime)
{
    Uint32 lSizeOfRange = sizeof(emuTimeType);
    printf("lSizeOfRange: %u\n", lSizeOfRange);
    
    return 4;
}

bool Scheduler::ExecuteInterrupt(emuTimeType aIntTime)
{
    Uint32 lRangeInt = Range(aIntTime);
    Uint32 lNextIntRange = (lRangeInt+1) & (Ranges-1);
    Uint32 lRange = Range(mTime);

    if (lRange == lNextIntRange ) return true;
    if (lRangeInt == lRange && lRangeInt <= lRange) return true;
    return false;
}

bool Scheduler::NextRange(emuTimeType aIntTime, emuTimeType aEmuTime)
{
    Uint32 lRangeInt = Range(aIntTime);
    Uint32 lNextIntRange = (lRangeInt+1) & (Ranges-1);
    Uint32 lRangeEmu = Range(aEmuTime);
    return lNextIntRange == lRangeEmu;
}
