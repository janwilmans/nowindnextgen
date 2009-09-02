//! DummyCpu.h
#ifndef DUMMY_CPU_H
#define DUMMY_CPU_H

#include "cpu/Z80.h"

class DummyCpu : public Z80
{
    virtual emuTimeType ExecuteInstructionsUntil(emuTimeType endTime) 
    {
		while (Emulator::emuTime < endTime)
        {
            Emulator::emuTime++;
            printf("emuTime++: %u\n", Emulator::emuTime);
        }
		return Emulator::emuTime;
    }
};

#endif //DUMMY_CPU_H