//! CPU.h
#ifndef CPU_H
#define CPU_H

#include "basetypes.h" 
#include "FastDelegate.h"

using namespace fastdelegate;
using namespace nowind;

// for testing only, move to separate files....
class AddressBus {

};

class IOBus {

};

typedef fastdelegate::FastDelegate1<word, byte> MemReadDelegate;   // 1 parameter, returns byte 
typedef fastdelegate::FastDelegate2<word, byte> MemWriteDelegate;   // 2 parameters, returns void 

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
