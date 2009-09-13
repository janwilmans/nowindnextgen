//! CPU.h
#ifndef CPU_H
#define CPU_H

#include "basetypes.h" 

using namespace nowind;

class Bus;

class CPU {

public:
    CPU(Bus& aBus);
    virtual ~CPU() {}
    virtual emuTimeType ExecuteInstructions(emuTimeType startTime, emuTimeType endTime) = 0;

protected:
        Bus& mBus;

        MemReadDelegate readPage[4];
        MemReadDelegate writePage[4];
        
};

#endif //CPU_H
