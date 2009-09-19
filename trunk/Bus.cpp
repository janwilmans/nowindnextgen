#include "Bus.h"
#include "Scheduler.h"

using namespace nowind;

Bus::Bus(Scheduler& aScheduler) :  
mScheduler(aScheduler)
{

}

Bus::~Bus()
{

}

void Bus::addBusComponent(BusComponent *)
{

}

// memory access
void Bus::registerMemReadConsumer(Uint8 bank, MemReadDelegate* delegate)
{

}

void Bus::registerMemWriteConsumer(Uint8 bank, MemWriteDelegate* delegate)
{

}

// the memory-mapped components should use these methods to register to be called
void Bus::registerMemReadProvider(Uint8 slot, Uint8 subslot, Uint8 bank, MemReadDelegate delegate)
{

}
void Bus::registerMemWriteProvider(Uint8 slot, Uint8 subslot, Uint8 bank, MemWriteDelegate delegate)
{

}

// SSSR (Sub Slot Selection Register 0xFFFF) access
    // the cpu should use these methods to register itself
void Bus::registerSSSRReadConsumer(MemReadDelegate* delegate)
{

}
void Bus::registerSSSRWriteConsumer(MemWriteDelegate* delegate)
{

}

    // the memory-mapped components should use these methods to register to be called
void Bus::registerSSSRReadProvider(MemReadDelegate delegate)
{

}
void Bus::registerSSSRWriteProvider(MemWriteDelegate delegate)
{

}

// IO access
    // the cpu should use these methods to register itself
void Bus::registerIOReadConsumer(IOReadDelegate* delegate)
{

}
void Bus::registerIOWriteConsumer(IOWriteDelegate* delegate)
{

}

    // the IO-mapped components should use these methods to register to be called
void Bus::registerIOReadProvider(Uint8 port, IOReadDelegate delegate)
{

}
void Bus::registerIOWriteProvider(Uint8 port, IOWriteDelegate delegate)
{

}
