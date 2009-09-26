//! CPU.h
#ifndef CPU_H
#define CPU_H

#include "basetypes.h" 
#include "Component.h"

namespace nowind {

class Bus;

class CPU : public Component
{
public:
    CPU(Bus& aBus);
    virtual ~CPU() {}
    virtual emuTimeType ExecuteInstructions(emuTimeType startTime, emuTimeType endTime) = 0;

protected:
    Bus& mBus;

    // not implemented
    virtual void prepare() = 0;
    virtual void initialize() = 0;
    virtual void prepare_shutdown() = 0;       
};

} // namespace nowind

#endif //CPU_H
