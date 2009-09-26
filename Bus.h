//! Bus.h
#ifndef BUS_H
#define BUS_H

#include "basetypes.h"

// bank: in context of memoryMemoryMapper a 16kb region in memory these are max. 256 banks
//       in context of slot-selection, half a page (so a 8kb region of memory)

#include "Scheduler.h"

namespace nowind {

class BusComponent;
class Scheduler;
class IODevice;

class Bus 
{
public:
    Bus(Scheduler&);

    // add an IO-mapped only device
    void addIODevice(IODevice * aIODevice);
    
    // BusComponents use this to register
    void registerReadIO(Uint8 port, IOReadDelegate aDelegate);
    void registerWriteIO(Uint8 port, IOWriteDelegate aDelegate);

    // called by the CPU or other bus controller
    byte readIO(Uint8 port);
    void writeIO(Uint8 port, byte value);

    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~Bus();
protected:
    Scheduler& mScheduler;
private:

    // memory access
    MemReadDelegate* mMemRead[constSections];
    MemWriteDelegate* mMemWrite[constSections];

    // subslot selector access
    MemReadDelegate* mSSSRRead;
    MemWriteDelegate* mSSSRWrite;

    // I/O access
    IOReadDelegate mIORead[256];
    IOWriteDelegate mIOWrite[256];
};

} // nowind namespace 

#endif // BUS_H
