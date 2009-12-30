//! DummyCpu.h
#ifndef DUMMY_CPU_H
#define DUMMY_CPU_H

#include "debug.h"
#include "cpu/CPU.h"

namespace nowind {

class DummyCpu : public CPU
{
public:
    DummyCpu(Emulator& aEmulator) : CPU(aEmulator) {}

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
    virtual void prepare() {}
    virtual void prepare_shutdown() {}
    virtual void initialize() {}
    virtual void reset() {}
};



} // namespace nowind

#endif //DUMMY_CPU_H