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

        // to be deleted....
        MemReadDelegate readPage[4];
        MemReadDelegate writePage[4];

        MemReadDelegate readSection[constSections];
        MemWriteDelegate writeSection[constSections];        
};

#endif //CPU_H
