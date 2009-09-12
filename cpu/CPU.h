//! CPU.h
#ifndef CPU_H
#define CPU_H

#include "msxtypes.h" 

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
    virtual emuTimeType ExecuteInstructionsUntil(emuTimeType startTime, emuTimeType endTime) = 0;

protected:
        AddressBus& mAddressBus;
        IOBus& mIOBus;
};

#endif //CPU_H
