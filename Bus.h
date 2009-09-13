#pragma once

#include "basetypes.h"

using namespace nowind;

// bank: in context of memorymapper a 16kb region in memory these are max. 256 banks
//       in context of slot-selection, half a page (so a 8kb region of memory)


class Bus 
{
public:
    Bus();
/*
    // the cpu should use these methods to register itself
    virtual void registerABReadConsumer(Uint8 bank, MemReadDelegate* delegate);
    virtual void registerABWriteConsumer(Uint8 bank, MemWriteDelegate* delegate);

    // the memory-mapped components should use theses methods to register to be called
    virtual void registerABReadProvider(Uint8 slot, Uint8 subslot, Uint8 bank, MemReadDelegate delegate);
    virtual void registerABWriteProvider(Uint8 slot, Uint8 subslot, Uint8 bank, MemWriteDelegate delegate);

    // the cpu should use these method to register itself
    virtual void registerIOReadConsumer(Uint8 bank, MemReadDelegate* delegate);
    virtual void registerIOWriteConsumer(Uint8 bank, MemWriteDelegate* delegate);

    // the IO-mapped components should use these methods to register to be called
    virtual void registerIOReadProvider(Uint8 slot, Uint8 subslot, Uint8 bank, MemReadDelegate delegate);
    virtual void registerIOWriteProvider(Uint8 slot, Uint8 subslot, Uint8 bank, MemWriteDelegate delegate);
*/
    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~Bus();
};

