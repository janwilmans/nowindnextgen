#include "Bus.h"
#include "Scheduler.h"
#include "Component.h"
#include "IODevice.h"

using namespace nowind;

Bus::Bus(Scheduler& aScheduler) :  
mScheduler(aScheduler)
{

}

Bus::~Bus()
{

}

void Bus::registerReadIO(Uint8 port, IOReadDelegate aDelegate)
{
    mIORead[port] = aDelegate;
}

void Bus::registerWriteIO(Uint8 port, IOWriteDelegate aDelegate)
{
    mIOWrite[port] = aDelegate;
}

byte Bus::readIO(Uint8 port)
{
    return mIORead[port](port);
}

void Bus::writeIO(Uint8 port, byte value)
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
    MemReadDelegate& temp = *mMemRead[section];
    temp = aDelegate;
    //(*mMemRead[section]) = aDelegate;
}

void Bus::activateMemWriteSection(Uint8 section, MemWriteDelegate aDelegate)
{
    (*mMemWrite[section]) = aDelegate;
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
