//! CPU.h
#ifndef CPU_H
#define CPU_H

#include "basetypes.h" 

using namespace nowind;

// for testing only, move to separate files....
class AddressBus {

};

class IOBus {

};

class CPU {

public:
    CPU(AddressBus& addressBus, IOBus& ioBus);
    virtual ~CPU() {}
    virtual emuTimeType ExecuteInstructions(emuTimeType startTime, emuTimeType endTime) = 0;

protected:
        AddressBus& mAddressBus;
        IOBus& mIOBus;

        MemReadDelegate readPage[4];
        MemReadDelegate writePage[4];
        
};

#endif //CPU_H
