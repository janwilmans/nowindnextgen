#include "Bus.h"
#include "Scheduler.h"
#include "Component.h"
#include "IODevice.h"
#include "NullDevice.h"

using namespace nowind;
using namespace fastdelegate;

Bus::Bus(Scheduler& aScheduler) :  
mScheduler(aScheduler)
{
    NullDevice* nullDevice = new NullDevice(*this);     // leaked
    for (Uint16 port=0; port<256; port++)
    {
        registerReadIO(port, MakeDelegate(nullDevice, &NullDevice::readIO));
        registerWriteIO(port, MakeDelegate(nullDevice, &NullDevice::writeIO));
    }
}

Bus::~Bus()
{

}

void Bus::prepare()
{

}

void Bus::initialize()
{

}

void Bus::prepare_shutdown()
{

}

void Bus::registerReadIO(Uint16 port, IOReadDelegate aDelegate)
{
    mIORead[port] = aDelegate;
}

void Bus::registerWriteIO(Uint16 port, IOWriteDelegate aDelegate)
{
    mIOWrite[port] = aDelegate;
}

byte Bus::readIO(word port)
{
    return mIORead[port](port);
}

void Bus::writeIO(word port, byte value)
{
    mIOWrite[port](port, value);
}

void Bus::addIODevice(IODevice * aIODevice)
{
    aIODevice->attachIO();
}

void Bus::registerMemRead(Uint8 section, MemReadDelegate* aDelegate)
{
    mMemRead[section] = aDelegate;
}

void Bus::registerMemWrite(Uint8 section, MemWriteDelegate* aDelegate)
{
    mMemWrite[section] = aDelegate;
}

void Bus::activateMemReadSection(Uint8 section, MemReadDelegate aDelegate)
{
    *mMemRead[section] = aDelegate;
}

void Bus::activateMemWriteSection(Uint8 section, MemWriteDelegate aDelegate)
{
    *mMemWrite[section] = aDelegate;
}

void Bus::registerSSSRRead(SSSRReadDelegate* aDelegate)
{
    mSSSRRead = aDelegate;
}

void Bus::registerSSSRWrite(SSSRWriteDelegate* aDelegate)
{
    mSSSRWrite = aDelegate;
}

void Bus::activateSSSRRead(SSSRReadDelegate aDelegate)
{
   (*mSSSRRead) = aDelegate;
}

void Bus::activateSSSRWrite(SSSRWriteDelegate aDelegate)
{
   (*mSSSRWrite) = aDelegate;
}

void Bus::registerReadSectionMemory(Uint8 section, byte** readSectionMemory)
{
    mReadSectionMemory[section] = readSectionMemory;
}

void Bus::setReadSectionMemory(Uint8 section, byte* memory)
{
    *mReadSectionMemory[section] = memory;
}
