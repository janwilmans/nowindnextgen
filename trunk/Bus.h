#pragma once

#include "basetypes.h"

// bank: in context of memorymapper a 16kb region in memory these are max. 256 banks
//       in context of slot-selection, half a page (so a 8kb region of memory)


class Bus 
{
public:
    Bus();

    // the cpu should use these method to register itself
    virtual void registerReadCallback(Uint8 bank, MemReadDelegate* delegate);
    virtual void registerWriteCallback(Uint8 bank, MemWriteDelegate* delegate);

    // the components should use these methods to register to be called
    virtual void registerReadDispatch(Uint8 slot, Uint8 subslot, Uint8 bank, MemReadDelegate delegate);
    virtual void registerWriteDispatch(Uint8 slot, Uint8 subslot, Uint8 bank, MemWriteDelegate delegate);

    // the destructor should release any allocated resources (memory/filehandles etc.. during runtime 
    virtual ~Bus() {}
};

