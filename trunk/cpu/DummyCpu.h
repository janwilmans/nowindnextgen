//! DummyCpu.h
#ifndef DUMMY_CPU_H
#define DUMMY_CPU_H

#include "cpu/Z80.h"

class DummyCpu : public Z80
{
    virtual emuTimeType ExecuteInstructionsUntil(emuTimeType endTime) 
    {
        emuTimeType localEmutime = Emulator::emuTime;
        do {
            localEmutime++;
                printf("localEmutime++: %lu (%ld)\n", localEmutime, localEmutime);
        }
		while((endTime - localEmutime) > 0);
		return localEmutime;
    }
};

#endif //DUMMY_CPU_H