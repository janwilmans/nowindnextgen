//! Bus.h
#ifndef BUS_H
#define BUS_H

#include "basetypes.h"

// bank: in context of memorymapper a 16kb region in memory these are max. 256 banks
//       in context of slot-selection, half a page (so a 8kb region of memory)

namespace nowind {

class Scheduler;
class BusComponent;

class Bus 
{
private:
    Uint8 selectedMainSlot[4];
    Uint8 selectedSubSlot[4];
    bool slotExpanded[4];

public:
    Bus(Scheduler&);

    virtual void addBusComponent(BusComponent *);

// memory access
    // the cpu should use these methods to register itself
    virtual void registerMemReadConsumer(Uint8 bank, MemReadDelegate* delegate);
    virtual void registerMemWriteConsumer(Uint8 bank, MemWriteDelegate* delegate);

    // the memory-mapped components should use these methods to register to be called
    virtual void registerMemReadProvider(Uint8 slot, Uint8 subslot, Uint8 bank, MemReadDelegate delegate);
    virtual void registerMemWriteProvider(Uint8 slot, Uint8 subslot, Uint8 bank, MemWriteDelegate delegate);

// SSSR (Sub Slot Selection Register 0xFFFF) access
    // the cpu should use these methods to register itself
    virtual void registerSSSRReadConsumer(MemReadDelegate* delegate);
    virtual void registerSSSRWriteConsumer(MemWriteDelegate* delegate);

    // the memory-mapped components should use these methods to register to be called
    virtual void registerSSSRReadProvider(MemReadDelegate delegate);
    virtual void registerSSSRWriteProvider(MemWriteDelegate delegate);

// IO access
    // the cpu should use these methods to register itself
    virtual void registerIOReadConsumer(IOReadDelegate* delegate);
    virtual void registerIOWriteConsumer(IOWriteDelegate* delegate);

    // the IO-mapped components should use these methods to register to be called
    virtual void registerIOReadProvider(Uint8 port, IOReadDelegate delegate);
    virtual void registerIOWriteProvider(Uint8 port, IOWriteDelegate delegate);

    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~Bus();
protected:
    Scheduler& mScheduler;

};

} // nowind namespace 

#endif // BUS_H
