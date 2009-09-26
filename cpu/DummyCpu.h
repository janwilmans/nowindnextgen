//! DummyCpu.h
#ifndef DUMMY_CPU_H
#define DUMMY_CPU_H

#include "debug.h"
#include "cpu/Z80.h"

namespace nowind {

class DummyCpu : public Z80
{
public:
    // find out why this is needed!!
    DummyCpu(Bus& bus) : Z80(bus) {}

    virtual emuTimeType ExecuteInstructions(emuTimeType startTime, emuTimeType endTime) 
    {
        emuTimeType localEmutime = startTime;
        do {
            localEmutime++;
            DBERR("localEmutime++: %lu (%ld)\n", localEmutime, localEmutime);
        }
		while((endTime - localEmutime) > 0);
		return localEmutime;
    }
};

} // namespace nowind

#endif //DUMMY_CPU_H