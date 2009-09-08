//! DummyCpu.h
#ifndef DUMMY_CPU_H
#define DUMMY_CPU_H

#include "debug.h"
#include "cpu/Z80.h"

class DummyCpu : public Z80
{
public:
    // find out why this is needed!!
    DummyCpu(AddressBus& addressBus, IOBus& ioBus) : Z80(addressBus, ioBus) {}

    virtual emuTimeType ExecuteInstructionsUntil(emuTimeType endTime) 
    {
        emuTimeType localEmutime = Emulator::emuTime;
        do {
            localEmutime++;
            DBERR("localEmutime++: %lu (%ld)\n", localEmutime, localEmutime);
        }
		while((endTime - localEmutime) > 0);
		return localEmutime;
    }
};

#endif //DUMMY_CPU_H