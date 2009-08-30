//! DummyCpu.h
#ifndef DUMMY_CPU_H
#define DUMMY_CPU_H

#include "cpu/Z80.h"

class DummyCpu : public Z80
{
    virtual void ExecuteInstructionsUntil(emuTimeType endTime) 
    {
        while (emuTime < endTime)
        {
            emuTime++;
            printf("emuTime++: %u\n", emuTime++);
        }
    }
};

#endif //DUMMY_CPU_H